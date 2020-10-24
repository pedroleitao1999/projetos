var camera, scene, renderer;

var camera1, camera2, camera3;

var clock, deltaTime;

var is_on_cooldown;

var cannon_awaiting;

var pressed_left, pressed_right, pressed_space, pressed_r = false;


function getRandom(min, max) {
    'use strict';
    return Math.random() * (max - min) + min;
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();
    createWalls();
    createcannons();
    while(ballsNumber < getRandom(10, 20)) {
        createBall(newBall, getRandom(-78, -14), 0, getRandom(-33, 33));
        var thisBall = turnToVector(ballsNumber - 1);
        for(var i = 0; i < ballsNumber; i++) {
            var otherBall = turnToVector(i);
            if(thisBall.distanceTo(otherBall) <= 8 && i != ballsNumber - 1){
                scene.remove(ballsArray[ballsNumber - 1]);
                ballsNumber -= 1;
            }
        }
    }
}

function createCamera1() {
    'use strict';
    camera1 = new THREE.OrthographicCamera(window.innerWidth / -10,
                                        window.innerWidth / 10,
                                        window.innerHeight / 10,
                                        window.innerHeight / -10,
                                        1, 1000);
    camera1.position.x = 0;
    camera1.position.y = 50;
    camera1.position.z = 0;
    camera1.lookAt(scene.position);
    scene.add(camera1);
}

function createCamera2() {
    'use strict';
    camera2 = new THREE.PerspectiveCamera(80,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    camera2.position.x = cannonArray[1].position.x + 20;
    camera2.position.y = cannonArray[1].position.y + 40;
    camera2.position.z = cannonArray[1].position.z;
    camera2.lookAt(wall4.position);
    scene.add(camera2);
}

function createCamera3() {
    'use strict';
    camera3 = new THREE.PerspectiveCamera(80,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    camera3.position.x = cannonArray[1].position.x + 30;
    camera3.position.y = cannonArray[1].position.y + 40;
    camera3.position.z = cannonArray[1].position.z;
    camera3.lookAt(cannonArray[1].position);
    scene.add(camera3);
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

        camera3.aspect = window.innerWidth / window.innerHeight;
        camera3.updateProjectionMatrix();
    }
}

function onKeyDown(e) {
    'use strict';
    switch (e.keyCode) {
        case 32:
            if(cannon_awaiting != -1) pressed_space = true;
            break;
        case 37:
            if(cannon_awaiting != -1) pressed_left = true;
            break;
        case 39:
            if(cannon_awaiting != -1) pressed_right = true;
            break;
    }
}

function onKeyUp(e) {
    'use strict';
    switch (e.keyCode) {
      case 32:
          pressed_space = false
          break;
      case 37:
          pressed_left = false;
          break;
      case 39:
          pressed_right = false;
          break;
    }
}

function onKeyPress(e) {
    'use strict';

    switch (e.keyCode) {
        case 49:
            camera = camera1;
            break;
        case 50:
            camera = camera2;
            break;
        case 51:
            camera = camera3;
            break;
        case 81:
        case 113:
            cannonSelect(cannonArray[0], 0);
            break;
        case 87:
        case 119:
            cannonSelect(cannonArray[1], 1);
            break;
        case 69:
        case 101:
            cannonSelect(cannonArray[2], 2);
            break;
        case 82:
        case 114:
            if (pressed_r) {
                pressed_r = false;
                for (var i = 0; i < ballsNumber; i++) {
                    axesHelperArray[i].visible = false;
                }
            }
            else {
                pressed_r = true;
                for (i = 0; i < ballsNumber; i++) {
                    axesHelperArray[i].visible = true;
                }
            }
            break;
    }
}

function init() {
    'use strict';
    is_on_cooldown = false;
    cannon_awaiting = -1;
    pressed_left = false;
    pressed_right = false;
    pressed_space = false;
    clock = new THREE.Clock(true);
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCamera1();
    createCamera2();
    createCamera3();
    camera = camera1;
    scene.add(camera);
    render();
    debugWalls(0);
    debugBalls(0);

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
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
    for(var i = 0; i < ballsNumber; i++) {
      checkBallColision(i);
      checkWallColision(i);
      move(i);
      fall(i);
      applyFriction(i);
    }

    if(cannon_awaiting == -1) {
        camera3.position.x = cannonArray[1].position.x + 30;
        camera3.position.y = cannonArray[1].position.y + 40;
        camera3.position.z = cannonArray[1].position.z;
        camera3.lookAt(cannonArray[1].position);
    }
    else if(cannon_awaiting == 1) {
        camera3.position.x = cannonArray[selectedCannon].position.x + 30;
        camera3.position.y = cannonArray[selectedCannon].position.y + 40;
        camera3.position.z = cannonArray[selectedCannon].position.z;
        camera3.lookAt(cannonArray[selectedCannon].position);
    }
    else {
        camera3.position.x = ballsArray[ballsNumber - 1].position.x + 30;
        camera3.position.y = ballsArray[ballsNumber - 1].position.y + 40;
        camera3.position.z = ballsArray[ballsNumber - 1].position.z;
        camera3.lookAt(ballsArray[ballsNumber - 1].position);
    }

    if(pressed_left && pressed_right);
    else if(pressed_left) rotateCannon("left");
    else if(pressed_right) rotateCannon("right");

    if(pressed_space) {
      if(!is_on_cooldown) {
        is_on_cooldown = true;
        setTimeout(function() {is_on_cooldown = false;},
                  250);
        shootCannon();
      }
    }
}
