#ifndef FINISHLINE_HPP
#define FINISHLINE_HPP

#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QTimer>

class FinishLine : public QGraphicsPixmapItem {
public:
    FinishLine(qreal x, qreal y, QGraphicsItem* parent = nullptr);
    ~FinishLine();

private:
    QMovie* animation;
    void updateFrame();
};

#endif // FINISHLINE_HPP 