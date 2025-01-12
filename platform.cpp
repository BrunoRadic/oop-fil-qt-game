#include "platform.hpp"

Platform::Platform(qreal x, qreal y, qreal width, qreal height, Type type, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , m_width(width)
    , m_height(height)
    , platformType(type)
    , startY(y)
    , endY(y - MOVE_DISTANCE)  // move up from starting position
    , moveSpeed(MOVE_SPEED)
    , movingUp(true)
{

    QPixmap platformTexture;
    

    platformTexture.load(":/pillar_3.png");
    platformTexture = platformTexture.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    if (platformTexture.isNull()) {
        // fallback
        platformTexture = QPixmap(width, height);
        platformTexture.fill(Qt::gray);
    }
    
    setPixmap(platformTexture);
    setPos(x, y);
}

void Platform::updateMovement() {
    if (platformType != MovingVertical) return;

    qreal currentY = pos().y();
    
    // change direction
    if (currentY <= endY) {
        movingUp = false;
    } else if (currentY >= startY) {
        movingUp = true;
    }

   // move platform
   setPos(pos().x(), currentY + (movingUp ? -moveSpeed : moveSpeed));
}

bool Platform::checkCollision(const QRectF& rect, double& velocityX, double& velocityY) {
    QRectF platformRect(pos().x(), pos().y(), m_width, m_height);
    
    if (rect.intersects(platformRect)) {
        double fromTop = rect.bottom() - platformRect.top();
        double fromBottom = platformRect.bottom() - rect.top();
        double fromLeft = rect.right() - platformRect.left();
        double fromRight = platformRect.right() - rect.left();

        // smallest overlap
        double minOverlap = std::min({fromTop, fromBottom, fromLeft, fromRight});

        // landing
        if (minOverlap == fromTop && velocityY > 0) {
            velocityY = 0;
            return true;  // standing
        }
        // bottom collision
        else if (minOverlap == fromBottom && velocityY < 0) {
            velocityY = 0;
        }
        // left collision
        else if (minOverlap == fromLeft && velocityX > 0) {
            velocityX = 0;
        }
        // right collision
        else if (minOverlap == fromRight && velocityX < 0) {
            velocityX = 0;
        }
    }
    return false;
}

Platform::Type Platform::getType() const {
    return platformType;
}
