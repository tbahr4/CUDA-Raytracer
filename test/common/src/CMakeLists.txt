# Gather files at this level
file (GLOB SOURCE_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

target_sources(${TEST_TARGET_NAME} PRIVATE ${TEST_SOURCE_FILES})

# Propagate source files to parent
set(SOURCE_FILES ${TEST_SOURCE_FILES} PARENT_SCOPE)
