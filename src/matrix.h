//
// Created by Administrator on 2024/10/8.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <CMatrix.h>
struct D3DXVECTOR3;

void CalcWorldCoors ( D3DXVECTOR3 *vecScreen, D3DXVECTOR3 *vecWorld );
void CalcScreenCoors ( D3DXVECTOR3 *vecWorld, D3DXVECTOR3 *vecScreen );

inline glm::mat4 ConvertGtaMatrixToMat(const CMatrix* gta_matrix) {
    return glm::mat4x3(
        glm::vec3(gta_matrix->right.x, gta_matrix->right.y, gta_matrix->right.z),
        glm::vec3(gta_matrix->up.x, gta_matrix->up.y, gta_matrix->up.z),
        glm::vec3(gta_matrix->at.x, gta_matrix->at.y, gta_matrix->at.z),
        glm::vec3(gta_matrix->pos.x, gta_matrix->pos.y, gta_matrix->pos.z)
    );
}

#endif //MATRIX_H
