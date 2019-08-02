#include "catch2/catch.hpp"
#define private public
#include "PuzzGen.h"

TEST_CASE("PuzzleGeneratorHelperUnitTests", "[PPG_UNIT_TEST]") {

	GIVEN("a simple Puzzle Relation with 4 nodes") {

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

		

		WHEN("A Circular dependency would be created as direct successor") {

			R->addPair(N1, N2);
			R->addPair(N2, N3);

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N2, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("A Circular dependency would be created as indirect successor") {

			R->addPair(N1, N2);
			R->addPair(N2, N3);

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N3, N1);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE(hasCircularDependency);
			}

		}

		WHEN("No circular dependency would be created, by adding N3->N4") {

			R->addPair(N1, N2);
			R->addPair(N2, N3);

			T_PuzzleNodePair circularDependencyPair = PuzzleRelation::makePuzzlePair(N3, N4);

			THEN("Return true in _checkCreatesCircularDependency()") {
				bool hasCircularDependency = PuzzleGeneratorHelper::_checkCreatesCircularDependency(circularDependencyPair, R);
				REQUIRE_FALSE(hasCircularDependency);
			}

		}

		WHEN("Looking for a node in sequential order starting from N1-N2") {

			T_PuzzleNodePair n1n2Pair = PuzzleRelation::makePuzzlePair(N1, N2);
			T_PuzzleNodePair n2n3Pair = PuzzleRelation::makePuzzlePair(N2, N3);

			R->addPair(n1n2Pair);
			R->addPair(n2n3Pair);

			THEN("Return true because node N3 can be found") {
				REQUIRE(PuzzleGeneratorHelper::_findNodeSequential(N3, n1n2Pair, R));
			}

			THEN("Return true because node N2 can be found in the starting node") {
				REQUIRE(PuzzleGeneratorHelper::_findNodeSequential(N2, n1n2Pair, R));
			}
		}

	}

}