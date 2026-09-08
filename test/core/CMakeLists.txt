# Add subdirectories
#add_subdirectory(...)

# Gather files at this level
file (GLOB LOCAL_TEST_SOURCE_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

target_sources(${TEST_TARGET_NAME} PRIVATE ${LOCAL_TEST_SOURCE_FILES})

# Propagate source files to parent
list(APPEND TEST_SOURCE_FILES ${LOCAL_TEST_SOURCE_FILES})
set(TEST_SOURCE_FILES ${TEST_SOURCE_FILES} PARENT_SCOPE)
