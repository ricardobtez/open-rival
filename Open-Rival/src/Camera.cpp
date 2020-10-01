#include "pch.h"
#include "Camera.h"

#include "MathUtils.h"

namespace Rival {

    const float Camera::zoomInterval = 0.1f;
    const float Camera::zoomMin = 0.5f;
    const float Camera::zoomMax = 2.0f;

    const float Camera::tileWidthCamera = 2.0f;
    const float Camera::tileHeightCamera = 1.0f;
    const float Camera::bottomEdgePadding = tileHeightCamera / 2.0f;

    Camera::Camera(
            float x,
            float y,
            float width,
            double aspectRatio,
            Scenario& scenario)
        : defaultWidth(width),
          defaultHeight(static_cast<float>(width / aspectRatio)),
          scenario(scenario) {

        centreOnPoint(x, y);
    }

    void Camera::centreOnPoint(float newX, float newY) {

        // Keep within the bounds of the map
        float cameraWidth = getWidth();
        float cameraHeight = getHeight();
        float minX = cameraWidth / 2;
        float minY = cameraHeight / 2;

        // Find the furthest point visible to the camera
        int lastTileIndexX = scenario.getWidth() - 1;
        int lastTileIndexY = scenario.getHeight() - 1;
        float rightEdge = lastTileIndexX + tileWidthCamera;
        float bottomEdge = lastTileIndexY + tileHeightCamera
                + bottomEdgePadding;
        float maxX = rightEdge - (cameraWidth / 2);
        float maxY = bottomEdge - (cameraHeight / 2);

        x = MathUtils::clampf(newX, minX, maxX);
        y = MathUtils::clampf(newY, minY, maxY);
    }

    void Camera::centreOnTile(int tileX, int tileY) {
        float offsetY = 0;
        if (tileX % 2 == 1) {
            // Tile co-ordinates zigzag up and down within a row
            offsetY = (tileHeightCamera / 2.0f);
        }

        centreOnPoint(
                (tileX * tileWidthCamera) + (tileWidthCamera / 2.0f),
                (tileY * tileHeightCamera) + (tileHeightCamera / 2.0f)
                        + offsetY);
    }

    void Camera::translate(float dx, float dy) {
        centreOnPoint(x + dx, y + dy);
    }

    float Camera::getX() const {
        return x;
    }

    float Camera::getY() const {
        return y;
    }

    float Camera::getWidth() const {
        return defaultWidth / zoom;
    }

    float Camera::getHeight() const {
        return defaultHeight / zoom;
    }

    float Camera::getLeft() const {
        return x - getWidth() / 2;
    }

    float Camera::getTop() const {
        return y - getHeight() / 2;
    }

    float Camera::getRight() const {
        return x + getWidth() / 2;
    }

    float Camera::getBottom() const {
        return y + getHeight() / 2;
    }

    float Camera::getZoom() const {
        return zoom;
    }

    void Camera::modZoom(float interval) {
        zoom += interval;
        zoom = MathUtils::clampf(zoom, zoomMin, zoomMax);

        // We call `centreOnPoint` here to perform a bounds check, since the
        // size of the visible region has now changed
        centreOnPoint(x, y);
    }

    bool Camera::contains(float px, float py) const {
        return px > getLeft()
                && px < getRight()
                && py > getTop()
                && py < getBottom();
    }

}  // namespace Rival
