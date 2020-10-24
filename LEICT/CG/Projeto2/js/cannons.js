var cannons;

var cannonArray = [];

var directionArray = [];

var cannonMaterial, cannonGeometry1, cannonGeometry2, cannonGeometry3;

var selectedCannon;

function createcannons() {
    'use strict';

    var cannons = new THREE.Group();
    var cannonGeometry1 = new THREE.CylinderGeometry(3, 3, 20, 15);
    var cannonGeometry2 = new THREE.CylinderGeometry(3, 3, 20, 15);
    var cannonGeometry3 = new THREE.CylinderGeometry(3, 3, 20, 15);

    var cannonMaterial1 = new THREE.MeshBasicMaterial({ color: 0x3c3d47, wireframe: false });
    var cannonMaterial2 = new THREE.MeshBasicMaterial({ color: 0x3c3d47, wireframe: false });
    var cannonMaterial3 = new THREE.MeshBasicMaterial({ color: 0x3c3d47, wireframe: false });

    cannonArray[0] = new THREE.Mesh(cannonGeometry1, cannonMaterial1);
    cannonArray[0].position.set(50, 0, 30);
    cannonArray[0].rotation.z += Math.PI/2;
    directionArray[0] = new THREE.Vector3(-1, 0, 0);

    cannonArray[1] = new THREE.Mesh(cannonGeometry2, cannonMaterial2);
    cannonArray[1].position.set(50, 0, 0);
    cannonArray[1].rotation.z += Math.PI/2;
    directionArray[1] = new THREE.Vector3(-1, 0, 0);

    cannonArray[2] = new THREE.Mesh(cannonGeometry3, cannonMaterial3);
    cannonArray[2].position.set(50, 0, -30);
    cannonArray[2].rotation.z += Math.PI/2;
    directionArray[2] = new THREE.Vector3(-1, 0, 0);

    cannons.add(cannonArray[0]);
    cannons.add(cannonArray[1]);
    cannons.add(cannonArray[2]);

    scene.add(cannons);

}

function cannonSelect(cannon, number){
    'use strict';
    if(cannon_awaiting == -1) cannon_awaiting = 1;
    cannonArray[0].material.color.set(0x3c3d47);
    cannonArray[1].material.color.set(0x3c3d47);
    cannonArray[2].material.color.set(0x3c3d47);
    cannon.material.color.set(0x8688A4);
    selectedCannon = number;
}

function shootCannon() {
  cannon_awaiting = 0;
  createBall(newBall, 50 - 10*Math.cos(cannonArray[selectedCannon].rotation.y), 0,
                cannonArray[selectedCannon].position.z + 10*Math.sin(cannonArray[selectedCannon].rotation.y));
  var x = directionArray[selectedCannon].x;
  var z = directionArray[selectedCannon].z;
  movementArray[ballsNumber - 1] = new THREE.Vector3(x, 0, z);
}

function rotateCannon(direction) {
  if(direction == "right" && cannonArray[selectedCannon].rotation.y > -Math.PI/2) {
    cannonArray[selectedCannon].rotation.y -= (Math.PI/3) * deltaTime;
  }
  else if(direction == "left" && cannonArray[selectedCannon].rotation.y < Math.PI/2) {
    cannonArray[selectedCannon].rotation.y += Math.PI/3 * deltaTime;
  }
  directionArray[selectedCannon] = new THREE.Vector3( -Math.cos( cannonArray[selectedCannon].rotation.y ), 0, Math.sin( cannonArray[selectedCannon].rotation.y ) );
}
