var room;

var floor, floorGeometry, floorDimensions = new THREE.Vector3(100, 5, 140);

var floorBasicMaterial, floorLambertMaterial, floorPhongMaterial;

var wall, wallGeometry, wallDimensions = new THREE.Vector3(5, 95, 140);

var wallBasicMaterial, wallLambertMaterial, wallPhongMaterial;

var cone, coneGeometry, coneDimensions = new THREE.Vector3(6, 0, 15);

var coneBasicMaterial, coneLambertMaterial, conePhongMaterial;

var sphere, sphereGeometry, sphereDimensions = new THREE.Vector3(4, 12, 12);

var sphereBasicMaterial, sphereLambertMaterial, spherePhongMaterial;

var spotlight, spotlights = [];

var light, lights = [], directionalLight;

var allCones, allSpheres;

var allConesBasicMaterial, allConesPhongMaterial, allConesLambertMaterial;

var allSpheresBasicMaterial, allSpheresLambertMaterial, allSpheresPhongMaterial;

function createRoom() {
    'use strict';

    room = new THREE.Group();

    floorGeometry = new THREE.CubeGeometry(floorDimensions.x, floorDimensions.y, floorDimensions.z, floorDimensions.x / 2, floorDimensions.y / 2, floorDimensions.z / 2);
    floorBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x222222, wireframe: false });
    floorLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x222222, wireframe: false });
    floorPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x222222, wireframe: false });

    wallGeometry = new THREE.CubeGeometry(wallDimensions.x, wallDimensions.y, wallDimensions.z, wallDimensions.x / 2, wallDimensions.y / 2, wallDimensions.z / 2);
    wallBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x111111, wireframe: false });
    wallLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x111111, wireframe: false });
    wallPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x111111, wireframe: false });

    coneGeometry = new THREE.CylinderGeometry(coneDimensions.x, coneDimensions.y, coneDimensions.z, 12, 12);
    coneBasicMaterial = new THREE.MeshBasicMaterial({ color: 0xC0C0C0, wireframe: false });
    coneLambertMaterial = new THREE.MeshLambertMaterial({ color: 0xC0C0C0, wireframe: false });
    conePhongMaterial = new THREE.MeshPhongMaterial({ color: 0xC0C0C0, wireframe: false });

    sphereGeometry = new THREE.SphereGeometry(sphereDimensions.x, sphereDimensions.y, sphereDimensions.z);
    sphereBasicMaterial = new THREE.MeshBasicMaterial({ color: 0xFFFACD, wireframe: false });
    sphereLambertMaterial = new THREE.MeshLambertMaterial({ color: 0xFFFACD, wireframe: false });
    spherePhongMaterial = new THREE.MeshPhongMaterial({ color: 0xFFFACD, wireframe: false });

    floor = new THREE.Mesh(floorGeometry, floorBasicMaterial);
    floor.position.set(0, 2.5, 0);

    wall = new THREE.Mesh(wallGeometry, wallBasicMaterial);
    wall.position.set(floor.position.x - floorDimensions.x / 2 + wallDimensions.x / 2, floorDimensions.y + wallDimensions.y / 2, floor.position.z);

    objects[numObjects] = floor;
    basicMaterials[numObjects] = floorBasicMaterial;
    lambertMaterials[numObjects] = floorLambertMaterial;
    phongMaterials[numObjects] = floorPhongMaterial;
    numObjects = numObjects + 1;

    objects[numObjects] = wall;
    basicMaterials[numObjects] = wallBasicMaterial;
    lambertMaterials[numObjects] = wallLambertMaterial;
    phongMaterials[numObjects] = wallPhongMaterial;
    numObjects = numObjects + 1;

    for(var i = 1; i <= 4; i++) {
        spotlight = new THREE.Group();

        cone = new THREE.Mesh(coneGeometry, coneBasicMaterial);
        cone.position.set(0, 0, 0);

        sphere = new THREE.Mesh(sphereGeometry, sphereBasicMaterial);
        sphere.position.set(cone.position.x, cone.position.y + 7.5, cone.position.z);

        objects[numObjects] = cone;
        basicMaterials[numObjects] = coneBasicMaterial;
        lambertMaterials[numObjects] = coneLambertMaterial;
        phongMaterials[numObjects] = conePhongMaterial;
        numObjects = numObjects + 1;

        objects[numObjects] = sphere;
        basicMaterials[numObjects] = sphereBasicMaterial;
        lambertMaterials[numObjects] = sphereLambertMaterial;
        phongMaterials[numObjects] = spherePhongMaterial;
        numObjects = numObjects + 1;

        light = new THREE.SpotLight(0xffffff);
        light.position.set(cone.position.x, cone.position.y + 7.5, cone.position.z);
        light.castShadow = true;

        light.shadow.mapSize.width = 1024;
        light.shadow.mapSize.height = 1024;

        light.shadow.camera.near = 500;
        light.shadow.camera.far = 4000;
        light.shadow.camera.fov = 30;

        spotlight.add(light);
        spotlight.add(cone);
        spotlight.add(sphere);
        room.add(spotlight);
        lights[i - 1] = light;
        spotlights[i - 1] = spotlight;

    }

    spotlights[0].position.set(floor.position.x + floorDimensions.x / 2, wall.position.y + wallDimensions.y / 2 - coneDimensions.z / 2, floor.position.z + floorDimensions.z / 2);
    spotlights[1].position.set(floor.position.x + floorDimensions.x / 2, wall.position.y + wallDimensions.y / 2 - coneDimensions.z / 2, floor.position.z - floorDimensions.z / 2);
    spotlights[2].position.set(floor.position.x - floorDimensions.x / 2 + wallDimensions.x + coneDimensions.x, wall.position.y + wallDimensions.y / 2 - coneDimensions.z / 2, floor.position.z + floorDimensions.z / 2);
    spotlights[3].position.set(floor.position.x - floorDimensions.x / 2 + wallDimensions.x + coneDimensions.x, wall.position.y + wallDimensions.y / 2 - coneDimensions.z / 2, floor.position.z - floorDimensions.z / 2);

    spotlights[0].rotation.x = Math.PI/4 * -3;
    spotlights[0].rotation.z = Math.PI/7;
    spotlights[1].rotation.x = Math.PI/4 * 3;
    spotlights[1].rotation.z = Math.PI/7;
    spotlights[2].rotation.x = Math.PI/4 * -3;
    spotlights[2].rotation.z = Math.PI/8 * -1;
    spotlights[3].rotation.x = Math.PI/4 * 3;
    spotlights[3].rotation.z = Math.PI/8 * -1;

    directionalLight = new THREE.DirectionalLight(0xffffff, 1);
    directionalLight.target = (wall);
    directionalLight.position.set(70, 70, 0);

    room.add(directionalLight);
    room.add(floor);
    room.add(wall);

    scene.add(room);
}
