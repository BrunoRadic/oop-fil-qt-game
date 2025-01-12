#include "collectible.hpp"
#include <QTimer>

Collectible::Collectible(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , m_value(100)
{
    animation = new QMovie(":/sparkly_sparkly.gif");
    animation->setScaledSize(QSize(50, 50));

    //  update frames
    QTimer* timer = new QTimer(animation);
    QEventLoop::connect(timer, &QTimer::timeout, [this]() {
        updateFrame();
    });
    timer->start(50);  // 50ms

    animation->start();
    setPos(x, y);
}

void Collectible::updateFrame() {
    QPixmap currentFrame = animation->currentPixmap();
    setPixmap(makeTransparent(currentFrame));
}

QPixmap Collectible::makeTransparent(const QPixmap& original) {
    QImage image = original.toImage().convertToFormat(QImage::Format_ARGB32);
    
    for(int y = 0; y < image.height(); y++) {
        for(int x = 0; x < image.width(); x++) {
            QColor color = image.pixelColor(x, y);
            if (color.red() > 250 && color.green() > 250 && color.blue() > 250) {
                image.setPixelColor(x, y, QColor(0, 0, 0, 0));
            }
        }
    }
    
    return QPixmap::fromImage(image);
}

Collectible::~Collectible() {
    delete animation;
}

bool Collectible::checkCollision(const QRectF& rect) {
    return boundingRect().translated(pos()).intersects(rect);
}
