#include "PuzzleRelation.h"


PuzzleRelation::PuzzleRelation()
{
}


PuzzleRelation::~PuzzleRelation()
{
}

T_PuzzlePairList PuzzleRelation::getPairs()
{
	return this->pairs;
}

void PuzzleRelation::addPair(PuzzleNode* lhs, PuzzleNode* rhs)
{
	T_PuzzleNodePair newPair = std::make_pair(lhs, rhs);
	this->pairs.push_back(newPair);
}

void PuzzleRelation::addPair(T_PuzzleNodePair newPair)
{
	this->pairs.push_back(newPair);
}

void PuzzleRelation::removePair(T_PuzzleNodePair pair)
{
	T_PuzzlePairList::iterator found = std::find(this->pairs.begin(), this->pairs.end(), pair);
	if (found != this->pairs.end()) {
		this->pairs.erase(found);
	}
}

std::string PuzzleRelation::getSimpleTextualRepresentation()
{
	std::string out = "";
	
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		out += "[" + it->first->getRelatedObject()->getObjectName() + "::" + it->first->getGoalState().getStateName() + "]";
		out += " <<< ";
		out += "[" + it->second->getRelatedObject()->getObjectName() + "::" + it->second->getGoalState().getStateName() + "]";
		out += "\n";
	}
	
	return out;
}


std::string PuzzleRelation::getRecursiveTextualRepresentationOfNode(T_PuzzleNodeList &alreadyOut, std::string* out, PuzzleNode* N, int level) {
	
	T_PuzzleNodeList pre = getPrecedingNodes(N);
	T_PuzzleNodeList fol = getFollowingNodes(N);
	std::string tmp = "";
	if (std::find(alreadyOut.begin(), alreadyOut.end(), N) == alreadyOut.end()) {
		for (int hyph = 0; hyph > level; hyph--) {
			tmp += "-";
		}
		tmp += "[(L" + std::to_string(level) + ")" + N->getRelatedObject()->getObjectName() + "::" + N->getGoalState().getStateName() + "]\n";
		alreadyOut.push_back(N);
	}
	(*out) += tmp;

	for (T_PuzzleNodeList::iterator pIt = pre.begin(); pIt != pre.end(); ++pIt) {
		if (std::find(alreadyOut.begin(), alreadyOut.end(), *pIt) == alreadyOut.end()) {
			getRecursiveTextualRepresentationOfNode(alreadyOut,out, *pIt, level-1);
		}
	}
	
	/*for (T_PuzzleNodeList::iterator fIt = fol.begin(); fIt != fol.end(); ++fIt) {
		if (std::find(alreadyOut.begin(), alreadyOut.end(), *fIt) == alreadyOut.end()) {
			getRecursiveTextualRepresentationOfNode(alreadyOut,out, *fIt, level+1);
		}
	}*/
	
	return (*out);
}

std::string PuzzleRelation::getExtendedTextualRepresentation(T_PuzzleNodeList nodes)
{
	std::string out = "";
	T_PuzzleNodeList alreadyOutputNodes;

	T_PuzzleNodeList leafs = getMaxima(nodes);

	for (T_PuzzleNodeList::iterator it = leafs.begin(); it != leafs.end(); ++it) {
		
		if (std::find(alreadyOutputNodes.begin(), alreadyOutputNodes.end(), *it) == alreadyOutputNodes.end()) {
			getRecursiveTextualRepresentationOfNode(alreadyOutputNodes, &out, *it, 0);
		}
		out += "\n\n";
	}
	out += "\n";
	return out;
}

T_PuzzleGraphNodeList PuzzleRelation::getGraphRepresentation(T_PuzzleNodeList nodes)
{
	T_PuzzleNodeList leafs = getMaxima(nodes);
	T_PuzzleGraphNodeList rootNodes;

	for (T_PuzzleNodeList::iterator it = leafs.begin(); it != leafs.end(); ++it) {
		PuzzleGraphNode* root = getRecursiveGraphRepresentation(*it);
		rootNodes.push_back(root);
	}

	return rootNodes;
}

PuzzleGraphNode* PuzzleRelation::getRecursiveGraphRepresentation(PuzzleNode* N)
{
	PuzzleGraphNode* root = new PuzzleGraphNode();
	root->setObject(N->getRelatedObject());
	root->setState(&(N->getRelatedObject()->getCurrentState()));

	T_PuzzleNodeList pre = getPrecedingNodes(N);
	T_PuzzleGraphNodeList children;
	for (T_PuzzleNodeList::iterator c = pre.begin(); c != pre.end(); ++c) {
		PuzzleGraphNode* child = getRecursiveGraphRepresentation(*c);
		children.push_back(child);
	}
	root->setChildren(children);
	return root;
}


void PuzzleRelation::checkDoForAllPreceding(PuzzleNode *N, bool(*Check)(PuzzleNode), void(*Do)(PuzzleNode*))
{
	for (std::vector<std::pair<PuzzleNode*, PuzzleNode*>>::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if ((it->first) == N) {
			if (Check(*it->first)) {
				Do(it->second);
			}
		}
	}
}

void PuzzleRelation::checkDoForAllFollowing(PuzzleNode * N, bool(*Check)(PuzzleNode), void(*Do)(PuzzleNode *))
{
	for (std::vector<std::pair<PuzzleNode*, PuzzleNode*>>::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			if (Check(*it->first)) {
				Do(it->second);
			}
		}
	}
}

bool PuzzleRelation::checkAllLarger(PuzzleNode * N, bool(*Check)(PuzzleNode))
{
	bool result = true;
	for (std::vector<std::pair<PuzzleNode*, PuzzleNode*>>::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			result = result && Check(*it->second) && checkAllLarger(it->second, Check);
		}
	}
	return result;
}

bool PuzzleRelation::checkAllSmaller(PuzzleNode * N, bool(*Check)(PuzzleNode))
{
	bool result = true;
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->second == N) {
			result = result && Check(*it->first) && checkAllSmaller(it->first, Check);
		}
	}
	return result;
}

bool PuzzleRelation::checkAllFollowing(PuzzleNode * N, bool(*Check)(PuzzleNode))
{
	bool result = true;
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			result = result && Check(*it->second);
		}
	}
	return result;
}

bool PuzzleRelation::checkAllPreceding(PuzzleNode* N, bool(*Check)(PuzzleNode))
{
	bool result = true;
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->second == N) {
			result = result && Check(*it->first);
		}
	}
	return result;
}

bool PuzzleRelation::checkAtLeastOneFollowing(PuzzleNode* N, bool(*Check)(PuzzleNode))
{
	bool result = false;
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			result = result || Check(*it->second);
		}
	}
	return result;
}

bool PuzzleRelation::checkAtLeastOnePreceding(PuzzleNode* N, bool(*Check)(PuzzleNode))
{
	bool result = false;
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->second == N) {
			result = result || Check(*it->first);
		}
	}
	return result;
}

T_PuzzlePairList PuzzleRelation::getNextPairs(T_PuzzleNodePair P)
{
	T_PuzzlePairList nextPairs;

	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (P.second == it->first) {
			nextPairs.push_back(*it);
		}
	}

	return nextPairs;
}

T_PuzzlePairList PuzzleRelation::getParallelPairs(T_PuzzleNodePair P)
{
	T_PuzzlePairList parallelPairs;

	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if ((*it) == P) continue; // same node
		if (P.second == it->second) {
			parallelPairs.push_back(*it);
		}
	}

	return parallelPairs;
}

bool PuzzleRelation::hasPrecedingNode(PuzzleNode * N)
{
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->second == N) {
			return true;
		}
	}
	return false;
}

bool PuzzleRelation::hasFollowingNode(PuzzleNode * N)
{
	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			return true;
		}
	}
	return false;
}

T_PuzzleNodeList PuzzleRelation::getMinima(T_PuzzleNodeList nodes)
{
	T_PuzzleNodeList mins;

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		bool isMin = !hasPrecedingNode(*it);
		if (isMin) mins.push_back(*it);
	}
	return mins;
}

T_PuzzleNodeList PuzzleRelation::getMaxima(T_PuzzleNodeList nodes)
{
	T_PuzzleNodeList maxs;

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		bool isMax = !hasFollowingNode(*it);
		if (isMax) maxs.push_back(*it);
	}
	return maxs;
}



T_PuzzleNodeList PuzzleRelation::getSmallestNodesFromList(T_PuzzleNodeList nodes)
{
	T_PuzzleNodeList mins;
	
	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		bool isMin = true;
		for (T_PuzzleNodeList::iterator chck = nodes.begin(); chck != nodes.end(); ++chck) {
			if (it == chck) continue;
			isMin = isMin && !findPrecedingNode(*it, *chck);
		}
		if (isMin) mins.push_back(*it);
	}
	return mins;
}

T_PuzzleNodeList PuzzleRelation::getLargestNodesFromList(T_PuzzleNodeList nodes)
{
	T_PuzzleNodeList maxs;

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		bool isMax = true;
		for (T_PuzzleNodeList::iterator chck = nodes.begin(); chck != nodes.end(); ++chck) {
			if (it == chck) continue;
			isMax = isMax && !findFollowingNode(*it, *chck);
		}
		if (isMax) maxs.push_back(*it);
	}

	return maxs;
}

bool PuzzleRelation::findPrecedingNode(PuzzleNode* start, PuzzleNode* nodeToFind)
{
	return findPrecedingNode(start, nodeToFind, false);
}

bool PuzzleRelation::findFollowingNode(PuzzleNode* start, PuzzleNode* nodeToFind)
{
	return findFollowingNode(start, nodeToFind, false);
}

bool PuzzleRelation::findPrecedingNode(PuzzleNode * start, PuzzleNode * nodeToFind, bool includeStart)
{
	bool result = false;
	if (includeStart) {
		if (start == nodeToFind) return true;
	}

	T_PuzzleNodeList pre = getPrecedingNodes(start);

	if (pre.empty()) return false;

	for (T_PuzzleNodeList::iterator it = pre.begin(); it != pre.end(); ++it) {
		if (*it == nodeToFind) {
			return true;
		}
		else {
			result = findPrecedingNode(*it, nodeToFind, includeStart);
			if (result) return true;
		}
	}

	return result;
}

bool PuzzleRelation::findFollowingNode(PuzzleNode * start, PuzzleNode * nodeToFind, bool includeStart)
{
	bool result = false;
	if (includeStart) {
		if (start == nodeToFind) return true;
	}

	T_PuzzleNodeList fol = getFollowingNodes(start);
	
	if (fol.empty()) return false;
	

	for (T_PuzzleNodeList::iterator it = fol.begin(); it != fol.end(); ++it) {
		if (*it == nodeToFind) {
			return true;
		}
		else {
			result = findFollowingNode(*it, nodeToFind, includeStart);
			if (result) return true;
		}
	}

	return result;
}

bool PuzzleRelation::findDirectlyPrecedingNode(PuzzleNode* start, PuzzleNode* nodeToFind)
{
	T_PuzzleNodeList pre = getPrecedingNodes(start);

	if (pre.empty()) return false;

	for (T_PuzzleNodeList::iterator it = pre.begin(); it != pre.end(); ++it) {
		if (*it == nodeToFind) {
			return true;
		}
	}

	return false;
}

bool PuzzleRelation::findDirectlyFollowingNode(PuzzleNode* start, PuzzleNode* nodeToFind)
{
	T_PuzzleNodeList fol = getFollowingNodes(start);

	if (fol.empty()) return false;


	for (T_PuzzleNodeList::iterator it = fol.begin(); it != fol.end(); ++it) {
		if (*it == nodeToFind) {
			return true;
		}
	}

	return false;
}

T_PuzzleNodeList PuzzleRelation::findNodesByPattern(T_PuzzleNodeList nodes, PuzzleObject* O, PuzzleState* S, bool(*EqualObject)(PuzzleObject*, PuzzleObject*), bool(*EqualState)(PuzzleState *, PuzzleState *))
{
	
	T_PuzzleNodeList foundNodes;
		
	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if (EqualObject(O, (*it)->getRelatedObject()) && EqualState(S, &(*it)->getGoalState())) {
			foundNodes.push_back(*it);
		}
	}
		
	return foundNodes;
}



T_PuzzleNodeList PuzzleRelation::findNearestPrecedingEqualNodesByObject(PuzzleNode * N)
{
	return findNearestPrecedingEqualNodesByObject(N, N);
}

T_PuzzleNodeList PuzzleRelation::findNearestPrecedingEqualNodesByObject(PuzzleNode* N, PuzzleNode* start) {
	T_PuzzleNodeList preNodes = getPrecedingNodes(start);

	T_PuzzleNodeList metas;

	for (T_PuzzleNodeList::iterator prev = preNodes.begin(); prev != preNodes.end(); ++prev) {
		if ((*prev)->getRelatedObject() == N->getRelatedObject()) {
			metas.push_back(*prev);
			continue;
		}
		else {
			T_PuzzleNodeList recNodes = findNearestPrecedingEqualNodesByObject(N, *prev);
			metas.insert(metas.end(), recNodes.begin(), recNodes.end());
		}
	}

	return metas;
}



T_PuzzleNodeList PuzzleRelation::findNearestFollowingEqualNodesByObject(PuzzleNode * N)
{
	return findNearestFollowingEqualNodesByObject(N, N);
}


T_PuzzleNodeList PuzzleRelation::findNearestFollowingEqualNodesByObject(PuzzleNode * N, PuzzleNode* start)
{
	T_PuzzleNodeList nextNodes = getFollowingNodes(start);

	T_PuzzleNodeList metas;

	for (T_PuzzleNodeList::iterator next = nextNodes.begin(); next != nextNodes.end(); ++next) {
		if ((*next)->getRelatedObject() == N->getRelatedObject()) {
			metas.push_back(*next);
			continue;
		}
		else {
			T_PuzzleNodeList recNodes = findNearestFollowingEqualNodesByObject(N, *next);
			metas.insert(metas.end(), recNodes.begin(), recNodes.end());
		}
	}

	return metas;
}

T_PuzzleNodeList PuzzleRelation::filterCompatibleNodesByRules(T_PuzzleNodeList nodes, T_PuzzleRuleList rules)
{
	T_PuzzleNodeList compatibles = nodes;
	return compatibles;
}



T_PuzzleNodeList PuzzleRelation::getPrecedingNodes(PuzzleNode * N)
{
	T_PuzzleNodeList precedingNodes;

	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->second == N) {
			precedingNodes.push_back(it->first);
		}
	}

	return precedingNodes;
}

T_PuzzleNodeList PuzzleRelation::getFollowingNodes(PuzzleNode * N)
{
	T_PuzzleNodeList followingNodes;

	for (T_PuzzlePairList::iterator it = this->pairs.begin(); it != this->pairs.end(); ++it) {
		if (it->first == N) {
			followingNodes.push_back(it->second);
		}
	}

	return followingNodes;
}

/*
*	Static: Pair Factory
*/
T_PuzzleNodePair PuzzleRelation::makePuzzlePair(PuzzleNode * N1, PuzzleNode * N2)
{
	T_PuzzleNodePair newPair = std::make_pair(N1, N2);
	return newPair;
}



