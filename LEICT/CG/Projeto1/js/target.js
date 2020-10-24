var camera, scene, renderer;

var geometry, material, mesh;

var ball, target;

function addCylinder(obj, x, y, z){
  'use strict';
  geometry = new THREE.CylinderGeometry(3.5, 3.5, 24, 10);
  material = new THREE.MeshBasicMaterial({ color: 0x67c7eb, wireframe: true });
  mesh = new THREE.Mesh(geometry, material);
  mesh.position.set(x, y, z);
  obj.add(mesh);
}

function addTorus(obj, x, y, z){
  'use strict';
  geometry = new THREE.TorusGeometry(2.7, 0.8, 10, 28);
  material = new THREE.MeshBasicMaterial({ color: 0x67c7eb, wireframe: true });
  mesh = new THREE.Mesh(geometry, material);
  mesh.position.set(x, y, z);
  obj.add(mesh);
}

function createTarget() {
    'use strict';

    target = new THREE.Group();
    var cylinder = new THREE.Object3D();
    var torus = new THREE.Object3D();

    material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true });

    addCylinder(cylinder, 40, 9.3, 0);
    addTorus(torus, 40, 24.8, 0);

    target.add(cylinder);
    target.add(torus);

    scene.add(target);

}
