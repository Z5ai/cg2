﻿#include <limits>
#include <cgv/math/fvec.h>
#include "implicit_primitive.h"


template <typename T>
struct sphere : public implicit_primitive<T>
{
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	sphere() { implicit_base<T>::gui_color = 0xFF8888; }
	std::string get_type_name() const { return "sphere"; }

	/// Evaluate the sphere quadric at p
	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 2.1a: Implement an algebraic function of p that evaluates to 0 on the
		//            unit sphere.

		f_p = p.x()*p.x() + p.y()*p.y() + p.z()*p.z() - 1;

		return f_p;
	}

	/// Evaluate the gradient of the sphere quadric at p
	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 2.1a: Return the gradient of the function at p.

		return grad_f_p;
	}

	void create_gui()
	{
		implicit_primitive<T>::create_gui();
	}
};

scene_factory_registration<sphere<double> > sfr_sphere("sphere;S");
