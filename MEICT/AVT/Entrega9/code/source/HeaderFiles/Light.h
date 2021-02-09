#pragma once
#ifndef Light_H
#define Light_H

#include <iostream>
#include "Vec3D.h"

class Light {
private:
	Vec3D position;
	Vec3D ambientLight, diffuseLight, specularLight;
	float power;

public:
	Light(void);
	Light(Vec3D p, Vec3D a, Vec3D d, Vec3D s, float pow);
	Vec3D getPosition(void) { return position; };
	Vec3D getAmbientLight(void) { return ambientLight; };
	Vec3D getDiffuseLight(void) { return diffuseLight; };
	Vec3D getSpecularLight(void) { return specularLight; };	
	float getPower(void) { return power; };
};
#endif // !Light_H#pragma once
