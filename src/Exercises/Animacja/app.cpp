//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Application/utils.h"

#include <memory>
#include <chrono>
#include "time.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    glGenBuffers(1, &u_pvm_buffer_);

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    pyramid = new Pyramid();

    pyramid->pyramid_= std::make_shared<Pyramid>();



    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    set_camera(new Camera);

    set_controller(new CameraController(camera()));

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 0.0f, 40.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 0.5f, 0.0f));

    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4), nullptr,GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,u_pvm_buffer_);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    start_ = std::chrono::steady_clock::now();
    rotation_period=4.0;

}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
}

void SimpleShapeApplication::frame() {

    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    auto rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/rotation_period;
    auto PVM = camera_->projection()*camera_->view();


    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/orbital_rotation_period;
    auto R=glm::rotate(glm::mat4(1.0f), rotation_angle,glm::vec3{0.0f, 0.0f, 1.0f});
    auto x = 10*cos(orbital_rotation_angle);
    auto y = 8*sin(orbital_rotation_angle);
    auto O = glm::translate(glm::mat4(1.0f), glm::vec3{x,y,0.0});
    auto M = camera()->projection()*camera()->view()*O*R;

    pyramid->draw(u_pvm_buffer_,M);

    auto moon = glm::scale(glm::mat4(1.0f), glm::vec3{0.5f, 0.5f, 0.5f});
    auto moon_orbital_rotation_period = 10.0f;
    auto moon_orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/moon_orbital_rotation_period;
    auto moon_R=glm::rotate(glm::mat4(1.0f),moon_orbital_rotation_angle,glm::vec3(0.0f,0.0f,1.0f));
    auto moon_r=3.0f;
    auto moon_x = moon_r*cos(moon_orbital_rotation_angle);
    auto moon_y = moon_r*sin(moon_orbital_rotation_angle);
    auto moon_O = glm::translate(glm::mat4(1.0f), glm::vec3{moon_x,moon_y,0.0});
    auto moon_M = camera()->projection()*camera()->view()*O*moon_O*moon_R*moon;

    pyramid->draw(u_pvm_buffer_,moon_M);

    auto satellite = glm::scale(glm::mat4(1.0f), glm::vec3{0.25f, 0.25f, 0.25f});
    auto satellite_orbital_rotation_period = 2.0f;
    auto satellite_orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satellite_orbital_rotation_period;
    auto satellite_R=glm::rotate(glm::mat4(1.0f),satellite_orbital_rotation_angle,glm::vec3(0.0f,1.0f,0.0f));
    auto satellite_r=1.5f;
    auto satellite_x = satellite_r*cos(satellite_orbital_rotation_angle);
    auto satellite_z = satellite_r*sin(satellite_orbital_rotation_angle);
    auto satellite_O = glm::translate(glm::mat4(1.0f), glm::vec3{satellite_x,0.0,satellite_z});
    auto satellite_M = camera()->projection()*camera()->view()*O*satellite_O*satellite_R*satellite;

    pyramid->draw(u_pvm_buffer_,satellite_M);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controller_) {
        controller_->mouse_moved(x, y);
    }
}