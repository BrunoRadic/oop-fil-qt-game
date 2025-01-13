#include "finishline.hpp"

FinishLine::FinishLine(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
    animation = new QMovie(":/finish.gif");
    
    animation->setScaledSize(QSize(200, 200));
    
    hitbox = QRectF(100, 0, 40, 200);
    
    animation->start();
    
    QTimer* timer = new QTimer(animation);
    QEventLoop::connect(timer, &QTimer::timeout, [this]() {
        updateFrame();
    });
    timer->start(50);  // 50ms
    
    setPos(x, y);
}

QRectF FinishLine::getHitbox() const {
    return hitbox.translated(pos());
}

void FinishLine::updateFrame() {
    setPixmap(animation->currentPixmap());
}

FinishLine::~FinishLine() {
    delete animation;
} 
