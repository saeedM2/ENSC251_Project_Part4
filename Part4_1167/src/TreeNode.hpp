//============================================================================
// Name        : TreeNode.hpp
// Description : Course Project Part 3 (Directory Tree)
// Copyright (c) 2016 School of Engineering Science, Simon Fraser University
//============================================================================

#ifndef SRC_TREENODE_HPP_
#define SRC_TREENODE_HPP_

#include <vector>

#define USE_OSS

#ifndef USE_OSS
#include <iostream>
#define OUT std::cout

#else
#include <sstream>
extern std::ostringstream oss;
#define OUT oss
#endif

namespace ensc251 {
	// using namespace std;

	static unsigned count = 0;

	class TreeNode;

	typedef TreeNode* (TreeNode::*action_func)(void);
	typedef void (TreeNode::*traverse_func)(const TreeNode*);

	class TreeNode
	{
		typedef std::vector<TreeNode*> TreeNodePVect;

		protected:
			TreeNodePVect childPVector;

		public:
			TreeNode() {};
			TreeNode(const TreeNode& nodeToCopy) { 
				for (auto childP: nodeToCopy.childPVector) {
					add_childP(childP->clone());
				}
			}

			virtual TreeNode* clone() const { return new TreeNode(*this); };

			virtual ~TreeNode() {
				OUT << "Destroying TreeNode with " << childPVector.size() << " children."<< std::endl;
				for (auto child : childPVector)
					delete child;
			}

		    void swap(TreeNode & other) // the swap member function (should never fail!)
		    {
		        std::swap(childPVector, other.childPVector);
		    }

		    TreeNode & operator = (TreeNode other) // note: argument passed by value, copy made!
		    {
		        swap(other); // swap *this with other
		        return *this; // by convention, always return *this
		    }// swapped other (original *this) is destroyed, releasing the memory

			void add_childP(TreeNode* child) { childPVector.push_back(child); }
			void add_children(const TreeNodePVect& childPV) { childPVector.insert(childPVector.end(), childPV.begin(), childPV.end()); }
			const TreeNodePVect& get_children() const { return childPVector; }

			TreeNode* traverse_children_post_order(traverse_func tf, action_func af)
			{
			    for(auto childP : childPVector) {
			        (childP->*tf)(this); // traverse child's children using tf
			    }
			    return (this->*af)();
			}

			TreeNode* traverse_children_pre_order(traverse_func tf, action_func af)
			{
				(this->*af)();
				for(auto childP : childPVector)
				{
					(childP->*tf)(this);
				}
				return nullptr;
			}

			TreeNode* traverse_children_in_order(traverse_func tf, action_func af)
			{
				if(childPVector.size() != 2)
				{
					OUT << "Error -- can only do inorder traversal on a node with 2 children" << std::endl;
					return nullptr;
				}
				(childPVector[0]->*tf)(this);
				(this->*af)();
				(childPVector[1]->*tf)(this);
				return nullptr;
			}

			TreeNode* count_action()
			{
				count++;
				return nullptr;
			}

			void count_traverse(const TreeNode*)
			{
				traverse_children_post_order(&TreeNode::count_traverse, &TreeNode::count_action);
			}
			void reset_count() { count = 0; }
			unsigned get_count() const { return count; }

	};
}

#endif /* SRC_TREENODE_HPP_ */
