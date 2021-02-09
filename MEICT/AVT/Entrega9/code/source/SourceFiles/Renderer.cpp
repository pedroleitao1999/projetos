//
// Created by Nuno Laranjo on 30/10/2020.
//

#include <GL/glew.h>
#include "../HeaderFiles/Renderer.h"

typedef struct {
    GLfloat position[4];
    GLfloat normal[4];
    GLfloat light_position[4];
    GLfloat light_ambient[4];
    GLfloat light_diffuse[4];
    GLfloat light_specular[4];
    GLfloat material_emissive[4];
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
    GLfloat material_shininess;
    GLfloat painted_color[4];
    GLfloat texcoord[2];
    GLfloat light_power;
} Info;

Renderer::Renderer(GLuint VaoId, GLuint *VboId, GLint UniformId) {
    this->VaoId = VaoId;
    std::copy(VboId, VboId+3, this->VboId);
    this->UniformId = UniformId;
}

void Renderer::drawTriangles(std::vector<Vec4D> vertices, std::vector<int> indices, Matrix4D uniform, Shader shader) {
    
    vector<Vec4D> normals = shader.getNormals();
    vector<Texcoord> texcoords = shader.getTexCoords();

    vector<GLubyte> GLindices;
    vector<Info> GLinfo;

    // Convert vector<Info> to GLinfo
    GLinfo.reserve(vertices.size());

    for (int i = 0; i < vertices.size(); i++) {

        Info info{
                {vertices[i].getX(), vertices[i].getY(), vertices[i].getZ(), vertices[i].getW()},
                {normals[i].getX(), normals[i].getY(), normals[i].getZ(), normals[i].getW()},
                {shader.getLight().getPosition().getX(), shader.getLight().getPosition().getY(), shader.getLight().getPosition().getZ(), 1.0f},
                {shader.getLight().getAmbientLight().getX(), shader.getLight().getAmbientLight().getY(), shader.getLight().getAmbientLight().getZ(), 1.0f},
                {shader.getLight().getDiffuseLight().getX(), shader.getLight().getDiffuseLight().getY(), shader.getLight().getDiffuseLight().getZ(), 1.0f},
                {shader.getLight().getSpecularLight().getX(), shader.getLight().getSpecularLight().getY(), shader.getLight().getSpecularLight().getZ(), 1.0f},
                {shader.getMaterial().generateEmissiveMaterial().getX(), shader.getMaterial().generateEmissiveMaterial().getY(),
                shader.getMaterial().generateEmissiveMaterial().getZ(), 1.0f},
                {shader.getMaterial().generateAmbientMaterial().getX(), shader.getMaterial().generateAmbientMaterial().getY(),
                shader.getMaterial().generateAmbientMaterial().getZ(), 1.0f},
                {shader.getMaterial().generateDiffuseMaterial().getX(), shader.getMaterial().generateDiffuseMaterial().getY(),
                shader.getMaterial().generateDiffuseMaterial().getZ(), 1.0f},
                {shader.getMaterial().generateSpecularMaterial().getX(), shader.getMaterial().generateSpecularMaterial().getY(),
                shader.getMaterial().generateSpecularMaterial().getZ(), 1.0f},
                shader.getShininess(),
                {shader.getPaintedColor().getX(), shader.getPaintedColor().getY(), shader.getPaintedColor().getZ(), 1.0f},
                {texcoords[i].getU(), texcoords[i].getV()},
                shader.getLightPower()
        };
        GLinfo.push_back(info);
    }

    GLindices.reserve(indices.size());
    for (int index : indices) {
        GLindices.push_back((GLubyte) index);
    }

    glBindVertexArray(VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, GLinfo.size() * sizeof(Info), &GLinfo.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)(GLindices.size() * sizeof(GLubyte)), &GLindices.front(), GL_STATIC_DRAW);

    glUniformMatrix4fv((GLint) UniformId, (GLsizei) 1, GL_FALSE, uniform.toGLfloats());

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) ( GLindices.size() * sizeof(GLubyte) ) );
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
