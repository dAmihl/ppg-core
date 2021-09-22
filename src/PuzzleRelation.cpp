#include "PuzzleRelation.h"

namespace PPG
{
	NodePairVec PuzzleRelation::getPairs()
	{
		return pairs;
	}

	void PuzzleRelation::addPair(Node* lhs, Node* rhs)
	{
		NodePair newPair = std::make_pair(lhs, rhs);
		this->pairs.push_back(newPair);
	}

	void PuzzleRelation::addPair(NodePair newPair)
	{
		this->pairs.push_back(newPair);
	}

	void PuzzleRelation::removePair(NodePair pair)
	{
		auto found = std::find(this->pairs.begin(), this->pairs.end(), pair);
		if (found != this->pairs.end()) {
			this->pairs.erase(found);
		}
	}

	std::string PuzzleRelation::getSimpleTextualRepresentation()
	{
		std::string out = "";

		for (auto& it: pairs) {
			out += "[" + it.first->getRelatedObject()->getObjectName() + "::" + it.first->getGoalState().getStateName() + "]";
			out += " <<< ";
			out += "[" + it.second->getRelatedObject()->getObjectName() + "::" + it.second->getGoalState().getStateName() + "]";
			out += "\n";
		}

		return out;
	}


	std::string PuzzleRelation::getRecursiveTextualRepresentationOfNode(NodeVec& alreadyOut, std::string* out, Node* N, int level) {

		NodeVec pre = getPrecedingNodes(N);
		NodeVec fol = getFollowingNodes(N);
		std::string tmp = "";
		if (std::find(alreadyOut.begin(), alreadyOut.end(), N) == alreadyOut.end()) {
			for (int hyph = 0; hyph > level; hyph--) {
				tmp += "-";
			}
			tmp += "[(L" + std::to_string(level) + ")" + N->getRelatedObject()->getObjectName() + "::" + N->getGoalState().getStateName() + "]\n";
			alreadyOut.push_back(N);
		}
		(*out) += tmp;

		for (NodeVec::iterator pIt = pre.begin(); pIt != pre.end(); ++pIt) {
			if (std::find(alreadyOut.begin(), alreadyOut.end(), *pIt) == alreadyOut.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOut, out, *pIt, level - 1);
			}
		}

		/*for (NodeVec::iterator fIt = fol.begin(); fIt != fol.end(); ++fIt) {
			if (std::find(alreadyOut.begin(), alreadyOut.end(), *fIt) == alreadyOut.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOut,out, *fIt, level+1);
			}
		}*/

		return (*out);
	}

	std::string PuzzleRelation::getExtendedTextualRepresentation(NodeVec nodes)
	{
		std::string out = "";
		NodeVec alreadyOutputNodes;

		NodeVec leafs = getMaxima(nodes);

		for (NodeVec::iterator it = leafs.begin(); it != leafs.end(); ++it) {

			if (std::find(alreadyOutputNodes.begin(), alreadyOutputNodes.end(), *it) == alreadyOutputNodes.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOutputNodes, &out, *it, 0);
			}
			out += "\n\n";
		}
		out += "\n";
		return out;
	}

	Vec<PuzzleGraphNode*> PuzzleRelation::getGraphRepresentation(NodeVec nodes)
	{
		NodeVec leafs = getMaxima(nodes);
		Vec<PuzzleGraphNode*> rootNodes;

		for (NodeVec::iterator it = leafs.begin(); it != leafs.end(); ++it) {
			PuzzleGraphNode* root = getRecursiveGraphRepresentation(*it);
			rootNodes.push_back(root);
		}

		return rootNodes;
	}

	PuzzleGraphNode* PuzzleRelation::getRecursiveGraphRepresentation(Node* N)
	{
		PuzzleGraphNode* root = new PuzzleGraphNode();
		root->setObject(N->getRelatedObject());
		root->setState(&(N->getRelatedObject()->getCurrentState()));

		NodeVec pre = getPrecedingNodes(N);
		Vec<PuzzleGraphNode*> children;
		for (NodeVec::iterator c = pre.begin(); c != pre.end(); ++c) {
			PuzzleGraphNode* child = getRecursiveGraphRepresentation(*c);
			children.push_back(child);
		}
		root->setChildren(children);
		return root;
	}


	void PuzzleRelation::checkDoForAllPreceding(Node* N, bool(*Check)(Node), void(*Do)(Node*))
	{
		for (auto& it: pairs) {
			if ((it.first) == N) {
				if (Check(*it.first)) {
					Do(it.second);
				}
			}
		}
	}

	void PuzzleRelation::checkDoForAllFollowing(Node* N, bool(*Check)(Node), void(*Do)(Node*))
	{
		for (auto& it: pairs) {
			if (it.first == N) {
				if (Check(*it.first)) {
					Do(it.second);
				}
			}
		}
	}

	bool PuzzleRelation::checkAllLarger(Node* N, bool(*Check)(Node))
	{
		bool result = true;
		for (auto& it : pairs) {
			if (it.first == N) {
				result = result && Check(*it.second) && checkAllLarger(it.second, Check);
			}
		}
		return result;
	}

	bool PuzzleRelation::checkAllSmaller(Node* N, bool(*Check)(Node))
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result && Check(*it.first) && checkAllSmaller(it.first, Check);
			}
		}
		return result;
	}

	bool PuzzleRelation::checkAllFollowing(Node* N, bool(*Check)(Node))
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.first == N) {
				result = result && Check(*it.second);
			}
		}
		return result;
	}

	bool PuzzleRelation::checkAllPreceding(Node* N, bool(*Check)(Node))
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result && Check(*it.first);
			}
		}
		return result;
	}

	bool PuzzleRelation::checkAtLeastOneFollowing(Node* N, bool(*Check)(Node))
	{
		bool result = false;
		for (auto& it: pairs) {
			if (it.first == N) {
				result = result || Check(*it.second);
			}
		}
		return result;
	}

	bool PuzzleRelation::checkAtLeastOnePreceding(Node* N, bool(*Check)(Node))
	{
		bool result = false;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result || Check(*it.first);
			}
		}
		return result;
	}

	NodePairVec PuzzleRelation::getNextPairs(NodePair P)
	{
		NodePairVec nextPairs;

		for (auto& it: pairs) {
			if (P.second == it.first) {
				nextPairs.push_back(it);
			}
		}

		return nextPairs;
	}

	NodePairVec PuzzleRelation::getParallelPairs(NodePair P)
	{
		NodePairVec parallelPairs;

		for (auto& it: pairs) {
			if (it == P) continue; // same node
			if (P.second == it.second) {
				parallelPairs.push_back(it);
			}
		}

		return parallelPairs;
	}

	bool PuzzleRelation::hasPrecedingNode(Node* N)
	{
		for (auto& it: pairs) {
			if (it.second == N) {
				return true;
			}
		}
		return false;
	}

	bool PuzzleRelation::hasFollowingNode(Node* N)
	{
		for (auto& it: pairs) {
			if (it.first == N) {
				return true;
			}
		}
		return false;
	}

	NodeVec PuzzleRelation::getMinima(NodeVec nodes)
	{
		NodeVec mins;

		for (auto& it: nodes) {
			bool isMin = !hasPrecedingNode(it);
			if (isMin) mins.push_back(it);
		}
		return mins;
	}

	NodeVec PuzzleRelation::getMaxima(NodeVec nodes)
	{
		NodeVec maxs;

		for (auto& it: nodes) {
			bool isMax = !hasFollowingNode(it);
			if (isMax) maxs.push_back(it);
		}
		return maxs;
	}



	NodeVec PuzzleRelation::getSmallestNodesFromList(NodeVec nodes)
	{
		NodeVec mins;

		for (auto& it : nodes) {
			bool isMin = true;
			for (auto& chck: nodes) {
				if (it == chck) continue;
				isMin = isMin && !findPrecedingNode(it, chck);
			}
			if (isMin) mins.push_back(it);
		}
		return mins;
	}

	NodeVec PuzzleRelation::getLargestNodesFromList(NodeVec nodes)
	{
		NodeVec maxs;

		for (auto& it: nodes) {
			bool isMax = true;
			for (auto& chck: nodes) {
				if (it == chck) continue;
				isMax = isMax && !findFollowingNode(it, chck);
			}
			if (isMax) maxs.push_back(it);
		}

		return maxs;
	}

	bool PuzzleRelation::findPrecedingNode(Node* start, Node* nodeToFind)
	{
		return findPrecedingNode(start, nodeToFind, false);
	}

	bool PuzzleRelation::findFollowingNode(Node* start, Node* nodeToFind)
	{
		return findFollowingNode(start, nodeToFind, false);
	}

	bool PuzzleRelation::findPrecedingNode(Node* start, Node* nodeToFind, bool includeStart)
	{
		bool result = false;
		if (includeStart) {
			if (start == nodeToFind) return true;
		}

		NodeVec pre = getPrecedingNodes(start);

		if (pre.empty()) return false;

		for (auto& it: pre) {
			if (it == nodeToFind) {
				return true;
			}
			else {
				result = findPrecedingNode(it, nodeToFind, includeStart);
				if (result) return true;
			}
		}

		return result;
	}

	bool PuzzleRelation::findFollowingNode(Node* start, Node* nodeToFind, bool includeStart)
	{
		bool result = false;
		if (includeStart) {
			if (start == nodeToFind) return true;
		}

		NodeVec fol = getFollowingNodes(start);

		if (fol.empty()) return false;


		for (auto& it: fol) {
			if (it == nodeToFind) {
				return true;
			}
			else {
				result = findFollowingNode(it, nodeToFind, includeStart);
				if (result) return true;
			}
		}

		return result;
	}

	bool PuzzleRelation::findDirectlyPrecedingNode(Node* start, Node* nodeToFind)
	{
		NodeVec pre = getPrecedingNodes(start);

		if (pre.empty()) return false;

		for (auto& it: pre) {
			if (it == nodeToFind) {
				return true;
			}
		}

		return false;
	}

	bool PuzzleRelation::findDirectlyFollowingNode(Node* start, Node* nodeToFind)
	{
		NodeVec fol = getFollowingNodes(start);

		if (fol.empty()) return false;

		for (auto& it: fol) {
			if (it == nodeToFind) {
				return true;
			}
		}

		return false;
	}

	NodeVec PuzzleRelation::findNodesByPattern(NodeVec nodes, PuzzleObject* O, PuzzleState* S, bool(*EqualObject)(PuzzleObject*, PuzzleObject*), bool(*EqualState)(PuzzleState*, PuzzleState*))
	{
		NodeVec foundNodes;

		// BIG TODO
		for (auto& it : nodes) {
			if (EqualObject(O, it->getRelatedObject()) && EqualState(S, &it->getGoalState())) {
				foundNodes.push_back(it);
			}
		}

		return foundNodes;
	}



	NodeVec PuzzleRelation::findNearestPrecedingEqualNodesByObject(Node* N)
	{
		return findNearestPrecedingEqualNodesByObject(N, N);
	}

	NodeVec PuzzleRelation::findNearestPrecedingEqualNodesByObject(Node* N, Node* start) {
		NodeVec preNodes = getPrecedingNodes(start);

		NodeVec metas;

		for (auto& prev: preNodes) {
			if (prev->getRelatedObject() == N->getRelatedObject()) {
				metas.push_back(prev);
				continue;
			}
			else {
				NodeVec recNodes = findNearestPrecedingEqualNodesByObject(N, prev);
				metas.insert(metas.end(), recNodes.begin(), recNodes.end());
			}
		}

		return metas;
	}



	NodeVec PuzzleRelation::findNearestFollowingEqualNodesByObject(Node* N)
	{
		return findNearestFollowingEqualNodesByObject(N, N);
	}


	NodeVec PuzzleRelation::findNearestFollowingEqualNodesByObject(Node* N, Node* start)
	{
		NodeVec nextNodes = getFollowingNodes(start);

		NodeVec metas;

		for (auto& next: nextNodes) {
			if (next->getRelatedObject() == N->getRelatedObject()) {
				metas.push_back(next);
				continue;
			}
			else {
				NodeVec recNodes = findNearestFollowingEqualNodesByObject(N, next);
				metas.insert(metas.end(), recNodes.begin(), recNodes.end());
			}
		}

		return metas;
	}

	NodeVec PuzzleRelation::filterCompatibleNodesByRules(NodeVec nodes, Vec<PuzzleRule> rules)
	{
		NodeVec compatibles = nodes;
		return compatibles;
	}


	NodeVec PuzzleRelation::getPrecedingNodes(Node* N)
	{
		NodeVec precedingNodes;

		for (auto& it: pairs) {
			if (it.second == N) {
				precedingNodes.push_back(it.first);
			}
		}

		return precedingNodes;
	}

	NodeVec PuzzleRelation::getFollowingNodes(Node* N)
	{
		NodeVec followingNodes;

		for (auto& it: pairs) {
			if (it.first == N) {
				followingNodes.push_back(it.second);
			}
		}

		return followingNodes;
	}

	/*
	*	Static: Pair Factory
	*/
	NodePair PuzzleRelation::makePuzzlePair(Node* N1, Node* N2)
	{
		NodePair newPair = std::make_pair(N1, N2);
		return newPair;
	}

}