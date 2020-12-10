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

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    std::vector<GLushort > indices(18);

    for(int i=0;i<18;i++)
        indices[i]=i;

    GLuint idx_buffer_handl;
    glGenBuffers(1,&idx_buffer_handl);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idx_buffer_handl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    std::vector<GLfloat> vertices = {
            0, 1, 0, 0.3f, 0.5f, 0.1f,
            0, 0, 0, 0.3f, 0.5f, 0.1f,
            1, 0, 0, 0.3f, 0.5f, 0.1f,

            0, 1, 0, 0.8f, 0.5f, 0.3f,
            1, 0, 0, 0.8f, 0.5f, 0.3f,
            1, 1, 0, 0.8f, 0.5f, 0.3f,

            0, 1, 0, 0.0f, 0.5f, 0.3f,
            0, 0, 0, 0.0f, 0.5f, 0.3f,
            0.5, 0.5, 1, 0.0f, 0.5f, 0.3f,

            1, 1, 0, 0.8f, 0.0f, 0.3f,
            0, 1, 0, 0.8f, 0.0f, 0.3f,
            0.5, 0.5, 1, 0.8f, 0.0f, 0.3f,

            1, 0, 0, 0.8f, 0.5f, 0.1f,
            1, 1, 0, 0.8f, 0.5f, 0.1f,
            0.5, 0.5, 1, 0.8f, 0.5f, 0.1f,

            0, 0, 0, 0.5f, 0.5f, 1.0f,
            1, 0, 0, 0.5f, 0.5f, 1.0f,
            0.5, 0.5, 1, 0.5f, 0.5f, 1.0f,

    };


    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    glEnableVertexAttribArray(0);
    // Define an array of generic vertex attribute data (index, size, type, normalized, stride, pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handl);
    glBindVertexArray(0);

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

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    set_camera(new Camera);

    set_controller(new CameraController(camera()));

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(1.0f, 1.0f, 3.0f),
                     glm::vec3(0.5f, 0.5f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));

    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4), nullptr,GL_STATIC_DRAW);

    //glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),&P_[0]);
    //glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),&V_[0]);

    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,u_pvm_buffer_);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
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

    auto PVM = camera_->projection()*camera_->view();
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_pvm_buffer_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,reinterpret_cast<GLvoid*>(0));
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