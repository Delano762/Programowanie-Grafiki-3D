#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw(GLuint u_pvm_buffer_,glm::mat4 &PVM);

    Pyramid(const Pyramid& rhs) = delete;
    Pyramid &operator=(const Pyramid& rhs) = delete;

    std::shared_ptr<Pyramid> pyramid_;

    void operator=(Pyramid&& rhs) = delete;
    Pyramid(Pyramid&& rhs) = delete;

    GLuint vao_;
    GLuint buffer_[2];
private:

    std::vector<GLushort > indices;
    std::vector<GLfloat> vertices = {
            -0.5, 0.5, 0, 0.3f, 0.5f, 0.1f,
            -0.5, -0.5, 0, 0.3f, 0.5f, 0.1f,
            0.5, -0.5, 0, 0.3f, 0.5f, 0.1f,

            -0.5, 0.5, 0, 0.8f, 0.5f, 0.3f,
            0.5, -0.5, 0, 0.8f, 0.5f, 0.3f,
            0.5, 0.5, 0, 0.8f, 0.5f, 0.3f,

            -0.5, 0.5, 0, 0.0f, 0.5f, 0.3f,
            -0.5, -0.5, 0, 0.0f, 0.5f, 0.3f,
            0, 0, 1, 0.0f, 0.5f, 0.3f,

            0.5, 0.5, 0, 0.8f, 0.0f, 0.3f,
            -0.5, 0.5, 0, 0.8f, 0.0f, 0.3f,
            0, 0, 1, 0.8f, 0.0f, 0.3f,

            0.5, -0.5, 0, 0.8f, 0.5f, 0.1f,
            0.5, 0.5, 0, 0.8f, 0.5f, 0.1f,
            0, 0, 1, 0.8f, 0.5f, 0.1f,

            -0.5, -0.5, 0, 0.5f, 0.5f, 1.0f,
            0.5, -0.5, 0, 0.5f, 0.5f, 1.0f,
            0, 0, 1, 0.5f, 0.5f, 1.0f,

    };
};