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

#include "3rdParty/src/stb/stb_image.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }


    xe::utils::set_uniform_block_binding(program,"Transformations",0);
    xe::utils::set_uniform_block_binding(program,"Material",1);
    xe::utils::set_uniform_block_binding(program,"Light",2);

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,0,u_pvm_buffer_);

    glGenBuffers(1, &u_material_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_material_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(PhongMaterial), nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,u_material_buffer_);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(Light), nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER,2,u_light_buffer_);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    position=glm::vec4(0.0f,1.0f,0.0f,1.0f);
    light.position=position;
    light.a = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    light.color=glm::vec4 (1.0f,1.0f,1.0f,1.0f);
    light.ambient=glm::vec3 (1.0f,1.0f,1.0f);

    GLuint diffuse_texture;
    glGenTextures(1,&diffuse_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/Textures/plastic.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint shininess_texture;
    glGenTextures(1,&shininess_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shininess_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/Textures/shininess.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    PhongMaterial *material=new PhongMaterial();

    material->Kd = glm::vec3(1.0, 0.0, 0.0);
    material->Kd_map = 0;
    material->Ks = glm::vec3(0.05, 0.05, 0.05);
    material->Ks_map = 0;
    material->Ns = 1000.0f;
    material->Ns_map = 0;

    material->Kd = glm::vec3(1.0, 1.0, 1.0);
    material->Kd_map = diffuse_texture;

    material->Ns = 1000.0f;
    material->Ns_map = shininess_texture;

    quad = new Quad();

    quad->set_material(material);

    quad->quad_= std::make_shared<Quad>();

    set_camera(new Camera);

    set_controller(new CameraController(camera()));

    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 4.0f, 0.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 0.0f, 1.0f));

    M=glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    xe::utils::set_uniform1i(program,"diffuse_map",0);
    xe::utils::set_uniform1i(program,"shininess_map",1);
    xe::utils::set_uniform1i(program,"specular_map",2);

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

    P = camera_->projection();
    VM = camera()->view()*M;
    auto R = glm::mat3(VM);
    N = glm::transpose(glm::inverse(R));
    //light.position=VM*position;
    light.position_in_vs = camera()->view() * light.position;

    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(glm::vec4), &light.position_in_vs[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER,u_material_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PhongMaterial), quad->material_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    auto material = quad->material_;
    if(material->Kd_map>0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->Kd_map);
    }
    if(material->Ns_map>0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->Ns_map);
    }
    if(material->Ks_map>0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->Ks_map);
    }

    quad->draw(u_pvm_buffer_,P,VM,N);
    glBindTexture(GL_TEXTURE_2D, 0);
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