//
// Created by robert on 28.12.2020.
//

#include "pyramid.h"
#include <vector>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"


Pyramid::Pyramid() {
    /*
        Tu dodajemy kod tworzący bufory indeksów i wierzchołków oraz obiekt VAO,
        Potem ładujemy dane wierzchołków i indeksow do buforów  i  podłaczamy je do atrybutów w szaderze
   */
    std::vector<GLushort > indices(18);

    indices={
            0, 1, 2,
            0, 2, 3,
            1, 0, 6,
            0, 3, 7,
            3, 2, 4,
            2, 1, 5
    };

    GLuint idx_buffer_handl;
    glGenBuffers(1,&idx_buffer_handl);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,idx_buffer_handl);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    glEnableVertexAttribArray(0);
    // Define an array of generic vertex attribute data (index, size, type, normalized, stride, pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handl);
    glBindVertexArray(0);

    int width, height, n_channels;
    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";
    uint8_t  *data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);

    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE,data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Pyramid::~Pyramid() {
    //Tu usuwamy VAO i bufory
}

void Pyramid::draw(GLuint u_pvm_buffer_,glm::mat4 &PVM) {
    //Tu wywolujemy polecenie glDrawElements

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffuse_texture_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT, nullptr);
    // glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_pvm_buffer_);


}