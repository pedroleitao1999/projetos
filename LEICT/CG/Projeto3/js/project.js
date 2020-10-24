var camera, scene, renderer;

var camera1, camera2;

var clock, deltaTime;

var objects = [];

var numObjects = 0;

var basicMaterials = [], lambertMaterials = [], phongMaterials = [];

var active = 0, pressedE = 0;
//Basic = 0, Lambert = 1, Phong = 2

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    createRoom();
    createPainting();
    createSculpture();
    createCamera1();
    createCamera2();
    camera = camera1;
    scene.add(camera);
}

function createCamera1() {
    'use strict';
    camera1 = new THREE.OrthographicCamera(window.innerWidth / -40,
                                        window.innerWidth / 40,
                                        window.innerHeight / 40,
                                        window.innerHeight / -40,
                                        1, 1000);
    camera1.position.x = 0;
    camera1.position.y = frame.position.y;
    camera1.position.z = frame.position.z;
    camera1.lookAt(frame.position);
    scene.add(camera1);
}

function createCamera2() {
    'use strict';
    camera2 = new THREE.PerspectiveCamera(80,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    camera2.position.x = 100;
    camera2.position.y = 100;
    camera2.position.z = 50;
    camera2.lookAt(new THREE.Vector3(-30, 15, -10));
    scene.add(camera2);
}

function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera1.left = window.innerWidth / -10;
        camera1.right = window.innerWidth / 10;
        camera1.top = window.innerHeight / 10;
        camera1.bottom = window.innerHeight / -10;
        camera1.near = 1;
        camera1.far = 1000;
        camera1.updateProjectionMatrix();

        camera2.aspect = window.innerWidth / window.innerHeight;
        camera2.updateProjectionMatrix();
    }
}

function onKeyPress(e) {
  'use strict';

  switch (e.keyCode) {
    case 49:
    if(lights[0].power > 0)
        lights[0].power = 0;
    else
        lights[0].power = Math.PI;
    break;
    case 50:
    if(lights[1].power > 0)
        lights[1].power = 0;
    else
        lights[1].power = Math.PI;
    break;
    case 51:
    if(lights[2].power > 0)
        lights[2].power = 0;
    else
        lights[2].power = Math.PI;
    break;
    case 52:
    if(lights[3].power > 0)
        lights[3].power = 0;
    else
        lights[3].power = Math.PI;
    break;
    case 53:
    camera = camera2;
    break;
    case 54:
    camera = camera1;
    break;
    case 81:
    case 113:
    if(directionalLight.intensity > 0)
        directionalLight.intensity = 0;
    else
        directionalLight.intensity = 1;
    break;
    case 87:
    case 119:
    keyW();
    break;
    case 69:
    case 101:
    keyE();
    break;
 }
}

function keyW() {

  if(active == 0 && pressedE == 0){
    active = 2;
    for(var i = 0; i < numObjects; i++){
      objects[i].material = phongMaterials[i];
    }
  }

  else if(active == 0 && pressedE == 1){
    active = 1;
    for(var i = 0; i < numObjects; i++){
      objects[i].material = lambertMaterials[i];
    }
  }

  else if(active == 1 || active == 2){
    active = 0;
    for(var i = 0; i < numObjects; i++){
      objects[i].material = basicMaterials[i];
    }
  }

}

function keyE(){

  if(active == 0){

    if(pressedE == 0){
      pressedE = 1;
    }

    else if(pressedE == 1){
      pressedE = 0;
    }

  }

  else if(active != 0){

    if(pressedE == 0){
      pressedE = 1;
      for(var i = 0; i < numObjects; i++){
        objects[i].material = lambertMaterials[i];
      }
    }

    else if(pressedE == 1){
      pressedE = 0;
      for(var i = 0; i < numObjects; i++){
        objects[i].material = phongMaterials[i];
      }
    }

  }


}

function init() {
    'use strict';
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
    icosaedro.rotateY(-Math.PI * 0.005);
    render();
    requestAnimationFrame(animate);
}

function update() {
    'use strict';
    deltaTime = clock.getDelta();
}
