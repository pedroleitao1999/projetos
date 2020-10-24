var camera, scene, renderer;

var camera, cameraScene, cameraPause;

var clock, deltaTime;

var ballSpeed, ballAcceleration;

var isNotPaused;

var numObjects, objects = [], originalObjects = [];

var basicMaterials = [], otherMaterials = [];

var isBasicMaterial;

var sceneWireframe = false;

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    createChess();
    createDie();
    createBall();
    createPause();
    createCameraScene();
    createCameraPause();
    camera = cameraScene;
    scene.add(camera);
}

function createCameraScene() {
    'use strict';
    cameraScene = new THREE.PerspectiveCamera(80,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    cameraScene.position.x = 75;
    cameraScene.position.y = 75;
    cameraScene.position.z = 33.5;
    cameraScene.lookAt(chess.position);
}

function createCameraPause() {
    'use strict';
    cameraPause = new THREE.PerspectiveCamera(80,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    cameraPause.position.x = 500;
    cameraPause.position.y = 75;
    cameraPause.position.z = 0;
    cameraPause.lookAt(pause.position);
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera.left = window.innerWidth / -10;
        camera.right = window.innerWidth / 10;
        camera.top = window.innerHeight / 10;
        camera.bottom = window.innerHeight / -10;
        camera.near = 1;
        camera.far = 1000;
        camera.updateProjectionMatrix();

        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
    }
}

function onKeyPress(e) {
  'use strict';

  switch (e.keyCode) {
    case 66:
    case 98:
      if(isNotPaused){
        ballAcceleration *= -1;
      }
      break;
    case 68:
    case 100:
      if(isNotPaused){
        if(directionalLight.intensity == 0) directionalLight.intensity = dIntensity;
        else directionalLight.intensity = 0;
      }
      break;
    case 80:
    case 112:
      if(isNotPaused){
        if(pointLight.intensity == 0) pointLight.intensity = plIntensity;
        else pointLight.intensity = 0;
      }
      break;
    case 82:
    case 114:
      if(!isNotPaused)
        reset();
      break;
    case 83:
    case 115:
      if(isNotPaused){
        camera = cameraPause;
        isNotPaused = 0;
      }
      else{
        camera = cameraScene;
        isNotPaused = 1;
      }
      break;
    case 87:
    case 119:
      if(isNotPaused){
        sceneWireframe = !sceneWireframe;
        for(var i = 0; i < numObjects; i++){
          basicMaterials[i].wireframe = sceneWireframe;
          otherMaterials[i].wireframe = sceneWireframe;
        }
        for(var i = 0; i < 6; i++){
          dieBasicMaterials[i].wireframe = sceneWireframe;
          dieStandardMaterials[i].wireframe = sceneWireframe;
        }
      }
      break;
    case 76:
    case 108:
      if(isNotPaused){
        if(isBasicMaterial){
          isBasicMaterial = 0;
          changeToOtherMaterial();
        }
        else{
          isBasicMaterial = 1;
          changeToBasicMaterial();
        }
      break;
    }
  }
}

function changeToOtherMaterial() {
  for(var i = 0; i < numObjects; i++){
    objects[i].material = otherMaterials[i];
  }
}

function changeToBasicMaterial() {
  for(var i = 0; i < numObjects; i++){
    objects[i].material = basicMaterials[i];
  }
}

function removeObjects() {
  'use strict';
  scene.remove(ballAxis);
  scene.remove(chess);
  scene.remove(pointLight);
  scene.remove(directionalLight);
  scene.remove(yDieAxis);
}

function reset() {
    'use strict';
    removeObjects();
    objects = [], originalObjects = [];
    basicMaterials = [], otherMaterials = [];
    numObjects = 0;
    ballSpeed = 0
    ballAcceleration = -1;
    isNotPaused = 1;
    sceneWireframe = false;
    camera = cameraScene;
    createChess();
    createDie();
    createBall();
}

function init() {
    'use strict';
    objects = [], originalObjects = [];
    basicMaterials = [], otherMaterials = [];
    numObjects = 0;
    ballSpeed = 0;
    ballAcceleration = -1;
    isNotPaused = 1;
    isBasicMaterial = 0;

    clock = new THREE.Clock(true);

    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    render();

    window.addEventListener("resize", onResize);
    window.addEventListener("keypress", onKeyPress);
}

function render() {
  'use strict';
  renderer.render(scene, camera);
}

function animate() {
    'use strict';

    update();
    render();
    requestAnimationFrame(animate);
}

function update() {
    'use strict';
    deltaTime = clock.getDelta();
    deltaTime *= isNotPaused;

    ballMovement();
    dieRotation();
}

function ballMovement() {
  'use strict'
  if(!(ballSpeed < 0.01 && ballAcceleration == -1 || ballSpeed > 10  && ballAcceleration == 1)) {
    ballSpeed += ballAcceleration * deltaTime;
  }
  if(ballSpeed < 0.01) ballSpeed = 0;

  ballAxis.rotation.y += Math.PI/2 * ballSpeed * deltaTime;

  ball.rotation.y += Math.PI/2 * ballSpeed * deltaTime;
}

function dieRotation() {
  'use strict'
  yDieAxis.rotation.y -= deltaTime;
}
