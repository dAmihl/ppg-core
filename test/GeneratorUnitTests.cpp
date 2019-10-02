#include "catch2/catch.hpp"
#include "PuzzGen.h"

TEST_CASE("PuzzleGeneratorHelperUnitTests", "[PPG_UNIT_TEST]") {

	GIVEN("a simple Puzzle Relation with 4 nodes and a simple Relation N1->N2->N3") {

		PuzzleRelation* R = new PuzzleRelation();

		PuzzleGenerator* PG = new PuzzleGenerator();
		
		PuzzleObject* O1 = new PuzzleObject("Object_O1");
		PuzzleState* S1 = new PuzzleState("State_S1");
		PuzzleNode* N1 = new PuzzleNode(O1, *S1);

		PuzzleObject* O2 = new PuzzleObject("Object_O2");
		PuzzleState* S2 = new PuzzleState("State_S2");
		PuzzleNode* N2 = new PuzzleNode(O2, *S2);

		PuzzleObject* O3 = new PuzzleObject("Object_O3");
		PuzzleState* S3 = new PuzzleState("State_S3");
		PuzzleNode* N3 = new PuzzleNode(O3, *S3);

		PuzzleObject* O4 = new PuzzleObject("Object_O4");
		PuzzleState* S4 = new PuzzleState("State_S4");
		PuzzleNode* N4 = new PuzzleNode(O4, *S4);

		T_PuzzleNodePair n1n2Pair = PuzzleRelation::makePuzzlePair(N1, N2);
		T_PuzzleNodePair n2n3Pair = PuzzleRelation::makePuzzlePair(N2, N3);

		R->addPair(n1n2Pair);
		R->addPair(n2n3Pair);
		

		WHEN("A Circular dependency would be created as direct successor") {

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N2, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("A Circular dependency would be created as indirect successor") {

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N3, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("No circular dependency would be created, by adding N3->N4") {

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N3, N4);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE_FALSE(hasCircularDependency);
			}

		}

		WHEN("Looking for a node in sequential order starting from N1-N2") {

			THEN("Return true because node N3 can be found") {
				REQUIRE(PuzzleGeneratorHelper::_findNodeSequential(N3, n1n2Pair, R));
			}

			THEN("Return true because node N2 can be found in the starting node") {
				REQUIRE(PuzzleGeneratorHelper::_findNodeSequential(N2, n1n2Pair, R));
			}
		}

		WHEN("A exclusive dependency would be created by adding N4->N2 where N4 and N1 have the same object and different states") {

			PuzzleState* S1_2 = new PuzzleState("State_S1_2");
			PuzzleNode* N4 = new PuzzleNode(O1, *S1_2);
			T_PuzzleNodePair exclusiveDependencyPair = PuzzleRelation::makePuzzlePair(N4, N2);

			THEN("Return true in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}

		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have different objects but same state") {

			PuzzleState* S1_2 = new PuzzleState("State_S1_2");
			PuzzleNode* N4 = new PuzzleNode(O2, *S1_2);
			T_PuzzleNodePair exclusiveDependencyPair = PuzzleRelation::makePuzzlePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE_FALSE(hasExclusiveDependency);
			}

		}

		WHEN("No exclusive dependency would be created by adding N4->N2 where N4 and N1 have the same object and same state") {

			PuzzleNode* N4 = new PuzzleNode(O1, *S1);
			T_PuzzleNodePair exclusiveDependencyPair = PuzzleRelation::makePuzzlePair(N4, N2);

			THEN("Return false in _checkCreatesExclusiveDependency()") {
				bool hasExclusiveDependency = PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(exclusiveDependencyPair, R);
				REQUIRE(hasExclusiveDependency);
			}

		}

		WHEN("An equal node N4 = N1 with Object of N4 == Object of N1 and State of N4 == State of N1 should be added to the relation") {

			PuzzleNode* N4 = new PuzzleNode(O1, *S1);

			THEN("Return true in _checkEquality()") {
				bool isEqualToN1 = PuzzleGeneratorHelper::_checkEquality(N4, N1);
				REQUIRE(isEqualToN1);
			}

		}

		WHEN("An inequal node N4 = N1 with Object of N4 == Object of N1 and State of N4 != State of N1 should be added to the relation") {

			PuzzleNode* N4 = new PuzzleNode(O1, *S2);

			THEN("Return false in _checkEquality()") {
				bool isEqualToN1 = PuzzleGeneratorHelper::_checkEquality(N4, N1);
				REQUIRE_FALSE(isEqualToN1);
			}
		}

		WHEN("A meta-equal node N4 = N2 is to be added to R via N4->N1->N2") {

			PuzzleObject* metaEqualO2 = O2;
			PuzzleState* metaEqualS2 = S2;
			PuzzleNode* N4 = new PuzzleNode(metaEqualO2, *metaEqualS2);

			T_PuzzleNodePair metaEqualPair = PuzzleRelation::makePuzzlePair(N4, N1);
			R->addPair(metaEqualPair);

			THEN("Return true in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = PuzzleGeneratorHelper::_checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE(hasMetaEqualOccurence);
			}
		}


		WHEN("A non-meta-equal node N4 ?= N2 is to be added to R via N4->N1->N2, where N4 and N2 have different states") {

			PuzzleObject* metaEqualO2 = O2;
			PuzzleState* metaEqualS2 = new PuzzleState("State_Sx");
			PuzzleNode* N4 = new PuzzleNode(metaEqualO2, *metaEqualS2);

			T_PuzzleNodePair metaEqualPair = PuzzleRelation::makePuzzlePair(N4, N1);
			R->addPair(metaEqualPair);

			THEN("Return false in _checkMetaEqualOccurence()") {
				bool hasMetaEqualOccurence = PuzzleGeneratorHelper::_checkMetaEqualOccurance(metaEqualPair, R);
				REQUIRE_FALSE(hasMetaEqualOccurence);
			}
		}

		WHEN("A custom rule is used to restrict the generation: (O4, *) < (O2, *)") {

			PuzzleRule* rule1 = new PuzzleRule(N4->getRelatedObject(), nullptr, N2->getRelatedObject(), nullptr, PuzzleRule::E_PuzzleRuleType::BEFORE);

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O4,*) =R= (O4, S4)") {
				bool isRuleObjectEqual = PuzzleGeneratorHelper::__isRuleObjectEqual(rule1->getLeftHandSideObject(), N4->getRelatedObject());
				bool isRuleStateEqual = PuzzleGeneratorHelper::__isRuleStateEqual(rule1->getLeftHandSideState(), S4);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual and isRuleStateEqual should return true with (O2,*) =R= (O2, S2)"){
				bool isRuleObjectEqual = PuzzleGeneratorHelper::__isRuleObjectEqual(rule1->getRightHandSideObject(), N2->getRelatedObject());
				bool isRuleStateEqual = PuzzleGeneratorHelper::__isRuleStateEqual(rule1->getRightHandSideState(), S2);
				REQUIRE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

			THEN("isRuleObjectEqual should return false, isRuleStateEqual true (because of wildcard) with (O2,*) =R= (O3, S2)"){
				bool isRuleObjectEqual = PuzzleGeneratorHelper::__isRuleObjectEqual(rule1->getRightHandSideObject(), N3->getRelatedObject());
				bool isRuleStateEqual = PuzzleGeneratorHelper::__isRuleStateEqual(rule1->getRightHandSideState(), S3);
				REQUIRE_FALSE(isRuleObjectEqual);
				REQUIRE(isRuleStateEqual);
			}

		}

		WHEN("A list of nodes N1..N5 is given") {
			T_PuzzleNodeList nodes;
			nodes.push_back(N1);
			nodes.push_back(N2);
			nodes.push_back(N3);
			nodes.push_back(N4);
			PuzzleNode* N5 = new PuzzleNode(O1, *S2);
			nodes.push_back(N5);
			PuzzleObject* O5 = new PuzzleObject("O5");

			THEN("findNodesByPattern for Object O1 and S1 should only return N1") {
				T_PuzzleNodeList foundNodes = R->findNodesByPattern(nodes, O1, S1, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 1);
				REQUIRE(foundNodes.at(0) == N1);
			}

			THEN("findNodesByPattern for Object O1 and * should return N1 and N5") {
				T_PuzzleNodeList foundNodes = R->findNodesByPattern(nodes, O1, nullptr, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 2);
				bool result = (foundNodes.at(0) == N1) || (foundNodes.at(0) == N5);
				REQUIRE( result );
			}

			THEN("findNodesByPattern for Object O5 and * should return nothing") {
				T_PuzzleNodeList foundNodes = R->findNodesByPattern(nodes, O5, nullptr, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
				int size = foundNodes.size();
				REQUIRE(size == 0);
			}

		}


	}

	GIVEN("A more extensive puzzle universe with X nodes and rules") {

		// Deterministic generation
		unsigned int seed = 664242;

		PuzzleGenerator* PG = new PuzzleGenerator();

		T_PuzzleRuleList rules;
		T_PuzzleEventList events;
		T_PuzzleObjectList objects;

		PuzzleObject* O1 = new PuzzleObject("Object_O1");
		objects.push_back(O1);
		PuzzleState* S1_1 = new PuzzleState("State_S1_1");
		PuzzleState* S1_2 = new PuzzleState("State_S1_2");
		StateTransition* T1 = new StateTransition();
		PuzzleEvent* E1 = new PuzzleEvent("Event_E1", O1);
		T1->addTransition(E1->getEventName(), *S1_1, *S1_2);
		O1->setStateTransition(*T1);
		events.push_back(E1);
		PuzzleNode* N1 = new PuzzleNode(O1, *S1_1);

		PuzzleObject* O2 = new PuzzleObject("Object_O2");
		objects.push_back(O2);
		PuzzleState* S2_1 = new PuzzleState("State_S2_1");
		PuzzleState* S2_2 = new PuzzleState("State_S2_2");
		StateTransition* T2 = new StateTransition();
		PuzzleEvent* E2 = new PuzzleEvent("Event_E2", O2);
		T2->addTransition(E2->getEventName(), *S2_1, *S2_2);
		O2->setStateTransition(*T2);
		events.push_back(E2);
		PuzzleNode* N2 = new PuzzleNode(O2, *S2_1);

		PuzzleObject* O3 = new PuzzleObject("Object_O3");
		objects.push_back(O3);
		PuzzleState* S3_1 = new PuzzleState("State_S3_1");
		PuzzleState* S3_2 = new PuzzleState("State_S3_2");
		PuzzleState* S3_3 = new PuzzleState("State_S3_2");
		StateTransition* T3 = new StateTransition();
		PuzzleEvent* E3_1 = new PuzzleEvent("Event_E3_1", O3);
		PuzzleEvent* E3_2 = new PuzzleEvent("Event_E3_2", O3);
		T3->addTransition(E3_1->getEventName(),*S3_1, *S3_2);
		T3->addTransition(E3_2->getEventName(),*S3_2, *S3_3);
		events.push_back(E3_1);
		events.push_back(E3_2);
		O3->setStateTransition(*T3);
		PuzzleNode* N3 = new PuzzleNode(O3, *S3_1);

		T_PuzzleNodePair n1n2Pair = PuzzleRelation::makePuzzlePair(N1, N2);
		T_PuzzleNodePair n2n3Pair = PuzzleRelation::makePuzzlePair(N2, N3);

		PuzzleRule* R1 = new PuzzleRule(O1, S1_2, O2, nullptr, PuzzleRule::E_PuzzleRuleType::BEFORE);
		PuzzleRule* R2 = new PuzzleRule(O2, S2_2, O3, nullptr, PuzzleRule::E_PuzzleRuleType::BEFORE);

		rules.push_back(*R1);
		rules.push_back(*R2);

		PG->setSeed(seed);
		PG->setNumberNodes(10);
		Puzzle* P = PG->generatePuzzle(objects, events, rules);

		WHEN("A puzzle is generated") {
			UNSCOPED_INFO("Generated Puzzle:");
			UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
			THEN("It is definitely generated..") {
				REQUIRE(P != nullptr);
			}

		}

		WHEN("A puzzle is generated with the Rule (O2,S2_2) < (O3, *)") {
		
			THEN("Every node (O2, S2_2) has to occur before every other node referencing O3") {

				T_PuzzleNodeList nodes = P->getNodes();

				PuzzleRelation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O2, S2_2)
					if ((*it)->getRelatedObject() == O2 && (*it)->getGoalState().getStateName() == (*S2_2).getStateName()) {
						auto checkNotO3 = [](PuzzleNode N) -> auto {return !(N.getRelatedObject()->getObjectName() == "Object_O3"); };
						// All smaller nodes than (O2, S2_2) have to be != O3
						result = result && R.checkAllSmaller((*it), checkNotO3);
						count++;
					}
					if (!result) break;
				}
				UNSCOPED_INFO("Checked nodes:");
				UNSCOPED_INFO(count);
				UNSCOPED_INFO("Generated Puzzle:");
				UNSCOPED_INFO(P->getRelation().getExtendedTextualRepresentation(P->getNodes()));
				REQUIRE(result);
			}
		
		}

		WHEN("A puzzle is generated with the Rule (O1,S1_2) < (O2, S2_1)") {

			THEN("Every node (O1, S1_2) has to occur before every other node referencing O2 and Stage S2_1") {

				T_PuzzleNodeList nodes = P->getNodes();

				PuzzleRelation R = P->getRelation();

				bool result = true;
				int count = 0;

				for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {

					// The node (O1, S1_2)
					if ((*it)->getRelatedObject() == O1 && (*it)->getGoalState().getStateName() == (*S1_2).getStateName()) {
						auto checkNotO2AndS2_1 = [](PuzzleNode N) -> auto {return !(N.getRelatedObject()->getObjectName() == "Object_O2") && !(N.getGoalState().getStateName() == "State_2_1"); };
						// All smaller nodes than (O1, S1_2) have to be != O2 AND != S2_1
						result = result && R.checkAllSmaller((*it), checkNotO2AndS2_1);
						count++;
					}
					if (!result) break;
				}

				UNSCOPED_INFO("Checked Nodes:");
				UNSCOPED_INFO(count);
				REQUIRE(result);
			}

		}

	}

}