#include "rayAccelerator.h"
#include "macros.h"

using namespace std;

BVH::BVHNode::BVHNode(void) {}

void BVH::BVHNode::setAABB(AABB& bbox_) { this->bbox = bbox_; }

void BVH::BVHNode::makeLeaf(unsigned int index_, unsigned int n_objs_) {
	this->leaf = true;
	this->index = index_;
	this->n_objs = n_objs_;
}

void BVH::BVHNode::makeNode(unsigned int left_index_) {
	this->leaf = false;
	this->index = left_index_;
	//this->n_objs = n_objs_; 
}


BVH::BVH(void) {}

int BVH::getNumObjects() { return objects.size(); }


void BVH::Build(vector<Object*>& objs) {

	BVHNode* root = new BVHNode();

	Vector min = Vector(FLT_MAX, FLT_MAX, FLT_MAX), max = Vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	AABB world_bbox = AABB(min, max);

	for (Object* obj : objs) {
		AABB bbox = obj->GetBoundingBox();
		world_bbox.extend(bbox);
		objects.push_back(obj);
	}
	world_bbox.min.x -= EPSILON; world_bbox.min.y -= EPSILON; world_bbox.min.z -= EPSILON;
	world_bbox.max.x += EPSILON; world_bbox.max.y += EPSILON; world_bbox.max.z += EPSILON;
	root->setAABB(world_bbox);
	nodes.push_back(root);
	build_recursive(0, objects.size(), root); // -> root node takes all the 
}

void BVH::build_recursive(int left_index, int right_index, BVHNode* node) {
	// right_index, left_index and split_index refer to the indices in the objects vector
    // do not confuse with left_nodde_index and right_node_index which refer to indices in the nodes vector. 
	// node.index can have a index of objects vector or a index of nodes vector

	// number of objs is fewer than threshold -> node is leaf Node
	if (right_index - left_index <= Threshold) node->makeLeaf(left_index, right_index - left_index);
	else {

		// find largest axis: axis with largest range of centroids. 
		int largestAxis; 
		float xMin = FLT_MAX, xMax = -FLT_MAX;
		float yMin = FLT_MAX, yMax = -FLT_MAX;
		float zMin = FLT_MAX, zMax = -FLT_MAX;
		for (int i = left_index; i < right_index; i++) {
			if (this->objects[i]->GetBoundingBox().centroid().x > xMax) {
				xMax = this->objects[i]->GetBoundingBox().centroid().x;
			}
			if (this->objects[i]->GetBoundingBox().centroid().y > yMax) {
				yMax = this->objects[i]->GetBoundingBox().centroid().y;
			}
			if (this->objects[i]->GetBoundingBox().centroid().z > zMax) {
				zMax = this->objects[i]->GetBoundingBox().centroid().z;
			}

			if (this->objects[i]->GetBoundingBox().centroid().x < xMin) {
				xMin = this->objects[i]->GetBoundingBox().centroid().x;
			}
			if (this->objects[i]->GetBoundingBox().centroid().y < yMin) {
				yMin = this->objects[i]->GetBoundingBox().centroid().y;
			}
			if (this->objects[i]->GetBoundingBox().centroid().z < zMin) {
				zMin = this->objects[i]->GetBoundingBox().centroid().z;
			}
		}

		if ((xMax - xMin) > (yMax - yMin) && (xMax - xMin) > (zMax - zMin)) {
			largestAxis = 0;
		}
		else if ((xMax - xMin) < (yMax - yMin) && (yMax - yMin) > (zMax - zMin)) {
			largestAxis = 1;
		}
		else {
			largestAxis = 2;
		}

		Comparator cmp;
		cmp.dimension = largestAxis;
		sort(objects.begin() + left_index, objects.begin() + right_index, cmp);

		// find mid point & split_index where point divides into left and right
		double midPoint;
		int split_index;
		midPoint = node->getAABB().centroid().getAxisValue(largestAxis) ;

		// check if left or right are empty. If they are, find midPoint based on mean splitting.
		if ((this->objects[left_index]->GetBoundingBox().centroid().getAxisValue(largestAxis) > midPoint)||
			(this->objects[right_index - 1]->GetBoundingBox().centroid().getAxisValue(largestAxis) <= midPoint)) {
			midPoint = 0;
			for (int i = left_index; i < right_index; i++) {
				midPoint += this->objects[i]->GetBoundingBox().centroid().getAxisValue(largestAxis);
			}
			midPoint = midPoint / (right_index - left_index);
		} 

		if ((this->objects[left_index]->GetBoundingBox().centroid().getAxisValue(largestAxis) > midPoint) ||
			(this->objects[right_index - 1]->GetBoundingBox().centroid().getAxisValue(largestAxis) <= midPoint)) {
			split_index = left_index + Threshold;
		}
		else {
			for (int i = left_index; i < right_index; i++) {
				if (this->objects[i]->GetBoundingBox().centroid().getAxisValue(largestAxis) > midPoint) {
					split_index = i;
					break;
				}
			}
		}
		
		// build left node, right node
		BVHNode* left = new BVHNode();
		BVHNode* right = new BVHNode();
		Vector min = Vector(FLT_MAX, FLT_MAX, FLT_MAX), max = Vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		AABB bboxLeft = AABB(min, max);
		AABB bboxRight = AABB(min, max);

		for (int i = left_index; i < split_index; i++) {
			AABB bbox = this->objects[i]->GetBoundingBox();
			bboxLeft.extend(bbox);
		}
		for (int i = split_index; i < right_index; i++) {
			AABB bbox = this->objects[i]->GetBoundingBox();
			bboxRight.extend(bbox);
		}

		left->setAABB(bboxLeft);
		right->setAABB(bboxRight);
		node->makeNode(nodes.size());
		nodes.push_back(left);
		nodes.push_back(right);

		// call build_recursive for left and then for right
		build_recursive(left_index, split_index, left);
		build_recursive(split_index, right_index, right);
	}

}

bool BVH::Traverse(Ray& ray, Object** hit_obj, Vector& hit_point) {
	float tmp;

	bool leftHit;
	bool rightHit;
	float tLeft;
	float tRight;

	float tmin = FLT_MAX;  //contains the closest primitive intersection
	bool hit = false;

	BVHNode* currentNode = nodes[0];

	if (!currentNode->getAABB().intercepts(ray, tmp)) {
		return hit;
	}
	while (true) {
		leftHit = false;
		rightHit = false;
		if (!currentNode->isLeaf()) {
			BVHNode* left = nodes[(int)currentNode->getIndex() ];
			BVHNode* right = nodes[(int)currentNode->getIndex() + 1];
			//check intersections on left and right node
			if (left->getAABB().intercepts(ray, tLeft)) {
				currentNode = left;
				leftHit = true;
			}
			if (right->getAABB().intercepts(ray, tRight)) {
				currentNode = right;
				rightHit = true;
			}
			if (leftHit && rightHit) {
				// current node = closest & push furthest to stack 
				if (tLeft < tRight) {
					currentNode = left;
					hit_stack.push(StackItem(right, tRight));
				}
				else {
					currentNode = right;
					hit_stack.push(StackItem(left, tLeft));
				}
				continue;
			}
			else if ( rightHit || leftHit ) continue;
		}
		else {
			//check leaf closest object
			float t;
			for (int i = currentNode->getIndex(); i < currentNode->getIndex() + currentNode->getNObjs(); i++) {
				if (this->objects[i]->intercepts(ray, t) && t < tmin) {
					tmin = t;
					*hit_obj = this->objects[i];
					hit = true;
				}
			}
		}
		//check if any object on hit stack is closer
		bool nodeChanged = false;
		while (!hit_stack.empty()) {
			StackItem popd = hit_stack.top();
			hit_stack.pop();
			if (popd.t < tmin) {
				currentNode = popd.ptr;
				nodeChanged= true;
				break;
			}
		}
		if (nodeChanged) continue;

		if (hit_stack.empty()) {
			if (hit) hit_point = ray.direction * tmin + ray.origin;
			return hit;
		}
	}
}

bool BVH::Traverse(Ray& ray) {  //shadow ray with length
	float tmp;

	double length = ray.direction.length(); //distance between light and intersection point
	ray.direction.normalize();

	bool leftHit;
	bool rightHit;
	float tLeft;
	float tRight;
	BVHNode* currentNode = nodes[0];

	if (!currentNode->getAABB().intercepts(ray, tmp)) {
		return false;
	}
	while (true) {

		leftHit = false;
		rightHit = false;
		if (!currentNode->isLeaf()) {
			BVHNode* left = nodes[(int)currentNode->getIndex()];
			BVHNode* right = nodes[(int)currentNode->getIndex() + 1];
			//check intersections on left and right node

			if (left->getAABB().intercepts(ray, tLeft)) {
				currentNode = left;
				leftHit = true;
			}
			if (right->getAABB().intercepts(ray, tRight)) {
				currentNode = right;
				rightHit = true;
			}
			if (leftHit){
				currentNode = left;
				//if both are intersected right goes to hit stack
				if (rightHit) {
					hit_stack.push(StackItem(right, tRight));
				}
				continue;
			}
			
			else if (rightHit || leftHit) continue;
		}
		else {
			//check for intersection
			float t;
			for (int i = currentNode->getIndex(); i < currentNode->getIndex() + currentNode->getNObjs(); i++) {
				if (this->objects[i]->intercepts(ray, t) ) {
					return true;
				}
			}
		}
		//pop the top node of the stack
		bool nodeChanged = false;
		while (!hit_stack.empty()) {
			StackItem popd = hit_stack.top();
			hit_stack.pop();
			currentNode = popd.ptr;
			nodeChanged = true;
		}
		if (nodeChanged)continue;
		//no nodes left on the stack = no intersections
		if (hit_stack.empty()) {
			return false;
		}
	}

}
