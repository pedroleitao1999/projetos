var ballAxis, ball;

var ballRadius = 5;

var distanceToDie = 30;

function createBall() {
  'use strict'
  ballAxis = new THREE.Group();

  var ballGeometry = new THREE.SphereGeometry(ballRadius, ballRadius * 5, ballRadius * 5);

  var textureBall = new THREE.TextureLoader();
  var monaLisa = textureBall.load("/js/monalisa.jpg");

  var ballPhongMaterial = new THREE.MeshPhongMaterial({map: monaLisa, shininess: 500, specular: 0xffffff});
  var ballBasicMaterial = new THREE.MeshBasicMaterial({map: monaLisa});

  ball = new THREE.Mesh(ballGeometry, ballPhongMaterial);

  basicMaterials[numObjects] = ballBasicMaterial;
  otherMaterials[numObjects] = ballPhongMaterial;
  objects[numObjects] = ball;
  numObjects++;

  ball.position.set(0, ballRadius, die.position.z + distanceToDie);

  ballAxis.add(ball);

  scene.add(ballAxis);
}
