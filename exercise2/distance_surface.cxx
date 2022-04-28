﻿
#include <cgv/math/fvec.h>
#include "distance_surface.h"

// ======================================================================================
//  Task 2.2: GENERAL HINTS
//
//  The super class skeleton of distance_surface has a protected member called ::edges,
//  which contains a list of all edges defined in the skeleton. Similarily, the super
//  super class knot_vector has a member called points, which contains a list of all
//  points used by the edges, which skeleton::edges indexes into.
//  Also make sure to check the header file of the distance_surface class for useful
//  members.
//
// ======================================================================================

template <typename T>
typename distance_surface<T>::vec_type distance_surface<T>::get_edge_distance_vector(size_t i, const pnt_type &p) const
{
	vec_type v;

	// Task 2.2: Compute the distance vector from edge i to p.
    auto e = skeleton<T>::edges.at(i);
    pnt_type e_a = knot_vector<T>::points.at(e.first);
    pnt_type e_b = knot_vector<T>::points.at(e.second);
    vec_type e_v(e_a.x()-e_b.x(), e_a.y()-e_b.y(), e_a.z()-e_b.z());
    double e_l = length(e_v);

    pnt_type p_proj = e_a + ((p-e_a)*e_v)/(e_l*e_l);
    v.set(p.x()-p_proj.x(), p.y()-p_proj.y() ,p.z()-p_proj.z());
	return v;
}

template <typename T>
double distance_surface<T>::get_min_distance_vector (const pnt_type &p, vec_type& v) const
{
	double min_dist;

	// Task 2.2: Compute the minimum distance from the skeleton to p, and report the
	//           corresponding distance vector in v.
    v = get_edge_distance_vector(0,p);
    min_dist = length(v);

    double min_dist_temp;
    vec_type v_temp;
    for(int i=1; i<skeleton<T>::edges.size(); i++) {
        v_temp = get_edge_distance_vector(i, p);
        min_dist_temp = length(v_temp);
        if(min_dist_temp<min_dist){
            v = v_temp;
            min_dist = min_dist_temp;
        }
    }

    return min_dist;
}

template <typename T>
T distance_surface<T>::evaluate(const pnt_type& p) const
{
	double f_p = std::numeric_limits<double>::infinity();

	// Task 2.2: Evaluate the distance surface function at p.
    vec_type v;
    f_p = get_min_distance_vector(p, v) - r;

	return f_p;
}

template <typename T>
typename distance_surface<T>::vec_type distance_surface<T>::evaluate_gradient(const pnt_type& p) const
{
	vec_type grad_f_p(0, 0, 0);

	// Task 2.2: Return the gradient of the distance surface function at p.
    vec_type v;
    get_min_distance_vector(p, v);
    grad_f_p.set(v.x() ,v.y() ,v.z());

	return grad_f_p;
}

/// update helper variables for edge i
template <typename T>
void distance_surface<T>::update_edge_precomputations(size_t ei)
{
	edge_vector[ei] =
		  (knot_vector<T>::points)[(skeleton<T>::edges)[ei].second]
		- (knot_vector<T>::points)[(skeleton<T>::edges)[ei].first];
	edge_vector_inv_length[ei] = (T(1)/ edge_vector[ei].sqr_length()) * edge_vector[ei];
}

/// construct distance surface
template <typename T>
distance_surface<T>::distance_surface()
{
	r=0.5;
	gui_title_added = false;
}
/// reflect members to expose them to serialization
template <typename T>
bool distance_surface<T>::self_reflect(cgv::reflect::reflection_handler& rh)
{
	return
		skeleton<T>::self_reflect(rh) &&
		rh.reflect_member("r", r);
}

template <typename T>
void distance_surface<T>::append_edge_callback(size_t ei)
{
	edge_vector.push_back(vec_type(0,0,0));
	edge_vector_inv_length.push_back(vec_type(0,0,0));
	update_edge_precomputations(ei);
}
template <typename T>
void distance_surface<T>::edge_changed_callback(size_t ei)
{
	update_edge_precomputations(ei);
}
template <typename T>
void distance_surface<T>::position_changed_callback(size_t pi)
{
	for (unsigned ei=0; ei<(skeleton<T>::edges).size(); ei++) 
		if ((skeleton<T>::edges)[ei].first == pi || (skeleton<T>::edges)[ei].second == pi)
			update_edge_precomputations(ei);
}

template <typename T>
void distance_surface<T>::create_gui()
{
	if (!gui_title_added) {
		provider::add_view("distance surface", named::name)->set("color",0xFF8888);
		gui_title_added = true;
	}

	provider::add_member_control(this, "radius", r, "value_slider", "min=0;max=5;log=true;ticks=true");

	skeleton<T>::create_gui();
}

scene_factory_registration<distance_surface<double> > sfr_distance_surface("distance_surface;D");
