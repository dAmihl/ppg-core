#include "catch2/catch.hpp"
#include "PuzzGen.h"

using namespace PPG;

using NodeVec = Vec<Ptr<Node>>;
using NodePair = Pair<Ptr<Node>, Ptr<Node>>;
using RuleVec = Vec<Rule>;
using EventVec = Vec<Ptr<Event>>;
using ObjVec = Vec<Ptr<Object>>;


TEST_CASE("PuzzleGeneratorHelperUnitTests", "[PPG_UNIT_TEST]") {

	GIVEN("a simple Puzzle Relation with 4 nodes and a simple Relation N1->N2->N3") {

		Relation R;
		Generator PG;
		Context c;
		
		auto O1 = c.add<Object>("Object_O1");
		auto S1 = State("State_S1");
		Ptr<Node> N1 = make<Node>(O1, S1);

		auto O2 = c.add<Object>("Object_O2");
		State S2("State_S2");
		Ptr<Node> N2 = make<Node>(O2, S2);

		auto O3 = c.add<Object>("Object_O3");
		State S3("State_S3");
		Ptr<Node> N3 = make<Node>(O3, S3);

		auto O4 = c.add<Object>("Object_O4");
		State S4("State_S4");
		Ptr<Node> N4 = make<Node>(O4, S4);

		NodePair n1n2Pair = makePair(N1, N2);
		NodePair n2n3Pair = makePair(N2, N3);

		R.addPair(n1n2Pair);
		R.addPair(n2n3Pair);
		

		WHEN("A Circular dependency would be created as direct successor") {

			NodePair circularDependencyPair = makePair(N2, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = GeneratorHelper::checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}
		}

		WHEN("A Circular dependency would be created as indirect successor") {

			NodePair circularDependencyPair = makePair(N3, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = GeneratorHelper::checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}
		}

		WHEN("No circular dependency would be created, by adding N3->N4") {

			NodePair circularDependencyPair = makePair(N3, N4);

			THEN("Return true in _checkCreatesCircularDependency()") { 
				bool hasCircularDependency = GeneratorHelper::checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE_FALSE(hasCircularDependency);
			}
		}

		WHEN("Looking for a node in sequential order starting from N1-N2") {

			THEN("Return true because node N3 can be found") {
				REQUIRE(GeneratorHelper::findNodeSequential(N3, n1n2Pair, R));
			}

			THEN("Return true because node N2 can be found in the starting node") {
				REQUIRE(GeneratorHelper::findNodeSequential(N2, n1n2Pair, R));
			}
		}

		WHEN("A exclusive dependency would be created by adding N4->N2 where N4 and N1 have the same object and different states") {

			State S1_2("State_S1_2");
			Ptr<Node> N4 = make<Node>(O1, S1_2);
			NodePair exclusiveDependencyPair = makePair(N4, N2);

			THEN("Return true in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}
		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have different objects but same state") {

			State S1_2("State_S1_2");
			Ptr<Node> N4 = make<Node>(O2, S1_2);
			NodePair exclusiveDependencyPair = makePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE_FALSE(hasExclusiveDependency);
			}
		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have the same object and same state") {

			Ptr<Node> N4 = make<Node>(O1, S1);
			NodePair exclusiveDependencyPair = makePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}
		}

		WHEN("An equal node N4 = N1 with Object of N4 == Object of N1 and State of N4 == State of N1 should be added to the relation") {

			Ptr<Node> N4 = make<Node>(O1, S1);

			THEN("Return true in _checkEquality()") {
				bool isEqualToN1 = GeneratorHelper::checkEquality(N4, N1);
				REQUIRE(isEqualToN1);
			}
		}

		WHEN("An inequal node N4 = N1 with Object of N4 == Object of N1 and State of N4 != State of N1 should be added to the relation") {

			Ptr<Node> N4 = make<Node>(O1, S2);

			THEN("Return false in _checkEquality()") {
				bool isEqualToN1 = GeneratorHelper::checkEquality(N4, N1);
				REQUIRE_FALSE(isEqualToN1);
			}
		}

		WHEN("A meta-equal node N4 = N2 is to be added to R via N4->N1->N2") {

			Ptr<Object> metaEqualO2 = O2;
			State metaEqualS2 = S2;
			Ptr<Node> N4 = make<Node>(metaEqualO2, metaEqualS2);

			NodePair metaEqualPair = makePair(N4, N1);
			R.addPair(metaEqualPair);

			THEN("Return true in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = GeneratorHelper::checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE(hasMetaEqualOccurence);
			}
		}


		WHEN("A non-meta-equal node N4 ?= N2 is to be added to R via N4->N1->N2, where N4 and N2 have different states") {

			Ptr<Object> metaEqualO2 = O2;
			State metaEqualS2("State_Sx");
			Ptr<Node> N4 = make<Node>(metaEqualO2, metaEqualS2);

			NodePair metaEqualPair = makePair(N4, N1);
			R.addPair(metaEqualPair);

			THEN("Return false in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = GeneratorHelper::checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE_FALSE(hasMetaEqualOccurence);
			}
		}

		WHEN("A custom rule is used to restrict the generation: (O4, *) < (O2, *)") {

			Rule rule1(N4->getObjPtr(), STATE_ANY, N2->getObjPtr(), STATE_ANY, Rule::EPuzzleRuleType::BEFORE);

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O4,*) =R= (O4, S4)") {
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1.getLeftHandSideObject(), N4->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1.getLeftHandSideState(), S4);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O2,*) =R= (O2, S2)"){
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1.getRightHandSideObject(), N2->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1.getRightHandSideState(), S2);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual should return false, isRuleStateEqual true (because of wildcard) with (O2,*) =R= (O3, S2)"){
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1.getRightHandSideObject(), N3->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1.getRightHandSideState(), S3);
				REQUIRE_FALSE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}
		}

		WHEN("A list of nodes N1..N5 is given") {
			NodeVec nodes;
			nodes.push_back(N1);
			nodes.push_back(N2);
			nodes.push_back(N3);
			nodes.push_back(N4);
			Ptr<Node> N5 = make<Node>(O1, S2);
			nodes.push_back(N5);
			Ptr<Object> O5 = c.add<Object>("O5"); 

			THEN("findNodesByPattern for Object O1 and S1 should only return N1") {
				NodeVec foundNodes = R.findNodesByPattern(nodes, *O1, S1, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 1);
				REQUIRE(foundNodes.at(0) == N1);
			}

			THEN("findNodesByPattern for Object O1 and * should return N1 and N5") {
				NodeVec foundNodes = R.findNodesByPattern(nodes, *O1, STATE_ANY, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 2);
				bool result = (foundNodes.at(0) == N1) || (foundNodes.at(0) == N5);
				REQUIRE( result );
			}

			THEN("findNodesByPattern for Object O5 and * should return nothing") {
				NodeVec foundNodes = R.findNodesByPattern(nodes, *O5, STATE_ANY, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 0);
			}
		}
	}

	GIVEN("A more extensive puzzle universe with X nodes and rules") {

		// Deterministic generation
		unsigned int seed = 664242;
		// Number of nodes to generate
		unsigned int numberNodes = 20;

		Generator PG(numberNodes);
		Context c;

		Ptr<Object> O1 = c.add<Object>("Object_O1");
		State S1_1("State_S1_1");
		State S1_2("State_S1_2");
		StateTransition T1;
		Ptr<Event> E1 = c.add<Event>("Event_E1", O1);
		T1.addTransition(E1->getEventName(), S1_1, S1_2);
		T1.addTransition(E1->getEventName(), S1_2, S1_1);
		O1->setStateTransition(T1);
		Ptr<Node> N1 = make<Node>(O1, S1_1);

		Ptr<Object> O2 = c.add<Object>("Object_O2");
		State S2_1("State_S2_1");
		State S2_2("State_S2_2");
		StateTransition T2;
		Ptr<Event> E2 = c.add<Event>("Event_E2", O2);
		T2.addTransition(E2->getEventName(), S2_1, S2_2);
		T2.addTransition(E2->getEventName(), S2_2, S2_1);
		O2->setStateTransition(T2);
		Ptr<Node> N2 = make<Node>(O2, S2_1);

		Ptr<Object> O3 = c.add<Object>("Object_O3");
		State S3_1("State_S3_1");
		State S3_2("State_S3_2");
		State S3_3("State_S3_3");
		StateTransition T3;
		Ptr<Event> E3_1 = c.add<Event>("Event_E3_1", O3);
		Ptr<Event> E3_2 = c.add<Event>("Event_E3_2", O3);
		T3.addTransition(E3_1->getEventName(),S3_1, S3_2);
		T3.addTransition(E3_2->getEventName(),S3_2, S3_3);
		O3->setStateTransition(T3);
		Ptr<Node> N3 = make<Node>(O3, S3_1);

		NodePair n1n2Pair = makePair(N1, N2);
		NodePair n2n3Pair = makePair(N2, N3);

		Rule R1(O1, S1_2, O2, S2_1, Rule::EPuzzleRuleType::BEFORE);
		Rule R2(O2, S2_2, O3, STATE_ANY, Rule::EPuzzleRuleType::BEFORE);
		Rule R3(O1, STATE_ANY, O3, STATE_ANY, Rule::EPuzzleRuleType::STRICT_BEFORE);
		Rule R4(O1, STATE_ANY, O3, STATE_ANY, Rule::EPuzzleRuleType::AFTER);
		Rule R5(O2, S2_2, O3, STATE_ANY, Rule::EPuzzleRuleType::STRICT_AFTER);

		//PG.setSeed(seed);		

		auto objects = c.getObjects();
		auto events = c.getEvents();
		auto rules = c.getRules();

		WHEN("A puzzle is generated") {
			auto P = PG.generatePuzzle(c);
			UNSCOPED_INFO("Generated Puzzle:");
			UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
			THEN("It is definitely generated..") {
				REQUIRE(P != nullptr);
			}

		}

		WHEN("A puzzle is generated with the Rule (O2,S2_2) < (O3, *)") {
			rules.clear();
			c.addRule(R2);
			auto P = PG.generatePuzzle(c);
			THEN("Every node (O2, S2_2) has to occur before every other node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O2, S2_2)
					if ((*it)->getRelatedObject() == *O2 && (*it)->getGoalState().getName() == S2_2.getName()) {
						auto checkNotO3 = [](const Node& N) -> auto {return !(N.getRelatedObject().getObjectName() == "Object_O3"); };
						// All smaller nodes than (O2, S2_2) have to be != O3
						result = result && R.checkAllSmaller((*it), checkNotO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R2.getTextualRepresentation());
				UNSCOPED_INFO("Checked nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}
		
		}

		WHEN("A puzzle is generated with the Rule (O1,S1_2) < (O2, S2_1)") {
			rules.clear();
			c.addRule(R1);
			auto P = PG.generatePuzzle(c);

			THEN("Every node (O1, S1_2) has to occur before every other node referencing O2 and Stage S2_1") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, S1_2)
					if ((*it)->getRelatedObject() == *O1 && (*it)->getGoalState().getName() == S1_2.getName()) {
						auto checkNotO2AndS2_1 = [](const Node& N) -> auto {return !(N.getRelatedObject().getObjectName() == "Object_O2" && N.getGoalState().getName() == "State_2_1"); };
						// All smaller nodes than (O1, S1_2) have to be != O2 AND != S2_1
						result = result && R.checkAllSmaller((*it), checkNotO2AndS2_1);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R1.getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}

		}

		WHEN("A puzzle is generated with the Rule (O1,*) <! (O3,*)") {
			rules.clear();
			c.addRule(R3);
			auto P = PG.generatePuzzle(c);

			THEN("Every node (O1,*) has to occur STRICTLY (directly and only) before a node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, *)
					if ((*it)->getRelatedObject() == *O1) {
						auto checkIsO3= [](const Node& N) -> auto {return (N.getRelatedObject().getObjectName() == "Object_O3"); };
						// All directly following nodes of (O1, *) have to  be = O3 
						result = result && R.checkAllFollowing((*it), checkIsO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R3.getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);

			}
		}

		WHEN("A puzzle is generated with the Rule (O1,*) > (O3,*)") {
			rules.clear();
			c.addRule(R4);
			auto P = PG.generatePuzzle(c);

			THEN("Every node (O1,*) has to be after every other node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, *)
					if ((*it)->getRelatedObject() == *O1) {
						auto checkNotO3 = [](const Node& N) -> auto {return !(N.getRelatedObject().getObjectName() == "Object_O3"); };
						// There must not be a node referencing O3 LARGER than O1
						result = result && R.checkAllLarger((*it), checkNotO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R4.getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);

			}
		}


		WHEN("A puzzle is generated with the Rule (O2,S2_2) >! (O3, *)") {
			rules.clear();
			c.addRule(R5);
			auto P = PG.generatePuzzle(c);
			THEN("Every node (O2, S2_2) has to occur STRICTLY (directly and only) after another node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O2, S2_2)
					if ((*it)->getRelatedObject() == *O2 && (*it)->getGoalState().getName() == S2_2.getName()) {
						auto checkIsO3 = [](const Node& N) -> auto {return (N.getRelatedObject().getObjectName() == "Object_O3"); };
						// At least one preceding node has to be referencing O3
						result = result && R.checkAtLeastOnePreceding((*it), checkIsO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R5.getTextualRepresentation());
				UNSCOPED_INFO("Checked nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}

		}


		WHEN("A puzzle is generated without Rules") {
			rules.clear();
			auto P = PG.generatePuzzle(c);
			THEN("getGraphRepresentation() returns the recursive representation of the puzzle starting from the leaf (ending) nodes") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = false;
				
				Vec<GraphNode*> roots = P->getGraphRepresentation();
				GraphNode *leaf = roots.at(0);

				result = leaf != nullptr;
				result = result && leaf->getChildren().size() > 0;
				REQUIRE(result);
			}

		}


	}

}