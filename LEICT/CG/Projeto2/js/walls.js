var walls;

var wall1, wall2, wall3, wall4, wall5;

var wallMaterial, wallMaterial2;

var wallGeometry1, wallGeometry2, wallGeometry3, wallGeometry4, wallGeometry5;

function createWalls() {
    'use strict';

    walls = new THREE.Group();
    wallGeometry1 = new THREE.CubeGeometry(80, 7, 4);
    wallGeometry2 = new THREE.CubeGeometry(4, 7, 80);
    wallGeometry3 = new THREE.CubeGeometry(80, 7, 4);
    wallGeometry4 = new THREE.CubeGeometry(90, 2, 100);
    wallGeometry5 = new THREE.CubeGeometry(40, 2, 100);

    wallMaterial = new THREE.MeshBasicMaterial({ color: 0x3c3d47, wireframe: false });
    wallMaterial2 = new THREE.MeshBasicMaterial({ color: 0x272121, wireframe: false });

    wall1 = new THREE.Mesh(wallGeometry1, wallMaterial);
    wall1.position.set(-50, 0, 42);

    wall2 = new THREE.Mesh(wallGeometry2, wallMaterial);
    wall2.position.set(-88, 0, 0);

    wall3 = new THREE.Mesh(wallGeometry3, wallMaterial);
    wall3.position.set(-50, 0, -42);

    wall4 = new THREE.Mesh(wallGeometry4, wallMaterial2);
    wall4.position.set(-50, -4 , 0);

    wall5 = new THREE.Mesh(wallGeometry5, wallMaterial2);
    wall5.position.set(70, -4 , 0);

    walls.add(wall1);
    walls.add(wall2);
    walls.add(wall3);
    walls.add(wall4);
    walls.add(wall5);

    scene.add(walls);
}
