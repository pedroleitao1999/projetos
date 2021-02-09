#pragma once
#ifndef BlinnPhongTerm_H
#define BlinnPhongTerm_H

#include <iostream>
#include "Vec3D.h"

class BlinnPhongTerm {
private:
	float material;
	Vec3D light;
	float factor;

public:
	BlinnPhongTerm(float m, Vec3D l, float f);
	void setMaterial(float m) { material = m; };
	void setLight(Vec3D l) { light = l; };
	void setFactor(float f) { factor = f; };
	float getMaterial(void) { return material; };
	Vec3D getLight(void) { return light; };
	float getFactor(void) { return factor; };
	Vec3D multiplyComponents(void);
};
#endif // !BlinnPhongTerm_H#pragma once
