#include "finishline.hpp"

FinishLine::FinishLine(qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{
    animation = new QMovie(":/finish.gif");
    
    animation->setScaledSize(QSize(100, 200));
    
    animation->start();
    
    // update frame
    QTimer* timer = new QTimer(animation);
    QEventLoop::connect(timer, &QTimer::timeout, [this]() {
        updateFrame();
    });
    timer->start(50);  // 50ms
    
    setPos(x, y);
}

void FinishLine::updateFrame() {
    setPixmap(animation->currentPixmap());
}

FinishLine::~FinishLine() {
    delete animation;
} 
