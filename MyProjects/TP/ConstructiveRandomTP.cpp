#include "ConstructiveRandomTP.h"

using namespace TP;

TPConstructiveRandom::TPConstructiveRandom(ProblemInstance& _pTP, RandGen& _rg, int _nodeMaxChildren, int _treeMaxDepth) :
		pTP(_pTP), rg(_rg), nodeMaxChildren(_nodeMaxChildren), treeMaxDepth(_treeMaxDepth)
{

}

TPConstructiveRandom::~TPConstructiveRandom()
{
}
//
//Node* ConstructiveRandom::fillNode(Node& previousNode, int previousNodePos, int nOperators, int nVariables, int nextMaxDepth)
//{
//	int nROperator = rg.rand(nOperators);
//	int rVariable = rg.rand(nVariables);
//	int operatorOrVariable = rg.rand(2);
//
//	if (operatorOrVariable == 0)
//		previousNode.children.at(previousNodePos)->operation = pTP.operators[nROperator];
//
//	if (operatorOrVariable == 1)
//		previousNode.children.at(previousNodePos)->operation = rVariable;
//
//	int nRLayers = rg.rand(nextMaxDepth);
//	vector<Node*> currentChildren(nRLayers);
//	previousNode.children.at(previousNodePos)->children = currentChildren;
//
//	return previousNode.children.at(previousNodePos);
//}

void TPConstructiveRandom::fillNode(Node& node, int nOperators, int nVariables, int nodeMaxChildren, int treeMaxDepth)
{
	if (treeMaxDepth == 0)
		return;

	int nRandomChildren = 0;
	if (node.nodeChar.nT == opOperator)
	{
		nRandomChildren = 2 * (rg.rand(nodeMaxChildren) + 1);
		node.children = vector<Node*>(nRandomChildren, NULL);
	}

	for (int i = 0; i < nRandomChildren; i++)
	{
		Node* newNode = new Node(&node);
		node.children[i] = newNode;

		int nodeOperation = rg.rand(opNOperations);

		if (nodeOperation == opLag)
		{
			int nROperator = rg.rand(nOperators);
			newNode->nodeChar.nT = opOperator;
			newNode->nodeChar.operatorOrExpVariable = nROperator;
			newNode->nodeChar.K = -1;
//
		}
		if (nodeOperation == opOperator)
		{
			int rVariable = rg.rand(nVariables);
			newNode->nodeChar.nT = opOperator;
			newNode->nodeChar.operatorOrExpVariable = rVariable;
			newNode->nodeChar.K = 0;
		}

		if (nodeOperation == opPerceptron)
		{
			int rVariable = rg.rand(nVariables);
			newNode->nodeChar.nT = opPerceptron;
			newNode->nodeChar.operatorOrExpVariable = rVariable;
			newNode->nodeChar.K = 0;
			ActivationFunction randAF = rg.rand(NActivationFunctions);
			vector<double> rulesWeightsParams(3, 0); // rule, weight and episilon equal to 0
			Perceptron* p = new Perceptron;
			p->aF = randAF;
			p->rulesWeightsParams = rulesWeightsParams;
			newNode->nodeChar.p = p;
		}

		fillNode(*newNode, nOperators, nVariables, nodeMaxChildren, treeMaxDepth - 1);

	}

	return;
}

Solution<RepTP, MY_ADS>* TPConstructiveRandom::generateSolution(double timelimit)
{

//	cout << "welcome to construtive" << endl;
	int nOperators = pTP.operators.size();
	int nExpVariables = pTP.inputs[0].size() - 1; //the last variable is the one we want to know formula

//	cout<<nOperators<<"/"<<nVariables<<endl;
//	getchar();

	int firstOperator = rg.rand(nOperators);
	Node* firstNode = new Node; // or new Node(NULL)
	firstNode->nodeChar.nT = opOperator;
	firstNode->nodeChar.operatorOrExpVariable = firstOperator;
	firstNode->nodeChar.K = -1;

	fillNode(*firstNode, nOperators, nExpVariables, nodeMaxChildren, treeMaxDepth);

//	Node::printNode(firstNode);

	// copy representation to solution
	Solution<RepTP>* sol = new Solution<RepTP>(std::move(*firstNode));
	// destroy original solution
	delete firstNode;
	// return solution
	return sol;
}
