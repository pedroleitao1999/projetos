var pauseSize = 50, pauseWidth = 50;

var pause;

function createPause() {
    'use strict';

    var pauseGeometry = new THREE.CubeGeometry(pauseSize, pauseSize, pauseWidth, pauseSize / 2, pauseSize / 2, pauseWidth / 2);

    var texture = new THREE.TextureLoader();
    var pauseTexture = texture.load("/js/pause.jpg");
    var pauseMaterial = new THREE.MeshBasicMaterial({map: pauseTexture});

    pause = new THREE.Mesh(pauseGeometry, pauseMaterial);
    pause.position.set(500, 0, 0);

    scene.add(pause);
}
