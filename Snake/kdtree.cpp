#include <iostream>

#include "kdtree.hpp"


namespace snake {
	const static int N = 3;
	const static glm::vec3 left = glm::vec3(1.0f, 0, 0), up = glm::vec3(0, 1.0f, 0), forward = glm::vec3(0, 0, 1.0f);
}

// implementation of Node Class

snake::Node::Node(int _n) : Node(_n, BBox()) {}
snake::Node::Node(int _n, const BBox& _data) : data(_data), left(0), right(0), parent(0) {}

snake::Node::~Node() {
	if (left)
		delete left;

	if (right)
		delete right;
}

// implementation of kdtree

snake::Kdtree::Kdtree(const IndexedModel& model) {
	if (model.isBox) {
		root = new Node(N, BBox(glm::vec3(0), left, up, forward, 1, 1, 1));
	} else {
		std::list<Kdtree::data> plist;

		// Create boxes for all the leaves (points)
		for (auto iter = model.positions.cbegin(); iter != model.positions.cend(); ++iter) {
			// Initial rotation is Aligned with world rotation?
			plist.push_back(BBox(*iter, left, up, forward, 0, 0, 0));
		}

		makeTree(plist);
	}
}

void snake::Kdtree::separateByMedian(int axis,
	std::list<data> &plist,
	std::list<data> &left,
	std::list<data> &right) {

	int size = plist.size();
	int med = ceil( float(size) / float(2) );
	int count = 0;
	
	// Using lambda function here, to define comparison function--parametrized by 'axis'
	plist.sort([&](const data& a, const data& b) -> bool {
		return a.SmallerThanByAxis(axis, b);
	});
	
	for (const data& x : plist) {
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);

		++count;
	}
}

//void Kdtree::print_data(data pt)
//{
	//for( int i = 0; i < N; i++)
	//{
	//    std::cout<< pt[i] << ", ";
	//}
	//std::cout<<"\n";
//}

/*
*
*
*/
//void Kdtree::printTree( Node* head )
//{
//	//find the tree depth 
//	int maxdepth = 3;
//	int spaces = pow(2, maxdepth +1 ) - 1;
//	int depth = 0;
//	
//	std::cout<< "\n**** Print of Tree **********\n";
//	std::queue< Node* > current, next; 
//	Node * temp = head;
//	current.push(temp);
//
//	while( !current.empty() )
//	{
//		temp = current.front();
//		current.pop();
//
//		if (temp == nullptr)
//			std::cout<<"NULL\n";
//		else
//		{
//			Kdtree::print_data(temp->data);
//			next.push(temp->left);
//			next.push(temp->right);
//		}
//		if(current.empty())
//		{
//			depth++;
//			std::cout<< "level: "<<  depth <<"\n";
//			std::swap(current, next);
//		}
//	}
//}

/*
*  algorithm is based on http://en.wikipedia.org/wiki/Kd_tree
*/
void snake::Kdtree::makeTree(std::list<Kdtree::data>& plist) {
	Node* head = new Node(3);
	Kdtree::_makeTree(head, plist, 0);
	Kdtree::root = head;
}

void snake::Kdtree::_makeTree(Node* head, std::list<data>& plist, int depth) {
	// Init children to NULL
	head->left = 0;
	head->right = 0;

	// Shouldn't exist
	if (plist.empty())
		return;

	// A leaf. Keep original point data
	if (plist.size() == 1) {
		head->data = plist.front();
		return;
	}

	int axis = depth % N;

	std::list<data> left_list;
	std::list<data> right_list;
	Kdtree::separateByMedian(axis, plist, left_list, right_list);

	Node* left_node = new Node(N);
	Node* right_node = new Node(N);

	// Set parent as current head
	left_node->parent = head;
	right_node->parent = head;

	Kdtree::_makeTree(left_node, left_list, depth + 1);

	// Relevant node
	if (!left_list.empty())
		head->left = left_node;
	else {
		// Useless
		std::cout << "never left" << std::endl;
		delete left_node;
	}

	Kdtree::_makeTree(right_node, right_list, depth + 1);

	// Relevant node
	if (!right_list.empty())
		head->right = right_node;
	else {
		// Useless
		std::cout << "never right" << std::endl;
		delete right_node;
	}

	// Build own BBox based on children if this is not a leaf!
	if (head->left && head->right) {
		// Select the appropriate corners based on the position of the other box
		float wL = ((head->left->data.position.x <= head->right->data.position.x) ? -1 : 1) * head->left->data.width,
			hL = ((head->left->data.position.y <= head->right->data.position.y) ? -1 : 1) * head->left->data.height,
			dL = ((head->left->data.position.z <= head->right->data.position.z) ? -1 : 1) * head->left->data.depth,
			wR = ((head->right->data.position.x < head->left->data.position.x) ? -1 : 1) * head->right->data.width,
			hR = ((head->right->data.position.y < head->left->data.position.y) ? -1 : 1) * head->right->data.height,
			dR = ((head->right->data.position.z < head->left->data.position.z) ? -1 : 1) * head->right->data.depth;
		glm::vec3 cornerL = glm::vec3(head->left->data.position.x + wL,
									head->left->data.position.y + hL,
									head->left->data.position.z + dL);
		glm::vec3 cornerR = glm::vec3(head->right->data.position.x + wR,
									head->right->data.position.y + hR,
									head->right->data.position.z + dR);
		
		glm::vec3 p = (cornerL + cornerR) * 0.5f;

		// The farthest corners on each axis determine the size of the new box
		// Calculate the distance between them. Keep half
		float w = abs(cornerL.x - cornerR.x) / 2,
			  h = abs(cornerL.y - cornerR.y) / 2, 
			  d = abs(cornerL.z - cornerR.z) / 2;

		// (Points are axis aligned with the local coord system so they're all the same)
		// Use children box positions as two corners inside a larger bounding box
		head->data = BBox(p, head->left->data.xAxis, head->left->data.yAxis, head->left->data.zAxis, w, h, d);
	}
}