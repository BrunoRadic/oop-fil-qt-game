#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <QGraphicsPixmapItem>

class Platform : public QGraphicsPixmapItem {
public:
    enum Type { Normal, MovingVertical};
    
    Platform(qreal x, qreal y, qreal width, qreal height,
            Type type = Normal, QGraphicsItem* parent = nullptr);

    Type getType() const;
    bool checkCollision(const QRectF& rect, double& velocityX, double& velocityY);
    void updateMovement();
    double getVerticalVelocity() const { return verticalVelocity; }

private:
    qreal m_width;
    qreal m_height;
    Type platformType;
    
    // moving platforms
    qreal startY;
    qreal endY;
    qreal moveSpeed;
    bool movingUp;
    double verticalVelocity;
    static constexpr qreal MOVE_DISTANCE = 200.0;  // up/down
    static constexpr qreal MOVE_SPEED = 2.0;      // speed
};

#endif // PLATFORM_HPP 
