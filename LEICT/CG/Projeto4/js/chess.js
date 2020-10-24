var room;

var chess, chessHeight = 4, chessSize = 100;

var pointLight, directionalLight, plIntensity = 2, dIntensity = 0.5;

function createChess() {
    'use strict';

    var chessGeometry = new THREE.CubeGeometry(chessSize, chessHeight, chessSize, chessSize / 2, chessHeight / 2, chessSize / 2);

    var texture = new THREE.TextureLoader();
    var board = texture.load("/js/board.jpg");
    var bump = texture.load("/js/bump.jpg");
    var chessBasicMaterial = new THREE.MeshBasicMaterial({map: board});
    var chessStandardMaterial = new THREE.MeshPhongMaterial({map: board, bumpMap: bump, bumpScale: 0.5});


    chess = new THREE.Mesh(chessGeometry, chessStandardMaterial);
    chess.position.set(0, - chessHeight / 2, 0);

    basicMaterials[numObjects] = chessBasicMaterial;
    otherMaterials[numObjects] = chessStandardMaterial;
    objects[numObjects] = chess;
    numObjects++;

    pointLight = new THREE.PointLight(0xffffff, plIntensity, 100);
    pointLight.position.set(chess.position.x, chess.position.y + 90, chess.position.z);

    directionalLight = new THREE.DirectionalLight(0xffffff, dIntensity);
    directionalLight.position.set(chess.position.x, chess.position.y + 50, chess.position.z - 50);

    scene.add(chess);
    scene.add(pointLight);
    scene.add(directionalLight);
}
