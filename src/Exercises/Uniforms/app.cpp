//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    GLuint u_buffer_handle;
    glGenBuffers(1,&u_buffer_handle);

    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle);
    float strength=0.5;
    float light[3]={1.0,1.0,0.1};

    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float),&strength);
    glBufferSubData(GL_UNIFORM_BUFFER,4*sizeof(float),3*sizeof(float),light);

    glBindBuffer(GL_UNIFORM_BUFFER,0);

    glBindBufferBase(GL_UNIFORM_BUFFER,0,u_buffer_handle);


    std::vector<GLushort > indices =
        {
            0,1,2,3,4,5,6,7,8
        };
    GLuint idx_buffer_handl;
    glGenBuffers(1,&idx_buffer_handl);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idx_buffer_handl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, 0.8f, 0.5f, 0.3f,
            0.5f, 0.0f, 0.0f, 0.8f, 0.5f, 0.3f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.5f, 0.3f,
            -0.4f, 0.0f, 0.0f, 0.6f, 0.9f, 0.1f,
            -0.4f, -0.5f, 0.0f, 0.6f, 0.9f, 0.1f,
            0.4f, -0.5f, 0.0f, 0.6f, 0.9f, 0.1f,
            -0.4f, 0.0f, 0.0f, 0.8f, 0.3f, 0.6f,
            0.4f, -0.5f, 0.0f, 0.8f, 0.3f, 0.6f,
            0.4f, 0.0f, 0.0f, 0.8f, 0.3f, 0.6f
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

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    // glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_SHORT,reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}
