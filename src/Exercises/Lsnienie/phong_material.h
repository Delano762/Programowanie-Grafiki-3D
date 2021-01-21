//
// Created by robert on 14.01.2021.
//

#ifndef INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H
#define INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H

#endif //INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H

#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

struct PhongMaterial {
    alignas(16) glm::vec3 Kd;
    alignas(4) uint32_t Kd_map;
    alignas(16) glm::vec3 Ks;
    alignas(4) uint32_t Ks_map;
    alignas(4) float Ns;
    alignas(4) uint32_t Ns_map;
};