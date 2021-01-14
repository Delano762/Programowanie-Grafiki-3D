#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Quad  {
public:
    Quad();
    ~Quad();
    void draw(GLuint u_pvm_buffer_,glm::mat4 &P,glm::mat4 &VM,glm::mat3 &N);

    Quad(const Quad& rhs) = delete;
    Quad &operator=(const Quad& rhs) = delete;

    std::shared_ptr<Quad> quad_;

    void operator=(Quad&& rhs) = delete;
    Quad(Quad&& rhs) = delete;

    GLuint vao_;
    GLuint buffer_[2];

    GLuint diffuse_texture_;
private:

    std::vector<GLushort > indices;
    std::vector<GLfloat> vertices = {

            -1, 0, -1,     0, 0,   0,1,0,
            1, 0, -1,      1, 0,   0,1,0,
            1, 0, 1,       1, 1,   0,1,0,
            -1, 0, 1,      0, 1,   0,1,0
    };

};