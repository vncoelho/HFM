#ifndef TP_REPRESENTATION_H_
#define TP_REPRESENTATION_H_

#include <vector>
#include "../../OptFrame/Util/printable.h"

using namespace std;

enum Operation
{
	opLag, opOperator, opPerceptron, opConstant, opNOperations
};

enum ActivationFunction
{
	Heavisde_Low, Heavisde_High, Trapezoid_Low, Trapezoid_High, NActivationFunctions
};

struct Perceptron
{
	ActivationFunction aF; //activation function
	vector<double> rulesWeightsParams; //rule, weight, episilon, etc..
};

//Node characteristics
struct NodeChar
{
	Operation nT;
	int operatorOrExpVariable; //operator or explanatoryVariable, column of the input file
	int K; //Lag, Backshift operator, etc..
	double* PI = nullptr; // PI we go!
	Perceptron* p = nullptr;
};

struct Node
{
	NodeChar nodeChar;
	vector<Node*> children;
	Node* father;

	// default empty constructor
	Node()
	{
		// Luke, I'm ..... Nooooooooo!
		father = nullptr;
	}

	// father constructor
	Node(Node* _father)
	{
		father = _father;
	}

	// copy constructor
	Node(const Node& node)
	{
		father = node.father;
		nodeChar = node.nodeChar;
		for (unsigned i = 0; i < node.children.size(); i++)
			children.push_back(new Node(*node.children[i]));
	}

	// move constructor (C++11 only!)
	Node(Node&& node)
	{
		father = node.father;
		nodeChar = std::move(node.nodeChar);
		children = std::move(node.children);
		// steal from the dying friend... (dirty move!)
//		for (unsigned i = 0; i < node.children.size(); i++)
//			children.push_back(node.children[i]);
		// very important to empty the vector WITHOUT deleting the elements...
		node.children.clear();
	}

	// destructor (recursive destructor call cleans whole tree)
	~Node()
	{
		father = nullptr;
		// destroy dependent children
		for (unsigned i = 0; i < children.size(); i++)
		{
			// delete children
			delete children[i];
			// safety nullptr assign
			children[i] = nullptr;
		}
		// empty vector of garbage references
		children.clear();
	}

	// assignment operator
	Node& operator=(const Node& node)
	{
		// self-check, very important!!
		if (this == &node)
			return *this;
		father = node.father;
		nodeChar = node.nodeChar;
		for (unsigned i = 0; i < children.size(); i++)
			delete children[i];
		children.clear();
		for (unsigned i = 0; i < node.children.size(); i++)
			children.push_back(new Node(*node.children[i]));
		return *this;
	}

	// move assignment operator (C++11 only!)
	Node& operator=(Node&& node)
	{
		// self-check... don't know if needed with move assignment (???)
		if (this == &node)
			return *this;
		father = node.father;
		nodeChar = node.nodeChar;
		for (unsigned i = 0; i < children.size(); i++)
			delete children[i];
		children.clear();
		// steal from dying friend...
		for (unsigned i = 0; i < node.children.size(); i++)
			children.push_back(node.children[i]);
		// clear without delete!
		node.children.clear();
		node.father = nullptr;
		return *this;
	}

	friend ostream & operator<<(ostream & os, const Node& rep)
	{
		os << "=======================================" << endl;
		os << "======PRINTING SOL CUADRANTES CLUSTERS=" << endl;

		os << "=======================================" << endl;

		return os;
	}

	static void rPrintNode(Node* root)
	{

		cout << "(op:" << root->nodeChar.nT << "/";
		cout << root->nodeChar.operatorOrExpVariable << "/";
		if (root->nodeChar.nT == opLag)
			cout << root->nodeChar.K << "/t";
		if (root->nodeChar.nT == opPerceptron)
			cout << root->nodeChar.p->aF<< "/t" << root->nodeChar.p->rulesWeightsParams <<"\t";
		cout << "|c|=" << root->children.size() << "): { ";

		//for (Node* n : root->children){
		for (unsigned i = 0; i < root->children.size(); i++)
		{
			cout << endl;
			rPrintNode(root->children[i]);
		}
		cout << "}  \n";

	}

	static void printNode(Node* root)
	{
		cout << "ROOT NODE: (";
		rPrintNode(root);
		cout << ")";
		/*
		 cout << "My Value is " << root->operation << endl;
		 cout << "My children are ";
		 for (int i = 0; i < root->children.size(); i++){
		 cout << root->children[i]->operation << " ";
		 }
		 cout << endl;
		 for (int i = 0; i < root->children.size(); i++){
		 printNode(root->children[i]);         //call display on all children
		 }
		 cout << endl;
		 */
		cout << endl;
	}

};

typedef Node RepTP;

//

//#include <ostream>
//ostream & operator<< (ostream & s, RepTP){}

#endif /*TP_REPRESENTATION_H_*/

