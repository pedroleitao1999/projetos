var sculpture;

var pedestal, pedestalGeometry, pedestalDimensions = new THREE.Vector3(10, 35, 10);

var pedestalBasicMaterial, pedestalLambertMaterial, pedestalPhongMaterial;

var icosaedro, icoGeometry = new THREE.Geometry();

var icoBasicMaterial, icoLambertMaterial, icoPhongMaterial;

var goldenNumber = (1 + Math.sqrt(5)) / 2;

function createSculpture() {
    'use strict';

    sculpture = new THREE.Group();

    pedestalGeometry = new THREE.CubeGeometry(pedestalDimensions.x, pedestalDimensions.y, pedestalDimensions.z, pedestalDimensions.x / 2, pedestalDimensions.y / 2, pedestalDimensions.z / 2);
    pedestalBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x3c3d47, wireframe: false });
    pedestalLambertMaterial = new THREE.MeshPhongMaterial({ color: 0x3c3d47, wireframe: false });
    pedestalPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x3c3d47, wireframe: false });
    pedestal = new THREE.Mesh(pedestalGeometry, pedestalBasicMaterial);
    pedestal.position.set(floor.position.x - floorDimensions.x / 4, floorDimensions.y + pedestalDimensions.y / 2, wall.position.z + wallDimensions.z / 4);

    objects[numObjects] = pedestal;
    basicMaterials[numObjects] = pedestalBasicMaterial;
    lambertMaterials[numObjects] = pedestalLambertMaterial;
    phongMaterials[numObjects] = pedestalPhongMaterial;
    numObjects = numObjects + 1;

    icoGeometry.vertices.push (
      new THREE.Vector3(-1, goldenNumber, 0).multiplyScalar(0.7),
      new THREE.Vector3(1, goldenNumber, 0).multiplyScalar(1.09),
      new THREE.Vector3(-1, -goldenNumber, 0).multiplyScalar(1.08),
      new THREE.Vector3(1, -goldenNumber, 0).multiplyScalar(1.06),
      new THREE.Vector3(0, -1, goldenNumber).multiplyScalar(0.9),
      new THREE.Vector3(0, 1, goldenNumber).multiplyScalar(1.1),
      new THREE.Vector3(0, -1, -goldenNumber).multiplyScalar(0.9),
      new THREE.Vector3(0, 1, -goldenNumber).multiplyScalar(0.95),
      new THREE.Vector3(goldenNumber, 0, -1).multiplyScalar(1.04),
      new THREE.Vector3(goldenNumber, 0, 1).multiplyScalar(1.07),
      new THREE.Vector3(-goldenNumber, 0, -1).multiplyScalar(0.92),
      new THREE.Vector3(-goldenNumber, 0, 1).multiplyScalar(1.05)
    );

    icoGeometry.scale(9, 9, 9);

    icoGeometry.faces.push (
      new THREE.Face3(0, 11, 5),
      new THREE.Face3(0, 5, 1),
      new THREE.Face3(0, 1, 7),
      new THREE.Face3(0, 7, 10),
      new THREE.Face3(0, 10, 11),
      new THREE.Face3(1, 5, 9),
      new THREE.Face3(5, 11, 4),
      new THREE.Face3(11, 10, 2),
      new THREE.Face3(10, 7, 6),
      new THREE.Face3(7, 1, 8),
      new THREE.Face3(3, 9, 4),
      new THREE.Face3(3, 4, 2),
      new THREE.Face3(3, 2, 6),
      new THREE.Face3(3, 6, 8),
      new THREE.Face3(3, 8, 9),
      new THREE.Face3(4, 9, 5),
      new THREE.Face3(2, 4, 11),
      new THREE.Face3(6, 2, 10),
      new THREE.Face3(8, 6, 7),
      new THREE.Face3(9, 8, 1)
    );

    icoGeometry.computeFaceNormals();

    icoBasicMaterial = new THREE.MeshBasicMaterial({ color: 0x00BFFF, wireframe: false });
    icoLambertMaterial = new THREE.MeshLambertMaterial({ color: 0x00BFFF, wireframe: false });
    icoPhongMaterial = new THREE.MeshPhongMaterial({ color: 0x00BFFF, wireframe: false });
    icosaedro = new THREE.Mesh(icoGeometry, icoBasicMaterial);
    icosaedro.position.set(pedestal.position.x, floorDimensions.y + pedestalDimensions.y + 15, pedestal.position.z);

    objects[numObjects] = icosaedro;
    basicMaterials[numObjects] = icoBasicMaterial;
    lambertMaterials[numObjects] = icoLambertMaterial;
    phongMaterials[numObjects] = icoPhongMaterial;
    numObjects = numObjects + 1;

    sculpture.add(icosaedro);
    sculpture.add(pedestal);
    scene.add(sculpture);
}
