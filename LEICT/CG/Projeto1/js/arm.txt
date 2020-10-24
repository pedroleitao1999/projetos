var camera, scene, renderer, arm;

var geometry, material, mesh;

var ball;

var rotObjectMatrix;

function addTopArm(obj, x, y, z) {
    'use strict';
    geometry = new THREE.CubeGeometry(15, 3, 3);
    material = new THREE.MeshBasicMaterial({ color: 0x6a0c0b, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addHand(obj, x, y, z) {
    'use strict';
    geometry = new THREE.CubeGeometry(1, 6.5, 5);
    material = new THREE.MeshBasicMaterial({ color: 0xb97d10, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addFinger(obj, x, y, z){
    'use strict';
    geometry = new THREE.CubeGeometry(4, 0.5, 0.5);
    material = new THREE.MeshBasicMaterial({ color: 0xb97d10, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addBottomArm(obj, x, y, z) {
    'use strict';
    geometry = new THREE.CubeGeometry(3, 15, 3);
    material = new THREE.MeshBasicMaterial({ color: 0x6a0c0b, wireframe: true });
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function createArm(x, y, z) {
    'use strict';

    var toparm = new THREE.Object3D();
    var hand = new THREE.Object3D();
    var fingers = new THREE.Object3D();
    var bottomarm = new THREE.Object3D();
    arm = new THREE.Group();
    var balls = new THREE.Object3D();

    addTopArm(toparm, 9.2, 19.7, 0);
    addHand(hand, 21.8, 19.7, 0);
    addFinger(fingers, 24.3, 21.7, 0);
    addFinger(fingers, 24.3, 17.7, 0);
    addBottomArm(bottomarm, 0, 10.3, 0);
    addWheel(balls, 18.6, 19.7, 0);
    addWheel(balls, 0, 19.7, 0);

    arm.add(toparm);
    arm.add(hand);
    arm.add(fingers);
    arm.add(bottomarm);
    arm.add(balls);

    arm.position.x = x;
    arm.position.y = y;
    arm.position.z = z;
}
