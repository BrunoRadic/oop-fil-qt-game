#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QTimer>

class Collectible : public QGraphicsPixmapItem {
public:
    Collectible(qreal x, qreal y, QGraphicsItem* parent = nullptr);
    ~Collectible();

    bool checkCollision(const QRectF& rect);
    int getValue() const { return m_value; }

private:
    QMovie* animation;
    int m_value;
    void updateFrame();
    QPixmap makeTransparent(const QPixmap& original);
};

#endif // COLLECTIBLE_HPP
