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
#include "quad.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 0);
    }

    auto u_light_index = glGetUniformBlockIndex(program, "Light");
    if (u_light_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Light uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_light_index, 2);
    }

    quad = new Quad();

    quad->quad_= std::make_shared<Quad>();

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    position=glm::vec4(0.0f,1.0f,0.0f,1.0f);
    light.position=position;

    light.a = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
    light.color=glm::vec4 (1.0f,1.0f,1.0f,1.0f);
    light.ambient=glm::vec3 (0.2f,0.2f,0.2f);

    set_camera(new Camera);

    set_controller(new CameraController(camera()));

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 4.0f, 0.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 0.0f, 1.0f));

    M=glm::mat4(1.0f);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,3*sizeof(glm::vec4) + sizeof(glm::vec3), nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,2,u_light_buffer_);

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,0,u_pvm_buffer_);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    auto  u_diffuse_map_location = glGetUniformLocation(program,"diffuse_map");
    if(u_diffuse_map_location==-1) {
        std::cerr<<"Cannot find uniform diffuse_map\n";
    } else {
        glUniform1ui(u_diffuse_map_location,0);
    }
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

    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light.position);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &light.color);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &light.a);
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec3), &light.ambient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    P = camera_->projection();
    VM = camera()->view()*M;
    auto R = glm::mat3(VM);
    N = glm::transpose(glm::inverse(R));
    light.position=VM*position;

    quad->draw(u_pvm_buffer_,P,VM,N);
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

void SimpleShapeApplication::cleanup() {
    delete quad;
}