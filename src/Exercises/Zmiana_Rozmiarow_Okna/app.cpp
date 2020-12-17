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

            //-0.4f, -0.5f, 0.0f,  0.8f, 0.5f, 0.3f,
            //0.4f, -0.5f, 0.0f,  0.8f, 0.5f, 0.3f,

            //-0.4f, 0.0f, 0.0f,  0.8f, 0.5f, 0.3f,
            //-0.4f, -0.5f, 0.0f,  0.8f, 0.5f, 0.3f,
            //0.4f, -0.5f, 0.0f,  0.8f, 0.5f, 0.3f,
            //-0.4f, 0.0f, 0.0f,  0.8f, 0.5f, 0.3f,
            //0.4f, -0.5f, 0.0f,  0.8f, 0.5f, 0.3f,
            //0.4f, 0.0f, 0.0f,  0.8f, 0.5f, 0.3f,
    };


    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    aspect_ = (float)w/h;
    fov_ = glm::pi<float>()/4.0;
    near_ = 0.1f;
    far_ = 100.0f;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
    V_ = glm::lookAt(glm::vec3{0,1.1,3.0},glm::vec3{0.0,0.0,1.0},glm::vec3{1.0,1.0,1.0});
    glm::mat4 M(1.0f);

    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4), nullptr,GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),&P_[0]);
    glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),&V_[0]);

    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,u_pvm_buffer_);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}

void SimpleShapeApplication::frame() {

    auto PVM = P_ * V_;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindVertexArray(vao_);
    // glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}
