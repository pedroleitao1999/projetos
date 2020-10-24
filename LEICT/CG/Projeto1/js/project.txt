var camera, scene, renderer, viewSize, aspectRatio, robot, target, area;

var ball, clock, velocity, deltaTime, angle, direction;

var pressed_left, pressed_right, pressed_up, pressed_down;

var pressed_a, pressed_s, pressed_q, pressed_w;

function createScene() {
    'use strict';
    scene = new THREE.Scene();
    area = new THREE.Group();
    createRobot();
    createTarget();
    area.add(robot);
    area.add(target);
    scene.add(area);
}

function createSideCamera() {
    'use strict';
    viewSize = 90;
    aspectRatio = window.innerWidth / window.innerHeight;
    camera = new THREE.OrthographicCamera(- aspectRatio * viewSize / 2,
                                        aspectRatio * viewSize / 2,
                                        viewSize / 2,
                                        - viewSize / 2,
                                        -1000, 1000);
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 50;
    camera.lookAt(scene.position);
    scene.add(camera);
}

function createTopCamera() {
    'use strict';
    viewSize = 90;
    aspectRatio = window.innerWidth / window.innerHeight;
    camera = new THREE.OrthographicCamera(- aspectRatio * viewSize / 2,
                                        aspectRatio * viewSize / 2,
                                        viewSize / 2,
                                        - viewSize / 2,
                                        -1000, 1000);
    camera.position.x = 0;
    camera.position.y = 50;
    camera.position.z = 0;
    camera.lookAt(scene.position);
    scene.add(camera);
}

function createFrontCamera() {
    'use strict';
    viewSize = 90;
    aspectRatio = window.innerWidth / window.innerHeight;
    camera = new THREE.OrthographicCamera(- aspectRatio * viewSize / 2,
                                        aspectRatio * viewSize / 2,
                                        viewSize / 2,
                                        - viewSize / 2,
                                        -1000, 1000);
    camera.position.x = 50;
    camera.position.y = 0;
    camera.position.z = 0;
    camera.lookAt(scene.position);
    scene.add(camera);
}


function onResize() {
    'use strict';
    renderer.setSize(window.innerWidth, window.innerHeight);
    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
    }
}

function onKeyDown(e) {
    'use strict';
    switch (e.keyCode) {
        case 49:
            createTopCamera();
            break;
        case 50:
            createSideCamera();
            break;
        case 51:
            createFrontCamera();
            break;
        case 52:
            area.traverse(function (node) {
                if (node instanceof THREE.Mesh) {
                    node.material.wireframe = !node.material.wireframe;
                }
            });
            break;
        case 37:
            pressed_left = true;
            break;
        case 39:
            pressed_right = true;
            break;
        case 38:
            pressed_up = true;
            break;
        case 40:
            pressed_down = true;
            break;
        case 65:
        case 97:
            pressed_a = true;
            break;
        case 83:
        case 115:
            pressed_s = true;
            break;
        case 81:
        case 113:
            pressed_q = true;
            break;
        case 87:
        case 119:
            pressed_w = true;
            break;
    }
}

function onKeyUp(e) {
    'use strict';
    switch (e.keyCode) {
      case 37:
        pressed_left = false;
        direction.x = 0;
        break;
      case 39:
        pressed_right = false;
        direction.x = 0;
        break;
      case 38:
        pressed_up = false;
        direction.z = 0;
        break;
      case 40:
        pressed_down = false;
        direction.z = 0;
        break;
      case 65:
      case 97:
        pressed_a = false;
        break;
      case 83:
      case 115:
        pressed_s = false;
        break;
      case 81:
      case 113:
        pressed_q = false;
        break;
      case 87:
      case 119:
        pressed_w = false;
        break;
    }
}

function render() {
    'use strict';
    renderer.render(scene, camera);
}

function init() {
    'use strict';
    angle = 0;
    clock = new THREE.Clock(true);
    direction = new THREE.Vector3(0, 0, 0);
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createSideCamera();

    render();

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
}

function animate() {
    'use strict';
    deltaTime = clock.getDelta();
    update();
    render();
    requestAnimationFrame(animate);
}

function update() {
    'use strict';
    if(pressed_left && !pressed_right){
      if(pressed_up && !pressed_down){
        robot.position.x -= Math.cos(45) + Math.cos(45)*deltaTime;
        robot.position.z -= Math.cos(45) + Math.cos(45)*deltaTime;
      }
      else if(pressed_down && !pressed_up){
        robot.position.x -= Math.cos(45) + Math.cos(45)*deltaTime;
        robot.position.z += Math.cos(45) + Math.cos(45)*deltaTime;
      }
      else{
        robot.position.x -= 1 + deltaTime;
      }
    }
    else if(pressed_right && !pressed_left){
      if(pressed_up && !pressed_down){
        robot.position.x += Math.cos(45) + Math.cos(45)*deltaTime;
        robot.position.z -= Math.cos(45) + Math.cos(45)*deltaTime;
      }
      else if(pressed_down && !pressed_up){
        robot.position.x += Math.cos(45) + Math.cos(45)*deltaTime;
        robot.position.z += Math.cos(45) + Math.cos(45)*deltaTime;
      }
      else{
        robot.position.x += 1 + deltaTime;
      }
    }
    else if(pressed_up && !pressed_down){
      robot.position.z -= 1;
    }
    else if(pressed_down && !pressed_up){
      robot.position.z += 1;
    }
    if(pressed_a && !pressed_s){
          arm.rotation.y += Math.PI/90+Math.PI/90*deltaTime;
    }
    else if(pressed_s && !pressed_a){
          arm.rotation.y -= Math.PI/90+Math.PI/90*deltaTime;
    }
    if(pressed_q && !pressed_w && angle < 18){
        arm.rotation.z += Math.PI/90+Math.PI/90*deltaTime;
        angle++;
    }
    else if(pressed_w && !pressed_q && angle > -18){
        arm.rotation.z -= Math.PI/90+Math.PI/90*deltaTime;
        angle--;
    }
}
