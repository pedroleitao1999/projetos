#include "../HeaderFiles/BlinnPhongTerm.h"
#include <math.h>

BlinnPhongTerm::BlinnPhongTerm(float m, Vec3D l, float f) {
	material = m;
	light = l;
	factor = f;
}

Vec3D BlinnPhongTerm::multiplyComponents() {
	Vec3D multiplication = material * light * factor;
	return multiplication;
}