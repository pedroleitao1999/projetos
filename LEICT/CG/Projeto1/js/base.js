var camera, scene, renderer, car;

var geometry, material, mesh;

var ball;

function addCalote(obj, x, y, z) {
    'use strict';
    geometry = new THREE.SphereGeometry(3.5, 10, 10, Math.PI/2, Math.PI*2, 0, Math.PI/2);
    material = new THREE.MeshBasicMaterial({ color: 0xfbca03, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addBase(obj, x, y, z) {
    'use strict';
    geometry = new THREE.CubeGeometry(30, 3, 20);
    material = new THREE.MeshBasicMaterial({ color: 0x4b0908, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addWheel(obj, x, y, z) {
    'use strict';
    geometry = new THREE.SphereGeometry(2.7, 10, 10);
    material = new THREE.MeshBasicMaterial({ color: 0xfbca03, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function createBase() {
    'use strict';

    var base = new THREE.Object3D();
    var wheels = new THREE.Object3D();
    var calote = new THREE.Object3D();
    car = new THREE.Group();

    material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true });

    addBase(base, 0, 4.2, 0);
    addWheel(wheels, -12, 0, 7);
    addWheel(wheels, -12, 0, -7);
    addWheel(wheels, 12, 0, 7);
    addWheel(wheels, 12, 0, -7);
    addCalote(calote, 0, 5.7, 0);

    car.add(base);
    car.add(wheels);
    car.add(calote);

    scene.add(car);
}
