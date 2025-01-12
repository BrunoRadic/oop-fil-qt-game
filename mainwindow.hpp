#pragma once

#include <QMainWindow>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

#include <QTimer>
#include <QKeyEvent>
#include <QtQml/QQmlEngine>
#include <memory>

#include "platform.hpp"
#include "collectible.hpp"
#include "finishline.hpp"

namespace Ui {
class MainWindow;
}

class Platform;
class Collectible;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();

private:


    void updatePlayerPhysics();
    void checkCollisions();
    void showGameOver();
    void showLevelComplete();
    void resetGame();


    void createLevel();


    std::unique_ptr<Ui::MainWindow> ui;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QTimer* timer;
    QQmlEngine qmlEngine;


    QGraphicsPixmapItem* player;
    QGraphicsPixmapItem* groundPlatform;
    FinishLine* finishLine;
    QList<Platform*> platforms;
    QList<Collectible*> collectibles;


    // Game state
    int score;
    bool gameOver;
    bool levelCompleted;

    // Player state
    double playerX;
    double playerY;
    double velocityX;
    double velocityY;
    int playerWidth;
    int playerHeight;
    bool jumping;
    bool canJump;
    int jumpCount;
    bool movingLeft;
    bool movingRight;


    QRectF levelBounds;

    // Constants
    const double gravity = 0.5;
    const double jumpForce = -12.0;
    const double moveSpeed = 5.0;
    const double maxVelocityX = 4.0;
    const double friction = 0.9;
    // const double maxFallSpeed = 15.0; removed

    // states
    QPixmap playerStandingRight;
    QPixmap playerStandingLeft;
    QPixmap playerJumpingRight;
    QPixmap playerJumpingLeft;
    bool facingRight;
    QPixmap playerWalkingRight;
    QPixmap playerWalkingLeft;


    void updatePlayerState();
};
