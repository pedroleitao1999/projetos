var newBall, selectedBall;

var ballGeometry, ballMaterial;

var ballsArray = [], ballsNumber = 0, i;

var movementArray = [];

var axesHelperArray = [];

var randomAngle, x_speed, z_speed;

var x1, x2, z1, z2;


function createBall(object, x, y, z) {
    'use strict';
     ballGeometry = new THREE.SphereGeometry(3, 15, 15);
     ballMaterial = new THREE.MeshBasicMaterial({ color: 0xf9d5bb, wireframe: false });
     object = new THREE.Mesh(ballGeometry, ballMaterial);
     object.position.set(x, y, z);
     ballsArray[ballsNumber] = object;
     scene.add(object);

     x_speed = getRandom(0.25, 0.75);
     z_speed = getRandom(0.25, 0.75);
     randomAngle = getRandom(-Math.PI, Math.PI);
     deltaTime = clock.getDelta();
     movementArray[ballsNumber] = new THREE.Vector3(Math.cos(randomAngle) * x_speed, 0, Math.sin(randomAngle) * z_speed);

     axesHelperArray[ballsNumber] = new THREE.AxisHelper(5);
     ballsArray[ballsNumber].add(axesHelperArray[ballsNumber]);
     if (pressed_r) axesHelperArray[ballsNumber].visible = true;
     else axesHelperArray[ballsNumber].visible = false;

     ballsNumber += 1;
}

function checkWallColision(ball) {
    'use strict';
    if(ballsArray[ball].position.y < 0) return;

    if(ballsArray[ball].position.x <= -83) movementArray[ball].x *= -1;
    if(ballsArray[ball].position.z <= -37 && ballsArray[ball].position.z >= -41 && ballsArray[ball].position.x <= -7) movementArray[ball].z *= -1;
    if(ballsArray[ball].position.z >= 37 && ballsArray[ball].position.z <= 41 && ballsArray[ball].position.x <= -7) movementArray[ball].z *= -1;
    if(ballsArray[ball].position.x <= -7.5 && ballsArray[ball].position.z <= 45 && ballsArray[ball].position.z >= 38) movementArray[ball].x *= -1;
    if(ballsArray[ball].position.x <= -7.5 && ballsArray[ball].position.z >= -45 && ballsArray[ball].position.z <= -38) movementArray[ball].x *= -1;
}


function debugWalls(ball) {
  console.log("paredes");
  if(ballsArray[ball].position.y > -5) {
    while(ballsArray[ball].position.x + 88 < 5) ballsArray[ball].position.x += 3;
  }
  else if(ballsArray[ball].position.z <= -37 && ballsArray[ball].position.x <= -7 && ballsArray[ball].position.y > -5) {
    while(ballsArray[ball].position.z + 42 < 5) ballsArray[ball].position.z += 3;
  }
  else if(ballsArray[ball].position.z >= 37 && ballsArray[ball].position.x <= -7 && ballsArray[ball].position.y > -5) {
    while(ballsArray[ball].position.z - 42 < 5) ballsArray[ball].position.z -= 3;
  }
  if(ball == ballsNumber - 1) setTimeout('debugWalls(0)', 1000);
  else debugWalls(ball + 1);
}

function turnToVector(ball) {
    'use strict';
    return new THREE.Vector3(ballsArray[ball].position.x, ballsArray[ball].position.y, ballsArray[ball].position.z);
}

function checkBallColision(ball) {
    'use strict';
    if(ballsArray[ball].position.y < 0) return;

    var thisBall = turnToVector(ball);
    for(var i = ball + 1; i < ballsNumber; i++) {
        var otherBall = turnToVector(i);
        if(thisBall.distanceTo(otherBall) <= 6) {

            x1 = movementArray[ball].x;
            x2 = movementArray[i].x;
            movementArray[ball].x = x2;
            movementArray[i].x = x1;

            z1 = movementArray[ball].z;
            z2 = movementArray[i].z;
            movementArray[ball].z = z2;
            movementArray[i].z = z1;

        }
    }
}

function debugBalls(ball) {
  var thisBall = turnToVector(ball);
  for(var i = 0 + 1; i < ballsNumber; i++) {
    if(i == ball && i != ballsNumber - 1) i++;
    var otherBall = turnToVector(i);
    if(thisBall.distanceTo(otherBall) < 5.5) {
      var auxMovement = turnToVector(ball);
      auxMovement.sub(otherBall);
      auxMovement.multiplyScalar(0.2);
      ballsArray[ball].position.add(auxMovement);
      auxMovement.multiplyScalar(-1);
      ballsArray[i].position.add(auxMovement);
    }
  }
  if(ball == ballsNumber - 1) setTimeout('debugBalls(0)', 1000);
  else debugBalls(ball + 1);
}

function fall(ball) {
  'use strict';
  if(ballsArray[ball].position.x <= -89 || ballsArray[ball].position.x >= -6 && (movementArray[ball].x >= 0)) {
    movementArray[ball].y -= 3 * deltaTime;
    if(ball == ballsNumber - 1 && cannon_awaiting == 0) cannon_awaiting = 1;
  }
  if(ballsArray[ball].position.z >= 51 || ballsArray[ball].position.z <= -51) {
    movementArray[ball].y -= 3 * deltaTime;
    if(ball == ballsNumber - 1 && cannon_awaiting == 0) cannon_awaiting = 1;
  }
}

function rotateAroundAxis(object, axis, radians) {
  'use strict'
    var rotWorldMatrix = new THREE.Matrix4();
    rotWorldMatrix.makeRotationAxis(axis.normalize(), radians);
    rotWorldMatrix.multiply(object.matrix);
    object.matrix = rotWorldMatrix;
    object.rotation.setFromRotationMatrix(object.matrix, 'XYZ');
}

function move(ball) {
    'use strict'
    var x = movementArray[ball].x;
    var y = movementArray[ball].y;
    var z = movementArray[ball].z;
    var auxMovement = new THREE.Vector3(x, y, z);
    auxMovement.multiplyScalar(75 * deltaTime);
    ballsArray[ball].position.add(auxMovement);

    var axis = new THREE.Vector3(movementArray[ball].z, 0, -movementArray[ball].x).normalize();

    rotateAroundAxis(ballsArray[ball], axis, movementArray[ball].length() / 12);
}

function applyFriction(ball) {
  'use strict'
  if(movementArray[ball].length() <= 0.005) movementArray[ball].setLength(0);
  else movementArray[ball].setLength( movementArray[ball].length() - 0.003 );
}
