# !!!!!
# README
#
# You have to be building MetaCPPCLI or specify the location of the executable like this:
#
# set(META_CPP_BIN "Tools/MetaCPPCLI.exe")
#
# You have to me using CMake 3.1 or newer (target_sources)
# !!!!!

# This should be called after configuring the whole project
# Example
# meta_generate(MetaTest "ToReflect.hpp" "Generated.hpp" "Generated.cpp")
macro(meta_generate PROJECT_NAME IN_SOURCE OUT_HEADER OUT_SOURCE ADDITIONAL_FLAGS)
	message("Project ${PROJECT_NAME} will be reflected:")
	
	# get the include directories
	get_property(INC_DIRECTORIES TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
	
	# flags that will be passed to the tool
	set(FLAGS "")
	set(FLAGS "${FLAGS} ${ADDITIONAL_FLAGS}")
	
	# add the include directories into the flags
	message("  Included directories:")
	foreach (DIRECTORY ${INC_DIRECTORIES})
		message("   - ${DIRECTORY}")
		set(FLAGS "${FLAGS} --flag -I\"${DIRECTORY}\"")
	endforeach ()
		
	# get the absolute path to the files
	get_filename_component(IN_SOURCE_PATH "${IN_SOURCE}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	get_filename_component(OUT_HEADER_PATH "${OUT_HEADER}" REALPATH BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	get_filename_component(OUT_SOURCE_PATH "${OUT_SOURCE}" REALPATH BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

	# get the executable absolute path
	if(TARGET MetaCPPCLI)
		# we're compiling the cli
		add_dependencies(${PROJECT_NAME} MetaCPPCLI)
		set(META_CPP_EXE "$<TARGET_FILE:MetaCPPCLI>")
	else()
		# we'll fallback to a precompiled
		get_filename_component(META_CPP_EXE "${META_CPP_BIN}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	endif()
	
	message("  Source in: ${IN_SOURCE_PATH}")
	message("  Header out: ${OUT_HEADER_PATH}")
	message("  Source out: ${OUT_SOURCE_PATH}")
	message("  MetaCPPCLI path: ${META_CPP_EXE}")
	#message("  FLAGS: ${FLAGS}")
	
	target_sources(${PROJECT_NAME} PRIVATE ${OUT_HEADER_PATH} ${OUT_SOURCE_PATH})
	
	set(CMD "${META_CPP_EXE} \"${IN_SOURCE_PATH}\" -out-header \"${OUT_HEADER_PATH}\" -out-source \"${OUT_SOURCE_PATH}\" ${FLAGS}")
	separate_arguments(CMD)
	
	# command
	#add_custom_target(
	#	"${PROJECT_NAME}_metacpp_gen"
	#	COMMAND ${CMD}
	#	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	#)
	#add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}_metacpp_gen")
	#set_source_files_properties(${OUT_HEADER_PATH} ${OUT_SOURCE_PATH} PROPERTIES GENERATED TRUE)
	
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_BUILD
		COMMAND ${CMD}
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)
	set_source_files_properties(${OUT_HEADER_PATH} ${OUT_SOURCE_PATH} PROPERTIES GENERATED TRUE)
	
endmacro()