#pragma once
#ifndef Material_H
#define Material_H

#include <iostream>
#include "Vec3D.h"

class Material {
private:
	Vec3D emissive_color;
	Vec3D material_color;
	Vec3D other_materials_color;
	double shininess;

public:
	Material(void);
	Material(Vec3D eC, Vec3D mC, Vec3D oMC, double s);
	Vec3D generateEmissiveMaterial(void) { return emissive_color; };
	Vec3D generateAmbientMaterial(void) { return other_materials_color; };
	Vec3D generateDiffuseMaterial(void) { return material_color; };
	Vec3D generateSpecularMaterial(void) { return material_color; };
	Vec3D getEmissiveColor(void) { return emissive_color; };
	Vec3D getMaterialColor(void) { return material_color; };
	Vec3D getOtherMaterialsColor(void) { return other_materials_color; };
	double getShininess(void) { return shininess; };
	void setEmissiveColor(Vec3D eC) { emissive_color = eC; };
	void setMaterialColor(Vec3D mC) { material_color = mC; };
	void setOtherMaterialsColor(Vec3D oMC) { other_materials_color = oMC; };
	void setShininess(double s) { shininess = s; };
};
#endif // !Material_H#pragma once#pragma once
