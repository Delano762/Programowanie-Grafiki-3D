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
#include "quad.h"
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
    void cleanup() override;

    Quad* quad;

    glm::mat4 M;
    glm::mat4 P;
    glm::mat4 VM;
    glm::mat3 N;

    glm::vec4 position;
private:
    GLuint vao_;
    GLuint u_pvm_buffer_;
    GLuint u_light_buffer_;

    struct Light {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec4 a;
        glm::vec3 ambient;
    };
    Light light;
};