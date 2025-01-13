#ifndef FINISHLINE_HPP
#define FINISHLINE_HPP

#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QTimer>

class FinishLine : public QGraphicsPixmapItem {
public:
    FinishLine(qreal x, qreal y, QGraphicsItem* parent = nullptr);
    ~FinishLine();

    QRectF getHitbox() const;

private:
    QMovie* animation;
    void updateFrame();
    QRectF hitbox;
};

#endif // FINISHLINE_HPP 