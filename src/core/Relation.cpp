#include "Relation.h"

namespace PPG
{
	NodePairVec Relation::getPairs()
	{
		return pairs;
	}

	void Relation::addPair(Ptr<Node> lhs, Ptr<Node> rhs)
	{
		NodePair newPair = std::make_pair(lhs, rhs);
		pairs.push_back(newPair);
	}

	void Relation::addPair(NodePair newPair)
	{
		pairs.push_back(newPair);
	}

	void Relation::removePair(NodePair& pair)
	{
		auto found = std::find(pairs.begin(), pairs.end(), pair);
		if (found != pairs.end()) {
			pairs.erase(found);
		}
	}

	Str Relation::getSimpleTextualRepresentation() const
	{
		Str out = "";

		for (auto& it: pairs) {
			out += "[" + it.first->getRelatedObject().getObjectName() + "::" + it.first->getGoalState().getName() + "]";
			out += " <<< ";
			out += "[" + it.second->getRelatedObject().getObjectName() + "::" + it.second->getGoalState().getName() + "]";
			out += "\n";
		}

		return out;
	}


	PPG::Str Relation::getRecursiveTextualRepresentationOfNode(Vec<Ptr<Node>>& alreadyOut, Str* out, const Ptr<Node>& N, int level) const {

		NodeVec pre = getPrecedingNodes(N);
		NodeVec fol = getFollowingNodes(N);
		Str tmp = "";
		if (std::find(alreadyOut.begin(), alreadyOut.end(), N) == alreadyOut.end()) {
			for (int hyph = 0; hyph > level; hyph--) {
				tmp += "-";
			}
			tmp += "[(L" + std::to_string(level) + ")" + N->getRelatedObject().getObjectName() + "::" + N->getGoalState().getName() + "]\n";
			alreadyOut.push_back(N);
		}
		(*out) += tmp;

		for (auto& pIt: pre) {
			if (std::find(alreadyOut.begin(), alreadyOut.end(), pIt) == alreadyOut.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOut, out, pIt, level - 1);
			}
		}

		/*for (NodeVec::iterator fIt = fol.begin(); fIt != fol.end(); ++fIt) {
			if (std::find(alreadyOut.begin(), alreadyOut.end(), *fIt) == alreadyOut.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOut,out, *fIt, level+1);
			}
		}*/

		return (*out);
	}

	PPG::Str Relation::getExtendedTextualRepresentation(const NodeVec& nodes) const
	{
		Str out = "";
		Vec<Ptr<Node>> alreadyOutputNodes;

		NodeVec leafs = getMaxima(nodes);

		for (auto& it : leafs) {
			if (std::find(alreadyOutputNodes.begin(), alreadyOutputNodes.end(), it) == alreadyOutputNodes.end()) {
				getRecursiveTextualRepresentationOfNode(alreadyOutputNodes, &out, it, 0);
			}
			out += "\n\n";
		}
		out += "\n";
		return out;
	}

	PPG::Vec<GraphNode*> Relation::getGraphRepresentation(const NodeVec& nodes) const
	{
		NodeVec leafs = getMaxima(nodes);
		Vec<GraphNode*> rootNodes;

		for (auto& it: leafs) {
			GraphNode* root = getRecursiveGraphRepresentation(it);
			rootNodes.push_back(root);
		}

		return rootNodes;
	}

	PPG::GraphNode* Relation::getRecursiveGraphRepresentation(const Ptr<Node>& N) const
	{
		GraphNode* root = new GraphNode(N->getRelatedObject(), N->getRelatedObject().getCurrentState());

		NodeVec pre = getPrecedingNodes(N);
		Vec<GraphNode*> children;
		for (auto& it: pre) {
			GraphNode* child = getRecursiveGraphRepresentation(it);
			children.push_back(child);
		}
		root->setChildren(children);
		return root;
	}

	void Relation::checkDoForAllPreceding(const Ptr<Node>& N, bool(*Check)(const Node&), void(*Do)(Node&))
	{
		for (auto& it: pairs) {
			if ((it.first) == N) {
				if (Check(*it.first)) {
					Do(*it.second);
				}
			}
		}
	}

	void Relation::checkDoForAllFollowing(const Ptr<Node>& N, bool(*Check)(const Node&), void(*Do)(Node&))
	{
		for (auto& it: pairs) {
			if (it.first == N) {
				if (Check(*it.first)) {
					Do(*it.second);
				}
			}
		}
	}

	bool Relation::checkAllLarger(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = true;
		for (auto& it : pairs) {
			if (it.first == N) {
				result = result && Check(*it.second) && checkAllLarger(it.second, Check);
			}
		}
		return result;
	}

	bool Relation::checkAllSmaller(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result && Check(*it.first) && checkAllSmaller(it.first, Check);
			}
		}
		return result;
	}

	bool Relation::checkAllFollowing(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.first == N) {
				result = result && Check(*it.second);
			}
		}
		return result;
	}

	bool Relation::checkAllPreceding(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = true;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result && Check(*it.first);
			}
		}
		return result;
	}

	bool Relation::checkAtLeastOneFollowing(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = false;
		for (auto& it: pairs) {
			if (it.first == N) {
				result = result || Check(*it.second);
			}
		}
		return result;
	}

	bool Relation::checkAtLeastOnePreceding(const Ptr<Node>& N, bool(*Check)(const Node&)) const
	{
		bool result = false;
		for (auto& it: pairs) {
			if (it.second == N) {
				result = result || Check(*it.first);
			}
		}
		return result;
	}

	PPG::NodePairVec Relation::getNextPairs(const NodePair& P) const
	{
		NodePairVec nextPairs;

		for (auto& it: pairs) {
			if (P.second == it.first) {
				nextPairs.push_back(it);
			}
		}

		return nextPairs;
	}

	PPG::NodePairVec Relation::getParallelPairs(const NodePair& P) const
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

	bool Relation::hasPrecedingNode(const Ptr<Node>& N) const
	{
		for (auto& it: pairs) {
			if (it.second == N) {
				return true;
			}
		}
		return false;
	}

	bool Relation::hasFollowingNode(const Ptr<Node>& N) const
	{
		for (auto& it: pairs) {
			if (it.first == N) {
				return true;
			}
		}
		return false;
	}

	NodeVec Relation::getMinima(const NodeVec& nodes) const
	{
		NodeVec mins;

		for (auto& it: nodes) {
			bool isMin = !hasPrecedingNode(it);
			if (isMin) mins.push_back(it);
		}
		return mins;
	}

	NodeVec Relation::getMaxima(const NodeVec& nodes) const
	{
		NodeVec maxs;

		for (auto& it: nodes) {
			bool isMax = !hasFollowingNode(it);
			if (isMax) maxs.push_back(it);
		}
		return maxs;
	}



	NodeVec Relation::getSmallestNodesFromList(const NodeVec& nodes) const
	{
		NodeVec mins;

		for (const auto& it : nodes) {
			bool isMin = true;
			for (const auto& chck: nodes) {
				if (it == chck) continue;
				isMin = isMin && !findPrecedingNode(it, chck);
			}
			if (isMin) mins.push_back(it);
		}
		return mins;
	}

	NodeVec Relation::getLargestNodesFromList(const NodeVec& nodes) const
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

	bool Relation::findPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const
	{
		return findPrecedingNode(start, nodeToFind, false);
	}

	bool Relation::findFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const
	{
		return findFollowingNode(start, nodeToFind, false);
	}

	bool Relation::findPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind, bool includeStart) const
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

	bool Relation::findFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind, bool includeStart) const
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

	bool Relation::findDirectlyPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const
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

	bool Relation::findDirectlyFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const
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


	NodeVec Relation::findNearestPrecedingEqualNodesByObject(const Ptr<Node>& N) const
	{
		return findNearestPrecedingEqualNodesByObject(N, N);
	}

	NodeVec Relation::findNearestPrecedingEqualNodesByObject(const Ptr<Node>& N, const Ptr<Node>& start) const 
	{
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

	NodeVec Relation::findNearestFollowingEqualNodesByObject(const Ptr<Node>& N) const
	{
		return findNearestFollowingEqualNodesByObject(N, N);
	}

	NodeVec Relation::findNearestFollowingEqualNodesByObject(const Ptr<Node>& N, const Ptr<Node>& start) const
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


	NodeVec Relation::getPrecedingNodes(const Ptr<Node>& N) const
	{
		NodeVec precedingNodes;

		for (auto& it: pairs) {
			if (it.second == N) {
				precedingNodes.push_back(it.first);
			}
		}

		return precedingNodes;
	}

	NodeVec Relation::getFollowingNodes(const Ptr<Node>& N) const
	{
		NodeVec followingNodes;

		for (auto& it: pairs) {
			if (it.first == N) {
				followingNodes.push_back(it.second);
			}
		}

		return followingNodes;
	}


}