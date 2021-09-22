#include "catch2/catch.hpp"
#include "PuzzGen.h"

using namespace PPG;

using NodeVec = Vec<Node*>;
using NodePair = Pair<Node*, Node*>;
using RuleVec = Vec<Rule>;
using EventVec = Vec<Event*>;
using ObjVec = Vec<Object*>;


TEST_CASE("PuzzleGeneratorHelperUnitTests", "[PPG_UNIT_TEST]") {

	GIVEN("a simple Puzzle Relation with 4 nodes and a simple Relation N1->N2->N3") {

		Relation* R = new Relation();

		Generator* PG = new Generator();
		
		Object* O1 = new Object("Object_O1");
		PuzzleState* S1 = new PuzzleState("State_S1");
		Node* N1 = new Node(O1, *S1);

		Object* O2 = new Object("Object_O2");
		PuzzleState* S2 = new PuzzleState("State_S2");
		Node* N2 = new Node(O2, *S2);

		Object* O3 = new Object("Object_O3");
		PuzzleState* S3 = new PuzzleState("State_S3");
		Node* N3 = new Node(O3, *S3);

		Object* O4 = new Object("Object_O4");
		PuzzleState* S4 = new PuzzleState("State_S4");
		Node* N4 = new Node(O4, *S4);

		NodePair n1n2Pair = Relation::makePuzzlePair(N1, N2);
		NodePair n2n3Pair = Relation::makePuzzlePair(N2, N3);

		R->addPair(n1n2Pair);
		R->addPair(n2n3Pair);
		

		WHEN("A Circular dependency would be created as direct successor") {

			NodePair circularDependencyPair = Relation::makePuzzlePair(N2, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = GeneratorHelper::checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("A Circular dependency would be created as indirect successor") {

			NodePair circularDependencyPair = Relation::makePuzzlePair(N3, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = GeneratorHelper::checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("No circular dependency would be created, by adding N3->N4") {

			NodePair circularDependencyPair = Relation::makePuzzlePair(N3, N4);

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

			PuzzleState* S1_2 = new PuzzleState("State_S1_2");
			Node* N4 = new Node(O1, *S1_2);
			NodePair exclusiveDependencyPair = Relation::makePuzzlePair(N4, N2);

			THEN("Return true in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}

		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have different objects but same state") {

			PuzzleState* S1_2 = new PuzzleState("State_S1_2");
			Node* N4 = new Node(O2, *S1_2);
			NodePair exclusiveDependencyPair = Relation::makePuzzlePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE_FALSE(hasExclusiveDependency);
			}

		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have the same object and same state") {

			Node* N4 = new Node(O1, *S1);
			NodePair exclusiveDependencyPair = Relation::makePuzzlePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = GeneratorHelper::checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}

		}

		WHEN("An equal node N4 = N1 with Object of N4 == Object of N1 and State of N4 == State of N1 should be added to the relation") {

			Node* N4 = new Node(O1, *S1);

			THEN("Return true in _checkEquality()") {
				bool isEqualToN1 = GeneratorHelper::checkEquality(N4, N1);
				REQUIRE(isEqualToN1);
			}

		}

		WHEN("An inequal node N4 = N1 with Object of N4 == Object of N1 and State of N4 != State of N1 should be added to the relation") {

			Node* N4 = new Node(O1, *S2);

			THEN("Return false in _checkEquality()") {
				bool isEqualToN1 = GeneratorHelper::checkEquality(N4, N1);
				REQUIRE_FALSE(isEqualToN1);
			}
		}

		WHEN("A meta-equal node N4 = N2 is to be added to R via N4->N1->N2") {

			Object* metaEqualO2 = O2;
			PuzzleState* metaEqualS2 = S2;
			Node* N4 = new Node(metaEqualO2, *metaEqualS2);

			NodePair metaEqualPair = Relation::makePuzzlePair(N4, N1);
			R->addPair(metaEqualPair);

			THEN("Return true in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = GeneratorHelper::checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE(hasMetaEqualOccurence);
			}
		}


		WHEN("A non-meta-equal node N4 ?= N2 is to be added to R via N4->N1->N2, where N4 and N2 have different states") {

			Object* metaEqualO2 = O2;
			PuzzleState* metaEqualS2 = new PuzzleState("State_Sx");
			Node* N4 = new Node(metaEqualO2, *metaEqualS2);

			NodePair metaEqualPair = Relation::makePuzzlePair(N4, N1);
			R->addPair(metaEqualPair);

			THEN("Return false in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = GeneratorHelper::checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE_FALSE(hasMetaEqualOccurence);
			}
		}

		WHEN("A custom rule is used to restrict the generation: (O4, *) < (O2, *)") {

			Rule* rule1 = new Rule(N4->getRelatedObject(), nullptr, N2->getRelatedObject(), nullptr, Rule::EPuzzleRuleType::BEFORE);

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O4,*) =R= (O4, S4)") {
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1->getLeftHandSideObject(), N4->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1->getLeftHandSideState(), S4);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O2,*) =R= (O2, S2)"){
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1->getRightHandSideObject(), N2->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1->getRightHandSideState(), S2);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual should return false, isRuleStateEqual true (because of wildcard) with (O2,*) =R= (O3, S2)"){
				bool isRuleObjectEqual = GeneratorHelper::isRuleObjectEqual(rule1->getRightHandSideObject(), N3->getRelatedObject());
				bool isRuleStateEqual = GeneratorHelper::isRuleStateEqual(rule1->getRightHandSideState(), S3);
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
			Node* N5 = new Node(O1, *S2);
			nodes.push_back(N5);
			Object* O5 = new Object("O5");

			THEN("findNodesByPattern for Object O1 and S1 should only return N1") {
				NodeVec foundNodes = R->findNodesByPattern(nodes, O1, S1, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 1);
				REQUIRE(foundNodes.at(0) == N1);
			}

			THEN("findNodesByPattern for Object O1 and * should return N1 and N5") {
				NodeVec foundNodes = R->findNodesByPattern(nodes, O1, nullptr, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 2);
				bool result = (foundNodes.at(0) == N1) || (foundNodes.at(0) == N5);
				REQUIRE( result );
			}

			THEN("findNodesByPattern for Object O5 and * should return nothing") {
				NodeVec foundNodes = R->findNodesByPattern(nodes, O5, nullptr, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
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

		Generator* PG = new Generator();

		RuleVec rules;
		EventVec events;
		ObjVec objects;

		Object* O1 = new Object("Object_O1");
		objects.push_back(O1);
		PuzzleState* S1_1 = new PuzzleState("State_S1_1");
		PuzzleState* S1_2 = new PuzzleState("State_S1_2");
		StateTransition* T1 = new StateTransition();
		Event* E1 = new Event("Event_E1", O1);
		T1->addTransition(E1->getEventName(), *S1_1, *S1_2);
		T1->addTransition(E1->getEventName(), *S1_2, *S1_1);
		O1->setStateTransition(*T1);
		events.push_back(E1);
		Node* N1 = new Node(O1, *S1_1);

		Object* O2 = new Object("Object_O2");
		objects.push_back(O2);
		PuzzleState* S2_1 = new PuzzleState("State_S2_1");
		PuzzleState* S2_2 = new PuzzleState("State_S2_2");
		StateTransition* T2 = new StateTransition();
		Event* E2 = new Event("Event_E2", O2);
		T2->addTransition(E2->getEventName(), *S2_1, *S2_2);
		T2->addTransition(E2->getEventName(), *S2_2, *S2_1);
		O2->setStateTransition(*T2);
		events.push_back(E2);
		Node* N2 = new Node(O2, *S2_1);

		Object* O3 = new Object("Object_O3");
		objects.push_back(O3);
		PuzzleState* S3_1 = new PuzzleState("State_S3_1");
		PuzzleState* S3_2 = new PuzzleState("State_S3_2");
		PuzzleState* S3_3 = new PuzzleState("State_S3_3");
		StateTransition* T3 = new StateTransition();
		Event* E3_1 = new Event("Event_E3_1", O3);
		Event* E3_2 = new Event("Event_E3_2", O3);
		T3->addTransition(E3_1->getEventName(),*S3_1, *S3_2);
		T3->addTransition(E3_2->getEventName(),*S3_2, *S3_3);
		events.push_back(E3_1);
		events.push_back(E3_2);
		O3->setStateTransition(*T3);
		Node* N3 = new Node(O3, *S3_1);

		NodePair n1n2Pair = Relation::makePuzzlePair(N1, N2);
		NodePair n2n3Pair = Relation::makePuzzlePair(N2, N3);

		Rule* R1 = new Rule(O1, S1_2, O2, S2_1, Rule::EPuzzleRuleType::BEFORE);
		Rule* R2 = new Rule(O2, S2_2, O3, nullptr, Rule::EPuzzleRuleType::BEFORE);
		Rule* R3 = new Rule(O1, nullptr, O3, nullptr, Rule::EPuzzleRuleType::STRICT_BEFORE);
		Rule* R4 = new Rule(O1, nullptr, O3, nullptr, Rule::EPuzzleRuleType::AFTER);
		Rule* R5 = new Rule(O2, S2_2, O3, nullptr, Rule::EPuzzleRuleType::STRICT_AFTER);

		//PG->setSeed(seed);
		PG->setNumberNodes(numberNodes);
		

		WHEN("A puzzle is generated") {
			Puzzle* P = PG->generatePuzzle(objects, events, rules);
			UNSCOPED_INFO("Generated Puzzle:");
			UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
			THEN("It is definitely generated..") {
				REQUIRE(P != nullptr);
			}

		}

		WHEN("A puzzle is generated with the Rule (O2,S2_2) < (O3, *)") {
			rules.clear();
			rules.push_back(*R2);
			Puzzle* P = PG->generatePuzzle(objects, events, rules);
			THEN("Every node (O2, S2_2) has to occur before every other node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O2, S2_2)
					if ((*it)->getRelatedObject() == O2 && (*it)->getGoalState().getStateName() == (*S2_2).getStateName()) {
						auto checkNotO3 = [](Node N) -> auto {return !(N.getRelatedObject()->getObjectName() == "Object_O3"); };
						// All smaller nodes than (O2, S2_2) have to be != O3
						result = result && R.checkAllSmaller((*it), checkNotO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R2->getTextualRepresentation());
				UNSCOPED_INFO("Checked nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}
		
		}

		WHEN("A puzzle is generated with the Rule (O1,S1_2) < (O2, S2_1)") {
			rules.clear();
			rules.push_back(*R1);
			Puzzle* P = PG->generatePuzzle(objects, events, rules);

			THEN("Every node (O1, S1_2) has to occur before every other node referencing O2 and Stage S2_1") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, S1_2)
					if ((*it)->getRelatedObject() == O1 && (*it)->getGoalState().getStateName() == (*S1_2).getStateName()) {
						auto checkNotO2AndS2_1 = [](Node N) -> auto {return !(N.getRelatedObject()->getObjectName() == "Object_O2" && N.getGoalState().getStateName() == "State_2_1"); };
						// All smaller nodes than (O1, S1_2) have to be != O2 AND != S2_1
						result = result && R.checkAllSmaller((*it), checkNotO2AndS2_1);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R1->getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}

		}

		WHEN("A puzzle is generated with the Rule (O1,*) <! (O3,*)") {
			rules.clear();
			rules.push_back(*R3);
			Puzzle* P = PG->generatePuzzle(objects, events, rules);

			THEN("Every node (O1,*) has to occur STRICTLY (directly and only) before a node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, *)
					if ((*it)->getRelatedObject() == O1) {
						auto checkIsO3= [](Node N) -> auto {return (N.getRelatedObject()->getObjectName() == "Object_O3"); };
						// All directly following nodes of (O1, *) have to  be = O3 
						result = result && R.checkAllFollowing((*it), checkIsO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R3->getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);

			}
		}

		WHEN("A puzzle is generated with the Rule (O1,*) > (O3,*)") {
			rules.clear();
			rules.push_back(*R4);
			Puzzle* P = PG->generatePuzzle(objects, events, rules);

			THEN("Every node (O1,*) has to be after every other node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, *)
					if ((*it)->getRelatedObject() == O1) {
						auto checkNotO3 = [](Node N) -> auto {return !(N.getRelatedObject()->getObjectName() == "Object_O3"); };
						// There must not be a node referencing O3 LARGER than O1
						result = result && R.checkAllLarger((*it), checkNotO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R4->getTextualRepresentation());
				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);

			}
		}


		WHEN("A puzzle is generated with the Rule (O2,S2_2) >! (O3, *)") {
			rules.clear();
			rules.push_back(*R5);
			Puzzle* P = PG->generatePuzzle(objects, events, rules);
			THEN("Every node (O2, S2_2) has to occur STRICTLY (directly and only) after another node referencing O3") {

				NodeVec nodes = P->getNodes();

				Relation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O2, S2_2)
					if ((*it)->getRelatedObject() == O2 && (*it)->getGoalState().getStateName() == (*S2_2).getStateName()) {
						auto checkIsO3 = [](Node N) -> auto {return (N.getRelatedObject()->getObjectName() == "Object_O3"); };
						// At least one preceding node has to be referencing O3
						result = result && R.checkAtLeastOnePreceding((*it), checkIsO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Rule:");
				UNSCOPED_INFO(R5->getTextualRepresentation());
				UNSCOPED_INFO("Checked nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}

		}


		WHEN("A puzzle is generated without Rules") {
			rules.clear();
			Puzzle* P = PG->generatePuzzle(objects, events, rules);
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