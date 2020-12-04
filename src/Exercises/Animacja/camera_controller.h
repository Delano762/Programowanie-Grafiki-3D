//
// Created by robert on 03.12.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLLER_H
#define INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLLER_H

#endif //INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLLER_H
#include "camera.h"

class CameraController {
public:

    CameraController():camera_(nullptr) {}
    CameraController(Camera* camera):camera_(camera) {}
    void set_camera(Camera *camera) { camera_ = camera; }

    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    void LMB_pressed(float x, float y) {
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    };

    void LMB_released(float x, float y) {
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    };

    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

private:
    Camera *camera_;
    float scale_ = 0.03;
    bool LMB_pressed_=false;
    float x_,y_;
};