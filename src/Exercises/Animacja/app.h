//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"
#include "camera.h"
#include "camera_controller.h"
#include "pyramid.h"
#include <chrono>

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;

    void frame() override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    Camera *camera_;

    CameraController *controller_;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void set_controller(CameraController *controller) { controller_ = controller; }

    void framebuffer_resize_callback(int w, int h) override;
    void scroll_callback(double xoffset, double yoffset) override;

    Pyramid* pyramid;

    std::chrono::steady_clock::time_point start_;

    float rotation_period;

    glm::mat4 PVM;

private:
    GLuint vao_;
    GLuint u_pvm_buffer_;
};