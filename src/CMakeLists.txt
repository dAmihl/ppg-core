ADD_LIBRARY( ppg-core STATIC
            PuzzGen.cpp
			Puzzle.cpp 
			PuzzleEvent.cpp 
			PuzzleEventManager.cpp 
			PuzzleGenerator.cpp
			PuzzleGeneratorHelper.cpp
			PuzzleGraphNode.cpp
			PuzzleNode.cpp 
			PuzzleObject.cpp 
			PuzzleRandomizer.cpp 
			PuzzleRelation.cpp 
			PuzzleRule.cpp 
			PuzzleState.cpp 
			PuzzleUpdateListener.cpp			
			StateTransition.cpp
			PuzzleLogger.cpp
			IPuzzleObjectMetaData.cpp)
			
set_target_properties(ppg-core PROPERTIES 
		PUBLIC_HEADER 	"${PROJECT_HEADERS_DIR}/PuzzGen.h;${PROJECT_HEADERS_DIR}/PUZZGEN_TYPES.h;${PROJECT_HEADERS_DIR}/Puzzle.h;${PROJECT_HEADERS_DIR}/PuzzleEvent.h;${PROJECT_HEADERS_DIR}/PuzzleEventManager.h;${PROJECT_HEADERS_DIR}/PuzzleGenerator.h;${PROJECT_HEADERS_DIR}/PuzzleGeneratorHelper.h;${PROJECT_HEADERS_DIR}/PuzzleGraphNode.h;${PROJECT_HEADERS_DIR}/PuzzleNode.h;${PROJECT_HEADERS_DIR}/PuzzleObject.h;${PROJECT_HEADERS_DIR}/PuzzleRandomizer.h;${PROJECT_HEADERS_DIR}/PuzzleRelation.h;${PROJECT_HEADERS_DIR}/PuzzleRule.h;${PROJECT_HEADERS_DIR}/PuzzleState.h;${PROJECT_HEADERS_DIR}/PuzzleUpdateListener.h;${PROJECT_HEADERS_DIR}/StateTransition.h;${PROJECT_HEADERS_DIR}/IPuzzleObjectMetaData.h;${PROJECT_HEADERS_DIR}/PuzzleLogger.h;")

set_property(TARGET ppg-core PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		
INSTALL(TARGETS ppg-core 
        LIBRARY DESTINATION lib/
        PUBLIC_HEADER DESTINATION includes/
)