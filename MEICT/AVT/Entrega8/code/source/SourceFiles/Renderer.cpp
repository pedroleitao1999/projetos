//
// Created by Nuno Laranjo on 30/10/2020.
//

#include <GL/glew.h>
#include "../HeaderFiles/Renderer.h"

typedef struct {
    GLfloat XYZW[4];
    GLfloat RGBA[4];
} Vertex;

Renderer::Renderer(GLuint VaoId, GLuint *VboId, GLint UniformId) {
    this->VaoId = VaoId;
    std::copy(VboId, VboId+3, this->VboId);
    this->UniformId = UniformId;
}

void Renderer::drawTriangles(std::vector<Vec4D> vertices, std::vector<Vec4D> colors, std::vector<int> indices, std::vector<Vec4D> normals, Matrix4D uniform, 
    BlinnPhongTerm ambient, BlinnPhongTerm diffuse, BlinnPhongTerm specular, Vec3D lightPosition, Vec3D eye, double shininess) {

    if (vertices.size() != colors.size()) {
        // Throw exception
    }

    vector<GLubyte> GLindices;
    vector<Vertex> GLvertices;

    // Convert vector<Vec4D> to GLvertices
    GLvertices.reserve(vertices.size());
    for (int i = 0; i < vertices.size(); i++) {

        // Calculate normal vector
        Vec3D normal = Vec3D(normals[i].getX(), normals[i].getY(), normals[i].getZ());

        // Calculate diffuse factor for each vertex
        Vec3D vertice3D = Vec3D(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
        Vec3D lightDirection = lightPosition - vertice3D;
        double diffuseDotProduct = normal.dot(lightDirection);
        double diffuseFactor = max(diffuseDotProduct, 0.0);
        diffuse.setFactor((GLfloat)diffuseFactor);

        // Calculate specular factor for each vertex
        Vec3D specularSum = lightDirection + eye;
        Vec3D halfVector = specularSum.normalize();
        double specularDotProduct = halfVector.dot(normal);
        double specularFactor = pow(max(specularDotProduct, 0.0), shininess);
        specular.setFactor((GLfloat)specularFactor);

        // Calculate color intensity
        Vec3D intensity = ambient.multiplyComponents() + diffuse.multiplyComponents() + specular.multiplyComponents();
        
        Vertex vertex {
                {vertices[i].getX(), vertices[i].getY(), vertices[i].getZ(), vertices[i].getW()},
                {intensity.getX() * colors[i].getX(), intensity.getY() * colors[i].getY(), intensity.getZ() * colors[i].getZ(), colors[i].getW()}
        };
        GLvertices.push_back(vertex);
    }

    GLindices.reserve(indices.size());
    for (int index : indices) {
        GLindices.push_back((GLubyte) index);
    }
    
    glBindVertexArray(VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, GLvertices.size() * sizeof(Vertex), &GLvertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei) ( 2 * GLindices.size() * sizeof(GLubyte) ), &GLindices.front(), GL_STATIC_DRAW);
    
    glUniformMatrix4fv(UniformId, 1, GL_FALSE, uniform.toGLfloats());

    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) ( 2 * GLindices.size() * sizeof(GLubyte) ), GL_UNSIGNED_BYTE, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
