/**
 * common.glsl
 * Common types and functions used for ray tracing.
 */

const float pi = 3.14159265358979;
const float epsilon = 0.001;

struct Ray {
    vec3 o;     // origin
    vec3 d;     // direction - always set with normalized vector
    float t;    // time, for motion blur
};

Ray createRay(vec3 o, vec3 d, float t)
{
    Ray r;
    r.o = o;
    r.d = d;
    r.t = t;
    return r;
}

Ray createRay(vec3 o, vec3 d)
{
    return createRay(o, d, 0.0);
}

vec3 pointOnRay(Ray r, float t)
{
    return r.o + r.d * t;
}

float gSeed = 0.0;

uint baseHash(uvec2 p)
{
    p = 1103515245U * ((p >> 1U) ^ (p.yx));
    uint h32 = 1103515245U * ((p.x) ^ (p.y>>3U));
    return h32 ^ (h32 >> 16);
}

float hash1(inout float seed) {
    uint n = baseHash(floatBitsToUint(vec2(seed += 0.1,seed += 0.1)));
    return float(n) / float(0xffffffffU);
}

vec2 hash2(inout float seed) {
    uint n = baseHash(floatBitsToUint(vec2(seed += 0.1,seed += 0.1)));
    uvec2 rz = uvec2(n, n * 48271U);
    return vec2(rz.xy & uvec2(0x7fffffffU)) / float(0x7fffffff);
}

vec3 hash3(inout float seed)
{
    uint n = baseHash(floatBitsToUint(vec2(seed += 0.1, seed += 0.1)));
    uvec3 rz = uvec3(n, n * 16807U, n * 48271U);
    return vec3(rz & uvec3(0x7fffffffU)) / float(0x7fffffff);
}

float rand(vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 toLinear(vec3 c)
{
    return pow(c, vec3(2.2));
}

vec3 toGamma(vec3 c)
{
    return pow(c, vec3(1.0 / 2.2));
}

vec2 randomInUnitDisk(inout float seed) {
    vec2 h = hash2(seed) * vec2(1.0, 6.28318530718);
    float phi = h.y;
    float r = sqrt(h.x);
	return r * vec2(sin(phi), cos(phi));
}

vec3 randomInUnitSphere(inout float seed)
{
    vec3 h = hash3(seed) * vec3(2.0, 6.28318530718, 1.0) - vec3(1.0, 0.0, 0.0);
    float phi = h.y;
    float r = pow(h.z, 1.0/3.0);
	return r * vec3(sqrt(1.0 - h.x * h.x) * vec2(sin(phi), cos(phi)), h.x);
}

struct Camera
{
    vec3 eye;
    vec3 u, v, n;
    float width, height;
    float lensRadius;
    float planeDist, focusDist;
    float time0, time1;
};

Camera createCamera(
    vec3 eye,
    vec3 at,
    vec3 worldUp,
    float fovy,
    float aspect,
    float aperture,  //diametro em multiplos do pixel size
    float focusDist,  //focal ratio
    float time0,
    float time1)
{
    Camera cam;
    if(aperture == 0.0) cam.focusDist = 1.0; //pinhole camera then focus in on vis plane
    else cam.focusDist = focusDist;
    vec3 w = eye - at;
    cam.planeDist = length(w);
    cam.height = 2.0 * cam.planeDist * tan(fovy * pi / 180.0 * 0.5);
    cam.width = aspect * cam.height;

    cam.lensRadius = aperture * 0.5 * cam.width / iResolution.x;  //aperture ratio * pixel size; (1 pixel=lente raio 0.5)
    cam.eye = eye;
    cam.n = normalize(w);
    cam.u = normalize(cross(worldUp, cam.n));
    cam.v = cross(cam.n, cam.u);
    cam.time0 = time0;
    cam.time1 = time1;
    return cam;
}

Ray getRay(Camera cam, vec2 pixel_sample)  //rnd pixel_sample viewport coordinates
{
    vec2 ls = cam.lensRadius * randomInUnitDisk(gSeed);  //ls - lens sample for DOF
    float time = cam.time0 + hash1(gSeed) * (cam.time1 - cam.time0);
    
    //Calculate eye_offset and ray direction
    vec3 eye_offset = cam.eye + (cam.u * ls.x) + (cam.v * ls.y);

    vec3 ps;
    ps.x = (pixel_sample.x / iResolution.x - 0.5 ) * cam.width; 
    ps.y = (pixel_sample.y / iResolution.y - 0.5) * cam.height; 

    vec2 p;
    p.x = ps.x * cam.focusDist;
    p.y = ps.y * cam.focusDist;

    vec3 rayDir = cam.u *(p.x - ls.x) + cam.v *(p.y - ls.y) - cam.n * (cam.focusDist * cam.planeDist);

    return createRay(eye_offset, normalize(rayDir), time);
}

// MT_ material type
#define MT_DIFFUSE 0
#define MT_METAL 1
#define MT_DIALECTRIC 2

struct Material
{
    int type;
    vec3 albedo;
    float roughness; // controls roughness for metals
    float refIdx; // index of refraction for dialectric
};

Material createDiffuseMaterial(vec3 albedo)
{
    Material m;
    m.type = MT_DIFFUSE;
    m.albedo = albedo;
    return m;
}

Material createMetalMaterial(vec3 albedo, float roughness)
{
    Material m;
    m.type = MT_METAL;
    m.albedo = albedo;
    m.roughness = roughness;
    return m;
}

Material createDialectricMaterial(vec3 albedo, float refIdx)
{
    Material m;
    m.type = MT_DIALECTRIC;
    m.albedo = albedo;
    m.refIdx = refIdx;
    return m;
}

struct HitRecord
{
    vec3 pos;
    vec3 normal;
    float t;            // ray parameter
    Material material;
};

//Schlick approximation 
float schlick(float cosine, float refIdx)
{
    float r0 = (1.0 - refIdx) / (1.0 + refIdx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

//Caculating refraction direction
bool refraction(const in vec3 v, const in vec3 n, const in float ni_over_nt, 
                      out vec3 refracted) {
    float dt = dot(normalize(v), n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1.0-dt*dt);
    if (discriminant > 0.0) {
        refracted = ni_over_nt*(normalize(v) - n*dt) - n*sqrt(discriminant);
        return true;
    } else { 
        return false;
    }
}

bool scatter(Ray rIn, HitRecord rec, out vec3 atten, out Ray rScattered)
{
    if(rec.material.type == MT_DIFFUSE)
    {
        vec3 reflected = rec.normal + randomInUnitSphere(gSeed);
        rScattered = createRay(rec.pos + rec.normal * epsilon, normalize(reflected), rIn.t);
        atten = rec.material.albedo  * max(dot(rScattered.d, rec.normal), rIn.t) / pi;
        return true;
    }
    if(rec.material.type == MT_METAL)
    {
       vec3 reflected = reflect(rIn.d, rec.normal); 
        rScattered = createRay(rec.pos + rec.normal * epsilon, normalize(reflected + randomInUnitSphere(gSeed) * rec.material.roughness), rIn.t);
        atten = rec.material.albedo;
        return true;
    }
    if(rec.material.type == MT_DIALECTRIC)
    {
        atten = rec.material.albedo; //vec3(1.0) if no colour
        vec3 outwardNormal;
        float niOverNt;
        float cosine;
        vec3 refracted; 
        if(dot(rIn.d, rec.normal) > 0.0) //hit inside
        {
            outwardNormal = -rec.normal;
            niOverNt = rec.material.refIdx;
            cosine = rec.material.refIdx * dot(rIn.d, rec.normal);
        }
        else  //hit from outside
        {
            outwardNormal = rec.normal;
            niOverNt = 1.0 / rec.material.refIdx;
            cosine = -dot(rIn.d, rec.normal); 
        }

        //Use probabilistic math to decide if scatter a reflected ray or a refracted ray
        //if no total reflection  
        //else 

        float reflectProb;
        if(refraction( rIn.d, outwardNormal, niOverNt, refracted)){
            reflectProb = schlick(cosine, rec.material.refIdx);  
        }else{
            // total reflection
            reflectProb = 1.0;
        }
        
        vec3 reflected = reflect(rIn.d, rec.normal);
        if( hash1(gSeed) < reflectProb){ //Reflection
            // rScattered = calculate reflected ray
            // atten *= vec3(reflectProb); not necessary since we are only scattering reflectProb rays and not all reflected rays
            rScattered = createRay(rec.pos, normalize(reflected), rIn.t);
        }  
        else{
            //Refraction
            // rScattered = calculate refracted ray
            // atten *= vec3(1.0 - reflectProb); not necessary since we are only scattering 1-reflectProb rays and not all refracted rays
            rScattered = createRay(rec.pos, normalize(refracted), rIn.t);
        }  

        return true;
    }
    return false;
}

struct pointLight {
    vec3 pos;
    vec3 color;
};

pointLight createPointLight(vec3 pos, vec3 color) 
{
    pointLight l;
    l.pos = pos;
    l.color = color;
    return l;
}

struct Triangle {vec3 a; vec3 b; vec3 c; };

Triangle createTriangle(vec3 v0, vec3 v1, vec3 v2)
{
    Triangle t;
    t.a = v0; t.b = v1; t.c = v2;
    return t;
}
//Ray/Triangle intersection test using Tomas Moller-Ben Trumbore algorithm.
bool hit_triangle(Triangle t, Ray r, float tmin, float tmax, out HitRecord rec)
{
    vec3 AB = t.a - t.b;
    vec3 AC = t.a - t.c;

    vec3 BA = t.b - t.a;
    vec3 CA = t.c - t.a;

    float d = t.a.x - r.o.x;
    float h = t.a.y - r.o.y;
    float l = t.a.z - r.o.z;

    float m = AC.y * r.d.z - r.d.y * AC.z;
    float n = h * r.d.z - r.d.y *l;
    float p = AC.y * l - h * AC.z;
    float q = r.d.y * AB.z - AB.y * r.d.z;
    float s = AB.y * AC.z - AC.y * AB.z;

    float inv = 1. / (AB.x * m +  AC.x * q +  r.d.x * s);
    float beta = (d * m - AC.x * n - r.d.x * p) * inv;
    float gamma = (AB.x * n + d * q + r.d.x * (AB.y * l - h * AB.z)) * inv;
    
    float rTest = AB.y * l - h * AB.z;

    float e3 = AB.x * p - AC.x * rTest + d * s;
	float tExtra = e3 * inv;

    if (beta < 0.0 || gamma < 0.0 || beta + gamma > 1.0 || tExtra < epsilon) {
		return false;
	}
	
    vec3 normal = normalize(cross(BA,CA));
    if(tExtra < tmax && tExtra > tmin)
    {
        rec.t = tExtra;
        rec.normal = normal;
        rec.pos = pointOnRay(r, rec.t);
        return true;
    }
    return false;
}


struct Sphere
{
    vec3 center;
    float radius;
};

Sphere createSphere(vec3 center, float radius)
{
    Sphere s;
    s.center = center;
    s.radius = radius;
    return s;
}


struct MovingSphere
{
    vec3 center0, center1;
    float radius;
    float time0, time1;
};

MovingSphere createMovingSphere(vec3 center0, vec3 center1, float radius, float time0, float time1)
{
    MovingSphere s;
    s.center0 = center0;
    s.center1 = center1;
    s.radius = radius;
    s.time0 = time0;
    s.time1 = time1;
    return s;
}

vec3 center(MovingSphere mvsphere, float time)
{
    return mvsphere.center0 + ((time - mvsphere.time0)/ (mvsphere.time1 - mvsphere.time0)) * (mvsphere.center1 - mvsphere.center0);
}


/*
 * The function naming convention changes with these functions to show that they implement a sort of interface for
 * the book's notion of "hittable". E.g. hit_<type>.
 */

//Sphere intersection
bool hit_sphere(Sphere s, Ray r, float tmin, float tmax, out HitRecord rec)
{
    float t;
    vec3 distanceCenter = r.o - s.center;
    float a = dot(r.d, r.d);
    float b = dot(distanceCenter, r.d);
    float c = dot(distanceCenter, distanceCenter) - s.radius * s.radius;
    float delta = b * b - a * c;
    if(delta > 0.0)
    {
        float sqrtDiscriminant = sqrt(delta);
        float res1 = (-b - sqrt(delta)) / a;
        float res2 = (-b + sqrt(delta)) / a;

        t = res1 < tmin ? res2 : res1;
        if(t < tmax && t > tmin)
        {
            rec.t = t;
            rec.pos = pointOnRay(r, rec.t);
            rec.normal = (rec.pos - s.center) / s.radius;
            return true;
        } else {
            return false;
        }
    }
    return false;


}

//Moving Sphere interseciont (Motion Blur)
bool hit_movingSphere(MovingSphere s, Ray r, float tmin, float tmax, out HitRecord rec)
{
    float B, C;
    bool outside;
    float t;
    vec3 normal;
    vec3 distanceCenter = r.o - center(s, r.t);
    float a = dot(r.d, r.d);
    float b = dot(distanceCenter, r.d);
    float c = dot(distanceCenter, distanceCenter) - s.radius * s.radius;
    float delta = b * b - a * c;
    if(delta > 0.0)
    {
        float sqrtDiscriminant = sqrt(delta);
        float res1 = (-b - sqrt(delta)) / a;
        float res2 = (-b + sqrt(delta)) / a;

        t = res1 < tmin ? res2 : res1;
        if(t < tmax && t > tmin)
        {
            rec.t = t;
            rec.pos = pointOnRay(r, rec.t);
            rec.normal = (rec.pos - center(s,r.t)) / s.radius;
            return true;
        } else {
            return false;
        }
    }
    return false;

}

struct Box {vec3 a; vec3 b;};

Box createBox(vec3 v0, vec3 v1)
{
    Box b;
    b.a = v0; b.b = v1;
    return b;
}

//Box intersection using Kay and Kajiya Algorithm
bool hit_box(Box box, Ray r, float tmin, float tmax, out HitRecord rec)
{
    float ox = r.o.x;
	float oy = r.o.y;
	float oz = r.o.z;
	float dx = r.d.x;
	float dy = r.d.y;
	float dz = r.d.z;

	float x0 = box.a.x;
	float y0 = box.a.y;
	float z0 = box.a.z;
	float x1 = box.b.x;
	float y1 = box.b.y;
	float z1 = box.b.z;

    float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;

	float a = 1.0 / dx;
	if (a >= 0.0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	float b = 1.0 / dy;
	if (b >= 0.) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	float c = 1.0 / dz;
	if (c >= 0.) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	vec3 face_in, face_out; // aabb normals

	// finding largest entering t value
	if (tx_min > ty_min) {
		tmin = tx_min;
		face_in = (a >= 0.0) ? vec3(-1, 0, 0) : vec3(1, 0, 0); 
	}
	else {
		tmin = ty_min;
		face_in = (b >= 0.0) ? vec3(0, -1, 0) : vec3(0, 1, 0);
	}

	if (tz_min > tmin) {
		tmin = tz_min;
		face_in = (c >= 0.0) ? vec3(0, 0, -1) : vec3(0, 0, 1);
	}


	// finding smallest leaving t value
	if (tx_max < ty_max) {
		tmax = tx_max;
		face_out = (a >= 0.0) ? vec3(1, 0, 0) : vec3(-1, 0, 0);
	}
	else {
		tmax = ty_max;
		face_out = (b >= 0.0) ? vec3(0, 1, 0) : vec3(0, -1, 0);
	}

	if (tz_max < tmax) {
		tmax = tz_max;
		face_out = (c >= 0.0) ? vec3(0, 0, 1) : vec3(0, 0, -1);
	}


	// hit condition: intervals overlapping
	if (tmin < tmax && tmax > 0.) {
		if (tmax > 0.) { // outside
			rec.t = tmin;
			rec.normal = face_in;
		}
		else { // inside
			rec.t = tmax;
			rec.normal = face_out;
		}
        rec.pos = pointOnRay(r, rec.t);
		return true;
	}
	// else ray is behind the box

	return false;


}
