#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <stdio.h>
using namespace std;

#include "vector.h"
#include "ray.h"


#define PI				3.141592653589793238462f

class Camera
{

private:
	Vector eye, at, up; 
	float fovy, vnear, vfar, plane_dist, focal_ratio, aperture;
	float w, h;
	int res_x, res_y;
	Vector u, v, n;

public:
	Vector GetEye() { return eye; }
	int GetResX()  { return res_x; }
    int GetResY()  { return res_y; }
	float GetFov() { return fovy; }
	float GetPlaneDist() { return plane_dist; }
	float GetFar() {return vfar; }
	float GetAperture() { return aperture; }

    Camera( Vector from, Vector At, Vector Up, float angle, float hither, float yon, int ResX, int ResY, float Aperture_ratio, float Focal_ratio) {
	    eye = from;
	    at = At;
	    up = Up;
	    fovy = angle;
	    vnear = hither;
	    vfar = yon;
	    res_x = ResX;
	    res_y = ResY;
		focal_ratio = Focal_ratio;

        // set the camera frame uvn
        n = ( eye - at );
        plane_dist = n.length();
	    n = n / plane_dist;

	    u = up % n;
	    u = u / u.length();

	    v = n % u;

        //Dimensions of the vis window
	    h = 2 * plane_dist * tan( (PI * angle / 180) / 2.0f );
        w = ( (float) res_x / res_y ) * h;  

		aperture = Aperture_ratio * (w / res_x); //Lens aperture = aperture_ratio * pixel_size

		printf("\nwidth=%f height=%f fov=%f, viewplane distance=%f, pixel size=%.3f\n", w,h, fovy,plane_dist, w/res_x);
		if (Aperture_ratio != 0) printf("\nDepth-Of-Field effect enabled with a lens aperture = %.1f\n", Aperture_ratio);
    }

	void SetEye(Vector from) {
		eye = from;
		// set the camera frame uvn
		n = (eye - at);
		plane_dist = n.length();
		n = n / plane_dist;
		u = up % n;
		u = u / u.length();
		v = n % u;
	}

	Ray PrimaryRay(Vector pixel_sample) //  Rays cast from the Eye to a pixel sample which is in Viewport coordinates
	{
		float df = (eye - at).length();
		Vector Ze =  n/df;
		Vector Xe = (up%Ze) / (up % Ze).length();
		Vector Ye = Ze % Xe;
		Vector d =  ((Xe.normalize() * (float)(((pixel_sample.x + 0.5) / res_x) - 0.5))* w + (Ye.normalize() * (float)(((pixel_sample.y + 0.5) / res_y) - 0.5)) * h);
		d = d - (Ze.normalize()* df);
		return Ray(eye, d.normalize());
	}

	Ray PrimaryRay(const Vector& lens_sample, const Vector& pixel_sample) // DOF: Rays cast from  a thin lens sample to a pixel sample
	{
		float df = (eye - at).length();
		Vector Ze = n / df;
		Vector Xe = (up % Ze) / (up % Ze).length();
		Vector Ye = Ze % Xe;

		Vector ray_dir;
		Vector eye_offset;
		Vector ls; // sample point on lens (where pimary ray is shot from)	
		Vector p; // point where center ray hits focal plane
		Vector ps; // sample point on pixel
		
		float f = focal_ratio * plane_dist; // focal length of camera ( distance along the view direction)

		ps.x = (((pixel_sample.x ) / res_x) - (float)0.5) * w;
		ps.y = (((pixel_sample.y ) / res_y) - (float)0.5) * h;
		ps.z = -plane_dist;

		ls.x = lens_sample.x * aperture;
		ls.y = lens_sample.y * aperture;
		ls.z = 0;

		p.x = ps.x * focal_ratio; // assuming focal_ratio = f / d
		p.y = ps.y * focal_ratio;
		p.z = -f;

		ray_dir = (Xe.normalize() * (p.x - ls.x) + Ye.normalize() * (p.y - ls.y) - Ze.normalize() * f).normalize();

		eye_offset = eye + Xe * ls.x + Ye * ls.y;

		return Ray(eye_offset, ray_dir);
	}
};

#endif