//
// Created by Nuno Laranjo on 30/10/2020.
//

#ifndef AVT_RENDERER_H
#define AVT_RENDERER_H

#include <vector>
#include "Vec3D.h"
#include "Vec4D.h"
#include "Matrix4D.h"
#include "BlinnPhongTerm.h"

class Renderer {
private:
    GLuint VaoId;
    GLuint VboId[3];
    GLint UniformId;
public:
    Renderer(GLuint VaoId, GLuint *VboId, GLint UniformId);
    void drawTriangles(std::vector<Vec4D> vertices, std::vector<Vec4D> colors, std::vector<int> indices, std::vector<Vec4D> normals, Matrix4D uniform,
        BlinnPhongTerm ambient, BlinnPhongTerm diffuse, BlinnPhongTerm specular, Vec3D lightPosition, Vec3D eye, double shininess);
};


#endif //AVT_RENDERER_H
