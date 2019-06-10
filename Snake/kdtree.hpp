#pragma once

#include <list>

#include <glm/glm.hpp>

#include "obj_loader.hpp"
#include "bbox.hpp"

namespace snake {
	struct Node {
		Node * left;
		Node* right;
		Node* parent;
		BBox data;

		//default constructor
		Node(int _n);

		//copy constructor
		Node(int _n, const BBox& _data);

		//deconstructor
		virtual ~Node();

		inline bool isLeaf() const {
			return !left || !right;
		}
	};

	class Kdtree {
	public:
		//typedef float                numType;
		//typedef Node::vecType vecType;
		typedef BBox data;

		//constructor
		Kdtree(const IndexedModel& model);

		//deconstructor
		virtual ~Kdtree() {
			if (root)
				delete root;
		}

		/*
		*   Return the tree root node
		*/
		const Node* getRoot() const { return root; };
		/*
		* support function for printTree
		*/
		//void print_data(data pt);

		/*  prints the tree
		*   and really works best for small trees
		*   as a test of tree construction.
		*/
		//void printTree( Node* head );

		//function for finding the median of a list of points
		void separateByMedian(int axis, std::list<data> &plist, std::list<data> &left,
			std::list<data> &right);
		//function for making the tree
		void makeTree(std::list<data> &plist);

	private:
		//helper for makeTree 
		void _makeTree(Node* head, std::list<data> &plist, int depth);
		Node* root;
	};
}