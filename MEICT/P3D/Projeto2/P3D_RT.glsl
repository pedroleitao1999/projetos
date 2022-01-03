/**
* ver hash functions em
* https://www.shadertoy.com/view/XlGcRh hash functions GPU
* http://www.jcgt.org/published/0009/03/02/
 */

 #include "./common.glsl"
 #iChannel0 "self"

bool hit_world(Ray r, float tmin, float tmax, out HitRecord rec)
{
    bool hit = false;
    rec.t = tmax;
   
    if(hit_triangle(createTriangle(vec3(-10.0, -0.01, 10.0), vec3(10.0, -0.01, 10.0), vec3(-10.0, -0.01, -10.0)), r, tmin, rec.t, rec))
    {
        hit = true;
        rec.material = createDiffuseMaterial(vec3(0.2));
    }
    //creates a diffuse box
    if(hit_box(createBox(vec3(3, 3, 3), vec3(4,4, 4)), r, tmin, rec.t, rec))
    {
        hit = true;
        rec.material = createDiffuseMaterial(vec3(1, 0.0, 0.5));

    }
    //creates a metal box
    if(hit_box(createBox(vec3(-4, 1, 3), vec3(-3,4, 4)), r, tmin, rec.t, rec))
    {
        hit = true;
        rec.material = createMetalMaterial(vec3(0.0, 0.5, 0.5), 0.0);

    }

    if(hit_triangle(createTriangle(vec3(-10.0, -0.01, -10.0), vec3(10.0, -0.01, 10), vec3(10.0, -0.01, -10.0)), r, tmin, rec.t, rec))
    {
        hit = true;
        rec.material = createDiffuseMaterial(vec3(0.2));
    }

    if(hit_sphere(
        createSphere(vec3(-4.0, 1.0, 0.0), 1.0),
        r,
        tmin,
        rec.t,
        rec))
    {
        hit = true;
        //rec.material = createDiffuseMaterial(vec3(0.2, 0.95, 0.1));
        rec.material = createDiffuseMaterial(vec3(0.4, 0.2, 0.1));
    }

    if(hit_sphere(
        createSphere(vec3(4.0, 1.0, 0.0), 1.0),
        r,
        tmin,
        rec.t,
        rec))
    {
        hit = true;
        rec.material = createMetalMaterial(vec3(0.7, 0.6, 0.5), 0.0);
    }

    if(hit_sphere(
        createSphere(vec3(0.0, 1.0, 0.0), 1.0),
        r,
        tmin,
        rec.t,
        rec))
    {
        hit = true;
        rec.material = createDialectricMaterial(vec3(1.0), 1.5);
    }

    if(hit_sphere(
        createSphere(vec3(0.0, 1.0, 0.0), -0.95),
        r,
        tmin,
        rec.t,
        rec))
    {
        hit = true;
        rec.material = createDialectricMaterial(vec3(1.0), 1.5);
    }
   
    int numxy = 5;
    
    for(int x = -numxy; x < numxy; ++x)
    {
        for(int y = -numxy; y < numxy; ++y)
        {
            float fx = float(x);
            float fy = float(y);
            float seed = fx + fy / 1000.0;
            vec3 rand1 = hash3(seed);
            vec3 center = vec3(fx + 0.9 * rand1.x, 0.2, fy + 0.9 * rand1.y);
            float chooseMaterial = rand1.z;
            if(distance(center, vec3(4.0, 0.2, 0.0)) > 0.9)
            {
                if(chooseMaterial < 0.3)
                {
                    vec3 center1 = center + vec3(0.0, hash1(gSeed) * 0.5, 0.0);
                    // diffuse
                    if(hit_movingSphere(
                        createMovingSphere(center, center1, 0.2, 0.0, 1.0),
                        r,
                        tmin,
                        rec.t,
                        rec))
                    {
                        hit = true;
                        rec.material = createDiffuseMaterial(hash3(seed) * hash3(seed));
                    }
                }
                else if(chooseMaterial < 0.5)
                {
                    // diffuse
                    if(hit_sphere(
                        createSphere(center, 0.2),
                        r,
                        tmin,
                        rec.t,
                        rec))
                    {
                        hit = true;
                        rec.material = createDiffuseMaterial(hash3(seed) * hash3(seed));
                    }
                }
                else if(chooseMaterial < 0.7)
                {
                    // metal
                    if(hit_sphere(
                        createSphere(center, 0.2),
                        r,
                        tmin,
                        rec.t,
                        rec))
                    {
                        hit = true;
                        rec.material.type = MT_METAL;
                        rec.material = createMetalMaterial((hash3(seed) + 1.0) * 0.5, 0.0);
                    }
                }
                else if(chooseMaterial < 0.9)
                {
                    // metal
                    if(hit_sphere(
                        createSphere(center, 0.2),
                        r,
                        tmin,
                        rec.t,
                        rec))
                    {
                        hit = true;
                        rec.material.type = MT_METAL;
                        rec.material = createMetalMaterial((hash3(seed) + 1.0) * 0.5, hash1(seed));
                    }
                }
                else
                {
                    // glass (dialectric)
                    if(hit_sphere(
                        createSphere(center, 0.2),
                        r,
                        tmin,
                        rec.t,
                        rec))
                    {
                        hit = true;
                        rec.material.type = MT_DIALECTRIC;
                        rec.material = createDialectricMaterial(hash3(seed), 1.5);
                    }
                }
            }
        }
    }
    return hit;
}

//Blinn-Phong Illumination Model
vec3 directlighting(pointLight pl, Ray r, HitRecord rec){
  
    float shininess;
    float percentSpecular;
    float percentDifuse;
    HitRecord dummy;

    vec3 lightColor = pl.color ;
    vec3 hitpoint = rec.pos;
    vec3 diffCol;
    vec3 specCol;
    vec3 L = normalize(pl.pos - hitpoint);
    float dotNL = max(dot(normalize(rec.normal),L),0.);

    //for each material we assigned specular and diffuse Color and shininess
    if(rec.material.type == 0){
        diffCol = rec.material.albedo;
        shininess = 10.;
        specCol = vec3(0.1);
        percentDifuse = hash1(gSeed);
        percentSpecular = 0.01;
    }else if( rec.material.type == 1){
        shininess = 500.;
        percentSpecular = 1.0;
        percentDifuse = 0.0;
        specCol = rec.material.albedo;
        diffCol = vec3(0.0) ;
    }else{
        diffCol = vec3(0.0);
        specCol = vec3(0.04);
        shininess = 220.;
        percentSpecular = 0.7;
        percentDifuse = 0.0;
    }
    vec3 h = normalize(L - r.d);
    float spec = pow(max(dot(h, normalize(rec.normal)),0.),shininess);
    vec3 diffuse = diffCol * lightColor * dotNL * percentDifuse;
    vec3 specular = specCol * lightColor * spec * percentSpecular;
    vec3 colorOut = diffuse + specular;

    //calculating hard shadows
    bool isInShadow = hit_world(createRay(rec.pos + rec.normal * epsilon, normalize(pl.pos - rec.pos)), 0.001, 10000.0, rec);
    if(dot(normalize(pl.pos - rec.pos), rec.normal) > 0.0){
        if(!isInShadow){
            return colorOut; 
        }
    }

}

#define MAX_BOUNCES 10

vec3 rayColor(Ray r)
{
    HitRecord rec;
    vec3 col = vec3(0.0);
    vec3 throughput = vec3(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < MAX_BOUNCES; ++i)
    {
        if(hit_world(r, 0.001, 10000.0, rec))
        {
            //calculate direct lighting with 3 white point lights:
            Ray scatterRay;
            vec3 atten;

            
            pointLight one = createPointLight(vec3(-10.0, 15.0, 0.0), vec3(1.0, 1.0, 1.0));
            pointLight two = createPointLight(vec3(8.0, 15.0, 3.0), vec3(1.0, 1.0, 1.0));
            pointLight three =createPointLight(vec3(1.0, 15.0, -9.0), vec3(1.0, 1.0, 1.0));

            col += directlighting(one, r, rec) * throughput;
            col += directlighting(two, r, rec) * throughput;
            col += directlighting(three, r, rec) * throughput;

            //calculate secondary ray and update throughput
            if(scatter(r, rec, atten, scatterRay)){   
                r = scatterRay;   
                throughput *= atten;
            }else{
                col = vec3(0.0);
                break;
            }
        }
        else  //background
        {
            float t = 0.8 * (r.d.y + 1.0);
            col += throughput * mix(vec3(1.0), vec3(0.5, 0.7, 1.0), t);
            break;
        }
    }
    
    return col;

}

#define MAX_SAMPLES 10000.0

void main()
{
    gSeed = float(baseHash(floatBitsToUint(gl_FragCoord.xy))) / float(0xffffffffU) + iTime;

    vec2 mouse = iMouse.xy / iResolution.xy;
    mouse.x = mouse.x * 2.0 - 1.0;

    vec3 camPos = vec3(+10.*cos(1.5+6.*mouse.x) , 8.*mouse.y, +10.*sin(1.5+6.*mouse.x));
    vec3 camTarget = vec3(0.0, 0.0, -1.0);
    float fovy = 60.0;
    float aperture = 0.0; //DOF (change value to see effect) 
    float distToFocus = 1.0; //DOF (change value to see effect) 
    float time0 = 0.0;
    float time1 = 1.0;
    Camera cam = createCamera(
        camPos,
        camTarget,
        vec3(0.0, 1.0, 0.0),    // world up vector
        fovy,
        iResolution.x / iResolution.y,
        aperture,
        distToFocus,
        time0,
        time1);

    //usa-se o 4 canal de cor para guardar o numero de samples e não o iFrame pois quando se mexe o rato faz-se reset
    vec4 prev = texture(iChannel0, gl_FragCoord.xy / iResolution.xy); 
    vec3 prevLinear = toLinear(prev.xyz);

    vec2 ps = (gl_FragCoord.xy + hash2(gSeed));
    //  vec2 ps = gl_FragCoord.xy;
    vec3 color = rayColor(getRay(cam, ps)); 

    if(iMouseButton.x != 0.0 || iMouseButton.y != 0.0)
    {
        gl_FragColor = vec4(toGamma(color), 1.0);  //samples number reset = 1
        return;
    }
    if(prev.w > MAX_SAMPLES) 
    {
        gl_FragColor = prev;
        return;
    }

    float w = prev.w + 1.0;
    color = mix(prevLinear, color, 1.0/w);
    gl_FragColor = vec4(toGamma(color), w);

}