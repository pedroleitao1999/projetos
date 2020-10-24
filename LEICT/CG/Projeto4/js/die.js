var yDieAxis, zDieAxis, die;

var dieBasicMaterials = [], dieStandardMaterials = [];

var dieSize = 15;

function createDie() {
  'use strict'
  yDieAxis = new THREE.Group();

  zDieAxis = new THREE.Group();

  var dieGeometry = new THREE.CubeGeometry(dieSize, dieSize, dieSize, dieSize, dieSize, dieSize)

  var texture = new THREE.TextureLoader();
  var one = texture.load("/js/1.jpg");
  var two = texture.load("/js/2.jpg");
  var three = texture.load("/js/3.jpg");
  var four = texture.load("/js/4.jpg");
  var five = texture.load("/js/5.jpg");
  var six = texture.load("/js/6.jpg");

  dieBasicMaterials = [
    new THREE.MeshBasicMaterial({map: one}),
    new THREE.MeshBasicMaterial({map: two}),
    new THREE.MeshBasicMaterial({map: three}),
    new THREE.MeshBasicMaterial({map: four}),
    new THREE.MeshBasicMaterial({map: five}),
    new THREE.MeshBasicMaterial({map: six})
  ]

  dieStandardMaterials = [
    new THREE.MeshPhongMaterial({map: one, bumpMap: one, bumpScale: 0.5}),
    new THREE.MeshPhongMaterial({map: two, bumpMap: two, bumpScale: 0.5}),
    new THREE.MeshPhongMaterial({map: three, bumpMap: three, bumpScale: 0.5}),
    new THREE.MeshPhongMaterial({map: four, bumpMap: four, bumpScale: 0.5}),
    new THREE.MeshPhongMaterial({map: five, bumpMap: five, bumpScale: 0.5}),
    new THREE.MeshPhongMaterial({map: six, bumpMap: six, bumpScale: 0.5})
  ]

  die = new THREE.Mesh(dieGeometry, dieStandardMaterials);

  basicMaterials[numObjects] = dieBasicMaterials;
  otherMaterials[numObjects] = dieStandardMaterials;
  objects[numObjects] = die;
  numObjects++;

  die.rotation.z = Math.PI / 4;
  die.position.set(0, 0, 0);

  zDieAxis.add(die);
  zDieAxis.rotation.x = Math.PI / 5;

  yDieAxis.add(zDieAxis);
  yDieAxis.position.set(0, dieSize * 0.85, 0);
  scene.add(yDieAxis);
}
