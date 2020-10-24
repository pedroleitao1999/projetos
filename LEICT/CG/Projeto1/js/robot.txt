var camera, scene, renderer, car, arm, robot;

var geometry, material, mesh;

var ball;

function createRobot() {
		'use strict';

		robot = new THREE.Object3D();

		createBase();
    createArm(0, 5.7, 0);

		robot.add(car);
		robot.add(arm);

		scene.add(robot);
}
