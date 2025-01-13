#include "mainwindow.hpp"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , scene(new QGraphicsScene(this))
    , view(new QGraphicsView(scene, this))
    , timer(new QTimer(this))
    , score(0)
    , gameOver(false)
    , levelCompleted(false)
    , playerX(20)
    , playerY(20)
    , velocityX(0)
    , velocityY(0)
    , playerWidth(40)
    , playerHeight(40)
    , jumping(false)
    , canJump(true)
    , jumpCount(0)
    , movingLeft(false)
    , movingRight(false)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Floor is lava");

    // initialize graphics
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setCentralWidget(view);

    // level size
    scene->setSceneRect(0, 0, 5000, 600);

    createLevel();

    // level boundaries
    levelBounds = scene->sceneRect();

    // game loop
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    // load player states
    if (!playerStandingRight.load(":/player_right.png")) {
        playerStandingRight = QPixmap(playerWidth, playerHeight);
        playerStandingRight.fill(Qt::blue);
    }
    if (!playerStandingLeft.load(":/player_left.png")) {
        playerStandingLeft = QPixmap(playerWidth, playerHeight);
        playerStandingLeft.fill(Qt::blue);
    }
    if (!playerJumpingRight.load(":/moving_right.png")) {
        playerJumpingRight = QPixmap(playerWidth, playerHeight);
        playerJumpingRight.fill(Qt::red);
    }
    if (!playerJumpingLeft.load(":/moving_left.png")) {
        playerJumpingLeft = QPixmap(playerWidth, playerHeight);
        playerJumpingLeft.fill(Qt::red);
    }
    if (!playerWalkingRight.load(":/moving_right.png")) {
        playerWalkingRight = QPixmap(playerWidth, playerHeight);
        playerWalkingRight.fill(Qt::green);
    }
    if (!playerWalkingLeft.load(":/moving_left.png")) {
        playerWalkingLeft = QPixmap(playerWidth, playerHeight);
        playerWalkingLeft.fill(Qt::green);
    }

    // scaling
    QSize visualSize(150, 200);
    playerStandingRight = playerStandingRight.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    playerStandingLeft = playerStandingLeft.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    playerJumpingRight = playerJumpingRight.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    playerJumpingLeft = playerJumpingLeft.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    playerWalkingRight = playerWalkingRight.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    playerWalkingLeft = playerWalkingLeft.scaled(visualSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // transparency
    QImage rightImage = playerStandingRight.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage leftImage = playerStandingLeft.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage jumpRightImage = playerJumpingRight.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage jumpLeftImage = playerJumpingLeft.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage walkRightImage = playerWalkingRight.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage walkLeftImage = playerWalkingLeft.toImage().convertToFormat(QImage::Format_ARGB32);

    // process each state
    for (QImage* image : {&rightImage, &leftImage, &jumpRightImage, &jumpLeftImage,
                         &walkRightImage, &walkLeftImage}) {
        for (int y = 0; y < image->height(); y++) {
            for (int x = 0; x < image->width(); x++) {
                QColor color = image->pixelColor(x, y);
                if (color.alpha() == 0) {
                    image->setPixelColor(x, y, QColor(0, 0, 0, 0));
                } else {
                    image->setPixelColor(x, y, QColor(color.red(), color.green(), color.blue(), 255));
                }
            }
        }
    }

    // back to QPixmap
    playerStandingRight = QPixmap::fromImage(rightImage);
    playerStandingLeft = QPixmap::fromImage(leftImage);
    playerJumpingRight = QPixmap::fromImage(jumpRightImage);
    playerJumpingLeft = QPixmap::fromImage(jumpLeftImage);
    playerWalkingRight = QPixmap::fromImage(walkRightImage);
    playerWalkingLeft = QPixmap::fromImage(walkLeftImage);

    // Create player
    player = new QGraphicsPixmapItem();
    player->setPos(playerX, playerY);
    scene->addItem(player);
    facingRight = true;
    updatePlayerState();
}

void MainWindow::checkCollisions() {
    QRectF playerRect(playerX, playerY, playerWidth, playerHeight);
    bool onGround = false;

    // lava
    QRectF groundRect = groundPlatform->boundingRect().translated(groundPlatform->pos());
    if (playerRect.intersects(groundRect) && !gameOver) {
        gameOver = true;
        timer->stop();
        showGameOver();
        return;
    }

    // platform collisions
    for (auto platform : platforms) {
        if (platform->checkCollision(playerRect, velocityX, velocityY)) {
            // top of a platform
            playerY = platform->pos().y() - playerHeight;

            if (platform->getType() == Platform::MovingVertical) {
                double platformVelocity = platform->getVerticalVelocity();


                if (!jumping) {
                    velocityY = platformVelocity;
                }

                // allow jumping
                canJump = true;
                jumpCount = 0;

                // reset jumping state going down with the platform
                if (velocityY >= 0 && !jumping) {
                    jumping = false;
                }
            } else {
                velocityY = 0;
                jumping = false;
                canJump = true;
                jumpCount = 0;
            }

            onGround = true;
        }
    }

    // collectible collision
    for (auto it = collectibles.begin(); it != collectibles.end(); ) {
        if ((*it)->checkCollision(playerRect)) {
            score += (*it)->getValue();
            scene->removeItem(*it);
            delete *it;
            it = collectibles.erase(it);
        } else {
            ++it;
        }
    }

    // finish line collision
    if (playerRect.intersects(finishLine->getHitbox()) && !levelCompleted) {
        levelCompleted = true;
        timer->stop();
        showLevelComplete();
    }


    if (!onGround && !jumping) {
        canJump = false;
    }


    player->setPos(playerX, playerY);
}

void MainWindow::showLevelComplete() {

    timer->stop();


    QGraphicsRectItem* overlay = new QGraphicsRectItem(view->mapToScene(view->rect()).boundingRect());
    overlay->setBrush(QColor(0, 0, 0, 127));
    overlay->setZValue(100);
    scene->addItem(overlay);


    QGraphicsTextItem* completeText = new QGraphicsTextItem();
    completeText->setHtml(QString("<div style='text-align: center;'>"
                                  "<h1 style='color: green;'>LEVEL COMPLETE!</h1>"
                                  "<p style='color: white;'>Final Score: %1</p>"
                                  "<p style='color: white;'>Press R to restart</p>"
                                  "<p style='color: white;'>Press ESC to quit</p></div>").arg(score));
    completeText->setDefaultTextColor(Qt::white);
    completeText->setZValue(101);


    QRectF textRect = completeText->boundingRect();
    QRectF viewRect = view->mapToScene(view->rect()).boundingRect();
    completeText->setPos(viewRect.center() - textRect.center());
    scene->addItem(completeText);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    // gameover
    if (gameOver || levelCompleted) {
        if (event->key() == Qt::Key_R) {
            resetGame();
            return;
        } else if (event->key() == Qt::Key_Escape) {
            close();
            return;
        }
        return;
    }


    switch (event->key()) {
        case Qt::Key_A:
            movingLeft = true;
            facingRight = false;
            break;
        case Qt::Key_D:
            movingRight = true;
            facingRight = true;
            break;
        case Qt::Key_Space:
        case Qt::Key_W:
            if (canJump && jumpCount < 2) {
                velocityY = jumpForce;
                jumping = true;
                jumpCount++;
                if (jumpCount == 2) canJump = false;
            }
            break;
    }
}

void MainWindow::resetGame() {

    timer->stop();

    // clear
    scene->clear();
    platforms.clear();
    collectibles.clear();


    score = 0;
    gameOver = false;
    levelCompleted = false;


    playerX = 20;
    playerY = 20;
    velocityX = 0;
    velocityY = 0;
    jumping = false;
    canJump = true;
    jumpCount = 0;
    movingLeft = false;
    movingRight = false;


    createLevel();


    player = new QGraphicsPixmapItem();
    player->setPos(playerX, playerY);
    scene->addItem(player);
    updatePlayerState();


    view->centerOn(player);


    timer->start();
}

void MainWindow::updateGame() {
    if (!gameOver && !levelCompleted) {
        updatePlayerPhysics();
        updatePlayerState();


        for (auto platform : platforms) {
            platform->updateMovement();  // update hover platforms
        }

        checkCollisions();
        player->setPos(playerX, playerY);
        view->centerOn(player);
    }
}


void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) return;

    switch (event->key()) {
    case Qt::Key_A:
    case Qt::Key_Left:
        movingLeft = false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        movingRight = false;
        break;
    }
}

MainWindow::~MainWindow() {
    delete scene;
    delete view;
    delete timer;
    delete player;
    delete groundPlatform;
    delete finishLine;

    qDeleteAll(platforms);
    qDeleteAll(collectibles);
}

void MainWindow::createLevel() {

    scene->setSceneRect(0, 0, 5000, 600);  // level dimensions

    QPixmap backgroundImage(":/smoke_background.png");
    if (backgroundImage.isNull()) {
        qDebug() << "Failed to load background image!";
    }

    // fit height while maintaining aspect ratio
    qreal targetHeight = 600.0;
    qreal scaleFactor = targetHeight / backgroundImage.height();
    QPixmap scaledImage = backgroundImage.scaled(backgroundImage.width() * scaleFactor,
                                                 targetHeight,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation);

    // wider pixmap to hold repeated backgrounds
    QPixmap repeatedBackground(scene->sceneRect().width(), targetHeight);
    repeatedBackground.fill(Qt::transparent);

    // paint repeatedly
    QPainter painter(&repeatedBackground);
    int scaledWidth = scaledImage.width();
    int numCopies = ceil(scene->sceneRect().width() / scaledWidth);

    for (int i = 0; i < numCopies; ++i) {
        painter.drawPixmap(i * scaledWidth, 0, scaledImage);
    }
    painter.end();

    // background item
    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(repeatedBackground);
    background->setZValue(-1);
    background->setPos(0, 0);
    scene->addItem(background);

    // view settings
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 600);  // Window size

    // fire
    QPixmap groundTexture(":/ground_fire.png");
    if (groundTexture.isNull()) {
        qDebug() << "Failed to load ground fire!";
    }

    groundPlatform = new QGraphicsPixmapItem(groundTexture);
    groundPlatform->setPos(0, 600);
    groundPlatform->setZValue(1);
    scene->addItem(groundPlatform);

    struct PlatformData {
        qreal x, y, width, height;
        Platform::Type type;
    };

    QPixmap hoverImage(":/DRACARYS.png");
    auto spawn = new Platform(20, 100, 170, 70, Platform::Normal);
    if (!hoverImage.isNull()) {
        hoverImage = hoverImage.scaled(170, 110, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        spawn->setPixmap(hoverImage);
    }
    scene->addItem(spawn);
    platforms.append(spawn);



    std::vector<PlatformData> hoverPlatforms = {

        {600, 200, 75, 20, Platform::Normal},
        {1100, 100, 75, 20, Platform::Normal},
        {1250, 300, 75, 20, Platform::Normal},
        {1400, 450, 75, 20, Platform::Normal},
        {1800, 100, 75, 20, Platform::Normal},
        {2000, 350, 75, 20, Platform::Normal},
        {2500, 75, 100, 20, Platform::Normal},
        {2850, 250, 75, 20, Platform::Normal},
        {3000, 250, 75, 20, Platform::Normal},
        {3700, 250, 75, 20, Platform::Normal},
        {3875, 250, 75, 20, Platform::Normal},
        {4050, 250, 75, 20, Platform::Normal},
        {4225, 250, 75, 20, Platform::Normal},

        };

    // hover platforms
    if (!hoverImage.isNull()) {
        QPixmap scaledHover = hoverImage.scaled(100, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        for (const auto& data : hoverPlatforms) {
            auto platform = new Platform(data.x, data.y, data.width, data.height, Platform::Normal);

            platform->setPixmap(scaledHover);
            scene->addItem(platform);
            platforms.append(platform);
        }
    }

    // pillars
    std::vector<PlatformData> levelPlatforms = {

    {250, 400, 75, 200, Platform::Normal},
    {450, 300, 75, 350, Platform::Normal},
    {2200, 300, 75, 350, Platform::Normal},
    {2700, 450, 75, 350, Platform::Normal},
    {3150, 350, 50, 350, Platform::Normal},
    {3600, 350, 50, 350, Platform::Normal},
    {4500, 300, 65, 350, Platform::Normal},
    {4600, 350, 100, 350, Platform::Normal},
    {4700, 350, 100, 350, Platform::Normal},
    {4800, 350, 100, 350, Platform::Normal},
    {4900, 350, 100, 350, Platform::Normal}
};


// platforms, coins
for (const auto& data : levelPlatforms) {
    auto platform = new Platform(data.x, data.y, data.width, data.height, data.type);
    scene->addItem(platform);
    platforms.append(platform);

    auto collectible = new Collectible((data.x + data.width/2)-20, data.y - 50);
    scene->addItem(collectible);
    collectibles.append(collectible);
}

std::vector<PlatformData> flyingPlatforms{

    {2400, 250, 100, 20, Platform::MovingVertical},
    {800, 400, 100, 20, Platform::MovingVertical},
    {1600, 400, 60, 20, Platform::MovingVertical},
    {3375, 400, 60, 20, Platform::MovingVertical}


};

// fl. platforms
for (const auto& data : flyingPlatforms) {
    auto platform = new Platform(data.x, data.y, data.width, data.height, data.type);
    scene->addItem(platform);
    platforms.append(platform);
}

// finish line pos
finishLine = new FinishLine(4850, 200);
finishLine->setZValue(1);
scene->addItem(finishLine);

// "FINISH"
QGraphicsTextItem* finishText = new QGraphicsTextItem("FINISH");
finishText->setDefaultTextColor(Qt::white);
finishText->setFont(QFont("Arial", 20, QFont::Bold));
finishText->setPos(4600, 50);
finishText->setRotation(90);
scene->addItem(finishText);


}

void MainWindow::updatePlayerPhysics() {
    // horizontalno
    if (movingLeft) {
        velocityX = std::max(velocityX - moveSpeed, -maxVelocityX);
    }
    if (movingRight) {
        velocityX = std::min(velocityX + moveSpeed, maxVelocityX);
    }

    // friction when not moving - bez klizim
    if (!movingLeft && !movingRight) {
        velocityX *= friction;
        if (std::abs(velocityX) < 0.1) velocityX = 0;
    }

    // gravity
    velocityY += gravity;

    // Update position
    playerX += velocityX;
    playerY += velocityY;

    // level boundaries
    playerX = std::max(0.0, std::min(playerX, levelBounds.right() - playerWidth));

    // Update player position
    player->setPos(playerX, playerY);
}


void MainWindow::showGameOver() {


    QRectF sceneRect = scene->sceneRect();
    QGraphicsRectItem* overlay = new QGraphicsRectItem(sceneRect);
    overlay->setBrush(QColor(0, 0, 0, 127));
    overlay->setZValue(100);
    scene->addItem(overlay);

    // game over
    QGraphicsTextItem* gameOverText = new QGraphicsTextItem();
    gameOverText->setHtml(QString("<div style='text-align: center;'>"
                                  "<h1 style='color: red; font-size: 48px;'>GAME OVER</h1>"
                                  "<p style='color: white; font-size: 24px;'>Final Score: %1</p>"
                                  "<p style='color: white; font-size: 24px;'>Press R to restart</p>"
                                  "<p style='color: white; font-size: 24px;'>Press ESC to quit</p></div>").arg(score));
    gameOverText->setDefaultTextColor(Qt::white);
    gameOverText->setZValue(101);

    // center
    QRectF textRect = gameOverText->boundingRect();
    QPointF center = view->mapToScene(view->viewport()->rect().center());
    gameOverText->setPos(center.x() - textRect.width()/2, center.y() - textRect.height()/2);
    scene->addItem(gameOverText);
}

void MainWindow::updatePlayerState() {
    if (!player) return;

    QPixmap currentState;


    // load image
    if (jumping) {
        // In the air
        currentState = facingRight ? playerJumpingRight : playerJumpingLeft;
    } else if (movingLeft || movingRight) {
        // On ground and moving
        currentState = movingRight ? playerWalkingRight : playerWalkingLeft;
    } else {
        // On ground and still
        currentState = facingRight ? playerStandingRight : playerStandingLeft;
    }
    // Center
    int xOffset = -(currentState.width() - playerWidth) / 2;
    int yOffset = -(currentState.height() - playerHeight) / 2;

    player->setPixmap(currentState);
    player->setOffset(xOffset, yOffset);
}




