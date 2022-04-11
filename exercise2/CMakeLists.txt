cmake_minimum_required(VERSION 3.15)

if(NOT CG2_ROOT_DIR)
	find_package(CGV REQUIRED)
	get_filename_component(CG2_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
endif()

project(CG2_exercise2)

file(GLOB_RECURSE SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cxx")
file(GLOB_RECURSE HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
file(GLOB_RECURSE SHADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.gl*")

set(ALL_SOURCES ${SOURCES} ${HEADERS} ${IMG_SOURCES} ${ST_FILES} ${SHADERS} ${IMAGES})

add_library(CG2_exercise2 SHARED ${ALL_SOURCES})

target_link_libraries(CG2_exercise2
	cgv_gl
	cgv_type
	cgv_base
	cgv_signal
	cgv_reflect
	cgv_os
	cgv_gui
	cgv_render)

add_dependencies(CG2_exercise2
	cgv_viewer
	cg_ext
	cg_fltk
	cg_meta
	cmi_io
	cgv_reflect_types
	crg_light
	crg_stereo_view)

set_plugin_execution_params(CG2_exercise2
"plugin:cg_fltk \
\"type(shader_config):shader_path='${CGV_DIR}/libs/cgv_gl/glsl'\" \
plugin:cg_ext \
plugin:crg_stereo_view \
plugin:cg_meta \
plugin:cmi_io \
plugin:CG2_exercise2 \
config:\"${CMAKE_CURRENT_SOURCE_DIR}/config.def\"")
set_plugin_execution_working_dir(CG2_exercise2 ${CG2_ROOT_DIR})
configure_file(CG2_exercise2.sh.in ${CMAKE_BINARY_DIR}/CG2_exercise2.sh
	FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	@ONLY)

install(TARGETS CG2_exercise2 EXPORT cgv_plugins DESTINATION ${CGV_BIN_DEST})
