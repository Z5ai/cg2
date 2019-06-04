cmake_minimum_required(VERSION 2.6)
set(COURSE_NAME "CG2")
project(${COURSE_NAME} C CXX)


# Add CGV framework
# - configure
option(BUILD_EXAMPLE_PLUGIN "Build the example plugin" FALSE)
mark_as_advanced(FORCE BUILD_EXAMPLE_PLUGIN)
set(LIBS)
set(cgv_DIR ${CMAKE_CURRENT_SOURCE_DIR}/framework)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/framework ext_build/framework)
include_directories(
	${cgv_INCLUDE_DIRS} 
	../framework/libs 
	${cgv_gl_INCLUDE_DIRS}
)
# - prepare for use by the exercises
find_package(cgv 
	COMPONENTS render gl gui base type reflect utils signal data media
	HINTS ${cgv_DIR}
	)
find_package(OpenGL)
find_package(shader_test)
find_package(cgv_viewer)
cgv_find_package(GLEW)
cgv_find_package(cg_fltk)
cgv_find_package(cg_icons)
cgv_find_package(cgv_gl)
cgv_find_package(crg_stereo_view)
cgv_get_viewer_locations(VIEWER_EXE VIEWER_DEBUG_EXE)
get_filename_component(FINAL_BUILD_BIN_DIR ${VIEWER_EXE} DIRECTORY)

# Add exercises
add_subdirectory(exercise1)
add_subdirectory(exercise2)
add_subdirectory(exercise45)

# Launch configurations for various IDEs that are not Visual Studio
# - VS Code
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/.vscode)
configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/launch.json.in ${CMAKE_SOURCE_DIR}/.vscode/launch.json @ONLY)
# - pure Unix
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tmp)
foreach(EXERCISE_ID "1" "2" "45")
if(${EXERCISE_ID} EQUAL 2)
	set(EXTRA_PLUGINS "plugin:crg_grid plugin:crg_light")
	set(EXTRA_PLUGINS_DEBUG "pplugin:crg_grid_d plugin:crg_light_d")
	set(EXTRA_ARGS "")
else()
	set(EXTRA_PLUGINS "")
	set(EXTRA_PLUGINS_DEBUG "")
	set(EXTRA_ARGS "")
endif()
configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/run.sh.in ${CMAKE_BINARY_DIR}/tmp/run${EXERCISE_ID}.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/CMake/Templates/debug.sh.in ${CMAKE_BINARY_DIR}/tmp/debug${EXERCISE_ID}.sh @ONLY)
file(COPY
	${CMAKE_BINARY_DIR}/tmp/run${EXERCISE_ID}.sh
	DESTINATION ${CMAKE_BINARY_DIR}
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
file(COPY
	${CMAKE_BINARY_DIR}/tmp/debug${EXERCISE_ID}.sh
	DESTINATION ${CMAKE_BINARY_DIR}
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
endforeach(EXERCISE_ID)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/tmp)
