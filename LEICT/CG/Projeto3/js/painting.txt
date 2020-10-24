var painting;

var background, backgroundGeometry, bgDimensions = new THREE.Vector3(0.1, 20 + 9 * Math.sqrt(1/2), 20 + 9 * Math.sqrt(1/2));

var backgroundBasicMaterial, backgroundLambertMaterial, backgroundPhongMaterial;

var frame, frameGeometry, frameDimensions = new THREE.Vector3(2, bgDimensions.y + 2, bgDimensions.z + 2);

var frameBasicMaterial, frameLambertMaterial, framePhongMaterial;

var square, squareGeometry, squareDimensions = new THREE.Vector3(0.1, 2, 2);

var squareBasicMaterial, squareLambertMaterial, squarePhongMaterial;

var circle, circleGeometry, circleDimensions = new THREE.Vector3(0.5, 0.5, 0.1);

var circleBasicMaterial, circleLambertMaterial, circlePhongMaterial;

function createPainting() {
    'use strict';

    painting = new THREE.Group();

    backgroundGeometry = new THREE.CubeGeometry(bgDimensions.x, bgDimensions.y, bgDimensions.z, bgDimensions.x / 2, bgDimensions.y / 2, bgDimensions.z / 2);
    frameGeometry = new THREE.CubeGeometry(frameDimensions.x, frameDimensions.y, frameDimensions.z, frameDimensions.x / 2, frameDimensions.y / 2, frameDimensions.z / 2);
    squareGeometry = new THREE.CubeGeometry(squareDimensions.x, squareDimensions.y, squareDimensions.z);
    circleGeometry = new THREE.CylinderGeometry(circleDimensions.x, circleDimensions.y, circleDimensions.z, 12);

    backgroundBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x808080, wireframe: false });
    frameBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x724200, wireframe: false });
    squareBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x000000, wireframe: false });
    circleBasicMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: false });

    backgroundLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x808080, wireframe: false });
    frameLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x724200, wireframe: false });
    squareLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x000000, wireframe: false });
    circleLambertMaterial = new THREE.MeshLambertMaterial({ color: 0xffffff, wireframe: false });

    backgroundPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x808080, wireframe: false });
    framePhongMaterial = new THREE.MeshPhongMaterial({ color: 0x724200, wireframe: false });
    squarePhongMaterial = new THREE.MeshPhongMaterial({ color: 0x000000, wireframe: false });
    circlePhongMaterial = new THREE.MeshPhongMaterial({ color: 0xffffff, wireframe: false });

    background = new THREE.Mesh(backgroundGeometry, backgroundBasicMaterial);
    frame = new THREE.Mesh(frameGeometry, frameBasicMaterial);

    objects[numObjects] = background;
    basicMaterials[numObjects] = backgroundBasicMaterial;
    lambertMaterials[numObjects] = backgroundLambertMaterial;
    phongMaterials[numObjects] = backgroundPhongMaterial;
    numObjects = numObjects + 1;

    objects[numObjects] = frame;
    basicMaterials[numObjects] = frameBasicMaterial;
    lambertMaterials[numObjects] = frameLambertMaterial;
    phongMaterials[numObjects] = framePhongMaterial;
    numObjects = numObjects + 1;

    frame.position.set(wall.position.x + 3.5, wall.position.y, wall.position.z - wallDimensions.z / 4)
    background.position.set(frame.position.x + 1, frame.position.y, frame.position.z);
    painting.add(frame);
    painting.add(background);

    for(var a = 0; a <= 9; a++){
      for(var b = 0; b <= 9; b++){
        square = new THREE.Mesh(squareGeometry, squareBasicMaterial);
        square.position.set(background.position.x,
                           (background.position.y + 0.5 * bgDimensions.y - squareDimensions.y * 0.5) - a * (squareDimensions.y + Math.sqrt(1/2)),
                           (background.position.z + 0.5 * bgDimensions.z - squareDimensions.z * 0.5) - b * (squareDimensions.z + Math.sqrt(1/2)));
        objects[numObjects] = square;
        basicMaterials[numObjects] = squareBasicMaterial;
        lambertMaterials[numObjects] = squareLambertMaterial;
        phongMaterials[numObjects] = squarePhongMaterial;
        numObjects = numObjects + 1;
        painting.add(square);
        if(a == 9 || b == 9) continue;

        circle = new THREE.Mesh(circleGeometry, circleBasicMaterial);
        circle.position.set(background.position.x + 0.1,
                           (background.position.y + 0.5 * bgDimensions.y - (squareDimensions.y + Math.sqrt(1/2) / 2)) - a * (squareDimensions.y + Math.sqrt(1/2)),
                           (background.position.z + 0.5 * bgDimensions.z - (squareDimensions.z + Math.sqrt(1/2) / 2)) - b * (squareDimensions.y + Math.sqrt(1/2)));
        circle.rotation.z = Math.PI/2;
        objects[numObjects] = circle;
        basicMaterials[numObjects] = circleBasicMaterial;
        lambertMaterials[numObjects] = circleLambertMaterial;
        phongMaterials[numObjects] = circlePhongMaterial;
        numObjects = numObjects + 1;
        painting.add(circle);
      }
    }
    scene.add(painting);
}
