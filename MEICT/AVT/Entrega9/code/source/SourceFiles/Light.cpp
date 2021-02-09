#include <GL/glew.h>
#include "../HeaderFiles/Light.h"

Light::Light() {
	position = Vec3D();
	ambientLight = Vec3D();
	diffuseLight = Vec3D();
	specularLight = Vec3D();
	power = 0.0f;
}

Light::Light(Vec3D p, Vec3D a, Vec3D d, Vec3D s, float pow) {
	position = p;
	ambientLight = a;
	diffuseLight = d;
	specularLight = s;
	power = pow;
}