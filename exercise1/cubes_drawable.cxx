// This source code is property of the Computer Graphics and Visualization chair of the
// TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved
//
// The main file of the plugin. It defines a class that demonstrates how to register with
// the scene graph, drawing primitives, creating a GUI, using a config file and various
// other parts of the framework.

// Framework core
#include <cgv/base/register.h>
#include <cgv/gui/provider.h>
#include <cgv/gui/trigger.h>
#include <cgv/render/drawable.h>
#include <cgv/render/render_types.h>
#include <cgv/render/shader_program.h>
#include <cgv/render/vertex_buffer.h>
#include <cgv/render/attribute_array_binding.h>
#include <cgv/math/ftransform.h>

// Framework standard plugins
#include <cgv_gl/gl/gl.h>

// Local includes
#include "cubes_fractal.h"



/// ************************************************************************************/
/// Task 1.2a: Create a drawable that provides a (for now, empty) GUI and supports
///            reflection, so that its properties can be set via config file.
///
/// Task 1.2b: Utilize the cubes_fractal class to render a fractal of hierarchically
///            transformed cubes. Expose its recursion depth and color properties to GUI
///            manipulation and reflection. Set reasonable values via the config
///            file.
///
/// Task 1.2c: Implement an option (configurable via GUI and config file) to use a vertex
///            array object for rendering the cubes. The vertex array functionality 
///            should support (again, configurable via GUI and config file) both
///            interleaved (as in cgv_demo.cpp) and non-interleaved attributes.

// < your code here >

class cubes_drawable_scene :
	public cgv::base::base,      // This class supports reflection
	public cgv::gui::provider,   // Instances of this class provde a GUI
	public cgv::render::drawable // Instances of this class can be rendered
{
protected:
	
	float cube_color_r, cube_color_g, cube_color_b;

	unsigned int max_depth = 3;

	cgv::media::color<float> cube_color;

	enum RenderingMode { INTERLEAVED_MODE, NONINTERLEAVED_MODE, BUILT_IN, BONUS_TASK_1_3_1} rendering_mode;

	// Geometry buffers
	struct vertex {
		cgv::render::render_types::vec3 pos;
		cgv::render::render_types::vec3 normal;
	};
	std::vector<vertex> vertices;

	cgv::render::vertex_buffer vb;
	cgv::render::attribute_array_binding vertex_array;

	//Arrays for noninterleaved vertex arrays
	std::vector<cgv::render::render_types::vec3> positions;
	std::vector<cgv::render::render_types::vec3> normals;


	//Used multiple vbos for noninterleaved vertex arrays, 
	//so we have on for positions and one for the vertex normals
	cgv::render::vertex_buffer vb_non_interleaved_pos;
	cgv::render::vertex_buffer vb_non_interleaved_normals;
	cgv::render::attribute_array_binding vertex_array_non_interleaved;


	//Bonus Task 1.3.1
	std::vector<cgv::render::render_types::vec3> positions_bonus;
	std::vector<cgv::render::render_types::vec3> normals_bonus;

	cgv::render::vertex_buffer vb_bonus_pos;
	cgv::render::vertex_buffer vb_bonus_normals;
	cgv::render::attribute_array_binding vertex_array_bonus;

	bool invalid = false;


	//METHODS FOR UI CREATION

	// Should be overwritten to sensibly implement the cgv::base::named interface
	std::string get_type_name(void) const
	{
		return "cubes_drawable";
	}


	// Required interface for cgv::gui::provider
	void create_gui(void)
	{
		// Simple controls. Notifies us of GUI input via the on_set() method.
		// - section header
		add_decorator("Properties", "heading", "level=1");
		add_member_control(this, "recursion depth", max_depth);
		add_member_control(this, "root cube color", cube_color);
		add_member_control(
			this, "Rendering mode", rendering_mode, "dropdown",
			"enums='INTERLEAVED_MODE, NONINTERLEAVED_MODE, BUILT_IN, BONUS_TASK_1_3_1'"
		);
	}




	//METHODS FOR REFLECTION

	// Part of the cgv::base::base interface, can be implemented to make data members of
	// this class available as named properties, e.g. for use with config files
	bool self_reflect(cgv::reflect::reflection_handler& rh)
	{

		unsigned* rendering_mode_uint = (unsigned*)&rendering_mode;

		// Reflect the properties
		return
			rh.reflect_member("recursion_depth", max_depth) &&
			rh.reflect_member("cube_color_r", cube_color_r) &&
			rh.reflect_member("cube_color_g", cube_color_g) &&
			rh.reflect_member("rendering_mode",*rendering_mode_uint) &&
			rh.reflect_member("cube_color_b", cube_color_b);

	}

	// Part of the cgv::base::base interface, should be implemented to respond to write
	// access to reflected data members of this class, e.g. from config file processing
	// or gui interaction.
	void on_set(void* member_ptr)
	{
		// Reflect the change to fb_bgcolor in bgcolor (can only happen via reflection)
		if (member_ptr == &cube_color_r || member_ptr == &cube_color_g ||
			member_ptr == &cube_color_b)
		{
			cube_color.R() = cube_color_r;
			cube_color.G() = cube_color_g;
			cube_color.B() = cube_color_b;
			update_member(&cube_color);
		}
		// ...and vice versa (can only happen via GUI interaction)
		if (member_ptr == &cube_color)
		{
			cube_color_r = cube_color.R();
			cube_color_g = cube_color.G();
			cube_color_b = cube_color.B();
		}

		if (member_ptr == &max_depth)
			invalid = true;

		// Make sure the GUI reflects the new state, in case the write access did not
		// originate from GUI interaction
		update_member(member_ptr);

		// Also trigger a redraw in case the drawable node is active
		if (this->is_visible())
			post_redraw();
	}






	//METHODS FOR RENDERING

	void draw(cgv::render::context& ctx) {

		// Shortcut to the built-in default shader with lighting and texture support
		cgv::render::shader_program& default_shader = ctx.ref_surface_shader_program();

		ctx.set_color(cube_color);

		// Enable shader program we want to use for drawing
		default_shader.enable(ctx);

		auto cf = cubes_fractal::cubes_fractal();

		if (rendering_mode == RenderingMode::INTERLEAVED_MODE) {
			cf.use_vertex_array(&vertex_array, vertices.size(), GL_QUADS);
			cf.draw_recursive(ctx, cube_color, max_depth, 0);

		}
		else if (rendering_mode == RenderingMode::NONINTERLEAVED_MODE) {
			cf.use_vertex_array(&vertex_array_non_interleaved, positions.size(), GL_QUADS);
			cf.draw_recursive(ctx, cube_color, max_depth, 0);

		}
		else if (rendering_mode == RenderingMode::BUILT_IN) {
			cf.draw_recursive(ctx, cube_color, max_depth, 0);
		}
		else if (rendering_mode == RenderingMode::BONUS_TASK_1_3_1) {
			draw_cubes_fractal(ctx);
		}

		//draw_unit_cube(ctx);

		default_shader.disable(ctx);
	}


	// Part of the cgv::render::drawable interface, can be overwritten if there is some
	// intialization work to be done that needs a set-up and ready graphics context,
	// which usually you don't have at object construction time. Should return true if
	// the initialization was successful, false otherwise.
	bool init(cgv::render::context& ctx)
	{
		// Keep track of success - do it this way (instead of e.g. returning false
		// immediatly) to perform every init step even if some go wrong.
		bool success = true;

		init_unit_cube();

		positions_bonus.clear();
		normals_bonus.clear();

		init_cubes_fractal(ctx, max_depth, 0, 1, 
			cgv::render::render_types::vec3(0,0,0),
			cgv::render::render_types::vec3(1,0,0));

		cgv::render::shader_program& default_shader
			= ctx.ref_surface_shader_program();
		cgv::render::type_descriptor
			vec3type =
			cgv::render::element_descriptor_traits<cgv::render::render_types::vec3>
			::get_type_descriptor(vertices[0].pos);


		// - create buffer objects
		success = vb.create(ctx, &(vertices[0]), vertices.size()) && success;
		success = vertex_array.create(ctx) && success;
		
		
		success = vb_non_interleaved_pos.create(ctx, &(positions[0]), positions.size()) && success;
		success = vb_non_interleaved_normals.create(ctx, &(normals[0]), normals.size()) && success;
		success = vertex_array_non_interleaved.create(ctx) && success;


		success = vb_bonus_pos.create(ctx, &(positions_bonus[0]), positions_bonus.size()) && success;
		success = vb_bonus_normals.create(ctx, &(normals_bonus[0]), positions_bonus.size()) && success;
		success = vertex_array_bonus.create(ctx) && success;

		//Set attributearrays for interleaved vertex arrays
		success = vertex_array.set_attribute_array(
			ctx, default_shader.get_position_index(), vec3type, vb,
			0, // position is at start of the struct <-> offset = 0
			vertices.size(), // number of position elements in the array
			sizeof(vertex) // stride from one element to next
		) && success;

		success = vertex_array.set_attribute_array(
			ctx, default_shader.get_normal_index(), vec3type, vb,
			sizeof(cgv::render::render_types::vec3), // position is at start of the struct <-> offset = 0
			vertices.size(), // number of position elements in the array
			sizeof(vertex) // stride from one element to next
		) && success;


		//Set attribute arrays for noninterleaved vertex arrays
		success = vertex_array_non_interleaved.set_attribute_array(
			ctx, default_shader.get_position_index(), vec3type, vb_non_interleaved_pos,
			0, // position is at start of the struct <-> offset = 0
			positions.size(), // number of position elements in the array
			sizeof(cgv::render::render_types::vec3) // stride from one element to next
		) && success;

		success = vertex_array_non_interleaved.set_attribute_array(
			ctx, default_shader.get_normal_index(), vec3type, vb_non_interleaved_normals,
			0, // position is at start of the struct <-> offset = 0
			normals.size(), // number of position elements in the array
			sizeof(cgv::render::render_types::vec3) // stride from one element to next
		) && success;

		//Set attribute arrays for bonus task 1.3.1
		success = vertex_array_bonus.set_attribute_array(
			ctx, default_shader.get_position_index(), vec3type, vb_bonus_pos,
			0, // position is at start of the struct <-> offset = 0
			positions_bonus.size(), // number of position elements in the array
			sizeof(cgv::render::render_types::vec3) // stride from one element to next
		) && success;

		success = vertex_array_bonus.set_attribute_array(
			ctx, default_shader.get_normal_index(), vec3type, vb_bonus_normals,
			0, // position is at start of the struct <-> offset = 0
			normals_bonus.size(), // number of position elements in the array
			sizeof(cgv::render::render_types::vec3) // stride from one element to next
		) && success;


		// All initialization has been attempted
		return success;
	}


	// Part of the cgv::render::drawable interface, can be overwritten if there is some
	// work to be done before actually rendering a frame.
	void init_frame(cgv::render::context& ctx)
	{
		// Check if we need to recreate anything
		if (invalid)
		{
			init(ctx);
		}

	}

	void init_unit_cube(void) {

		vertices.resize(24);

		//Fill interleaved vertex array
		//Bottom face
		vertices[0].pos.set(-1, -1, -1);	vertices[0].normal.set(0, 1, 0);
		vertices[1].pos.set(1, -1, -1);		vertices[1].normal.set(0, 1, 0);
		vertices[2].pos.set(1, -1, 1);		vertices[2].normal.set(0, 1, 0);
		vertices[3].pos.set(-1, -1, 1);		vertices[3].normal.set(0, 1, 0);
		//Right face
		vertices[4].pos.set(1, -1, -1);		vertices[4].normal.set(1, 0, 0);
		vertices[5].pos.set(1, 1, -1);		vertices[5].normal.set(1, 0, 0);
		vertices[6].pos.set(1, 1, 1);		vertices[6].normal.set(1, 0, 0);
		vertices[7].pos.set(1, -1, 1);		vertices[7].normal.set(1, 0, 0);
		//Top face
		vertices[8].pos.set(1, 1, -1);		vertices[8].normal.set(0, -1, 0);
		vertices[9].pos.set(-1, 1, -1);		vertices[9].normal.set(0, -1, 0);
		vertices[10].pos.set(-1, 1, 1);		vertices[10].normal.set(0, -1, 0);
		vertices[11].pos.set(1, 1, 1);		vertices[11].normal.set(0, -1, 0);
		//Left face
		vertices[12].pos.set(-1, 1, -1);	vertices[12].normal.set(-1, 0, 0);
		vertices[13].pos.set(-1, -1, -1);	vertices[13].normal.set(-1, 0, 0);
		vertices[14].pos.set(-1, -1, 1);	vertices[14].normal.set(-1, 0, 0);
		vertices[15].pos.set(-1, 1, 1);		vertices[15].normal.set(-1, 0, 0);
		//Front face
		vertices[16].pos.set(-1, -1, 1);	vertices[16].normal.set(0, 0, 1);
		vertices[17].pos.set(1, -1, 1);		vertices[17].normal.set(0, 0, 1);
		vertices[18].pos.set(1, 1, 1);		vertices[18].normal.set(0, 0, 1);
		vertices[19].pos.set(-1, 1, 1);		vertices[19].normal.set(0, 0, 1);
		//Back face
		vertices[20].pos.set(-1, 1, -1);	vertices[20].normal.set(0, 0, -1);
		vertices[21].pos.set(1, 1, -1);		vertices[21].normal.set(0, 0, -1);
		vertices[22].pos.set(1, -1, -1);	vertices[22].normal.set(0, 0, -1);
		vertices[23].pos.set(-1, -1, -1);	vertices[23].normal.set(0, 0, -1);


		//Fill noninterleaved arrays
		positions.resize(24);
		normals.resize(24);

		//Bottom face
		positions[0].set(-1, -1, -1);		normals[0].set(0, 1, 0);
		positions[1].set(1, -1, -1);		normals[1].set(0, 1, 0);
		positions[2].set(1, -1, 1);			normals[2].set(0, 1, 0);
		positions[3].set(-1, -1, 1);		normals[3].set(0, 1, 0);
		//Right face						
		positions[4].set(1, -1, -1);		normals[4].set(1, 0, 0);
		positions[5].set(1, 1, -1);			normals[5].set(1, 0, 0);
		positions[6].set(1, 1, 1);			normals[6].set(1, 0, 0);
		positions[7].set(1, -1, 1);			normals[7].set(1, 0, 0);
		//Top face					
		positions[8].set(1, 1, -1);			normals[8].set(0, -1, 0);
		positions[9].set(-1, 1, -1);		normals[9].set(0, -1, 0);
		positions[10].set(-1, 1, 1);		normals[10].set(0, -1, 0);
		positions[11].set(1, 1, 1);			normals[11].set(0, -1, 0);
		//Left face						
		positions[12].set(-1, 1, -1);		normals[12].set(-1, 0, 0);
		positions[13].set(-1, -1, -1);		normals[13].set(-1, 0, 0);
		positions[14].set(-1, -1, 1);		normals[14].set(-1, 0, 0);
		positions[15].set(-1, 1, 1);		normals[15].set(-1, 0, 0);
		//Front face						
		positions[16].set(-1, -1, 1);		normals[16].set(0, 0, 1);
		positions[17].set(1, -1, 1);		normals[17].set(0, 0, 1);
		positions[18].set(1, 1, 1);			normals[18].set(0, 0, 1);
		positions[19].set(-1, 1, 1);		normals[19].set(0, 0, 1);
		//Back face							
		positions[20].set(-1, 1, -1);		normals[20].set(0, 0, -1);
		positions[21].set(1, 1, -1);		normals[21].set(0, 0, -1);
		positions[22].set(1, -1, -1);		normals[22].set(0, 0, -1);
		positions[23].set(-1, -1, -1);		normals[23].set(0, 0, -1);
		
	}

	//Unused method just for testing purposes 
	void draw_unit_cube(cgv::render::context& ctx) {
		vertex_array.enable(ctx);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertices.size());
		vertex_array.disable(ctx);
	}

	//METHODS FOR BONUS TASK 1.3.1

	void draw_cubes_fractal(cgv::render::context& ctx) {
		vertex_array_bonus.enable(ctx);
		glDrawArrays(GL_QUADS, 0, (GLsizei)positions_bonus.size());
		vertex_array_bonus.disable(ctx);
	}

	void init_cubes_fractal(cgv::render::context& ctx, unsigned max_depth, unsigned level, float scale,
		cgv::render::render_types::vec3 origin, cgv::render::render_types::vec3 direction) {

		scale *= .5f;

		add_cube(origin, scale);

		// Recurse
		unsigned num_children = level == 0 ? 4 : 3;
		if (level < max_depth) for (unsigned i = 0; i < num_children; i++)
		{

			direction = rotate_direction(direction, i);
			init_cubes_fractal(ctx, max_depth, level + 1, scale, origin + direction * 2 * scale, direction);

		}
	}
	
	cgv::render::render_types::vec3 rotate_direction(cgv::render::render_types::vec3 direction, int n) {
		auto d = direction;
		for (int i = 0; i < n; i++)
		{
			d = cgv::render::render_types::vec3(d.y(), -d.x(), 0);
		}
		return d;
	}

	void add_cube(cgv::render::render_types::vec3 offset, float scale) {
		//Bottom
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		//Right face
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		//top face
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		//left face
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		//front face
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (1 * scale) + offset.z());
		//back face
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());
		positions_bonus.emplace_back((-1 * scale) + offset.x(), (-1 * scale) + offset.y(), (-1 * scale) + offset.z());

		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(0, 1, 0);
		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(1, 0, 0);
		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(0, -1, 0);
		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(-1, 0, 0);
		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(0, 0, 1);
		for (int i = 0; i < 4; i++)
			normals_bonus.emplace_back(0, 0, -1);
	}
};


/// [END] Tasks 1.2a, 1.2b and 1.2c
/// ************************************************************************************/



/// ************************************************************************************/
/// Task 1.2a: register an instance of your drawable.


cgv::base::object_registration<cubes_drawable_scene> cubes_drawable_scene_registration("");
