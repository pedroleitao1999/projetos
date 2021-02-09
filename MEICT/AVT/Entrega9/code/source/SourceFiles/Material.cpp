#include <GL/glew.h>
#include "../HeaderFiles/Material.h"

Material::Material() {
	emissive_color = Vec3D();
	material_color = Vec3D();
	other_materials_color = Vec3D();
	shininess = 0.0;
}

Material::Material(Vec3D eC, Vec3D mC, Vec3D oMC, double s) {
	emissive_color = eC;
	material_color = mC;
	other_materials_color = oMC;
	shininess = s;
}