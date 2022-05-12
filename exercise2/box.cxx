#include <cgv/math/fvec.h>
#include "implicit_primitive.h"


template <typename T>
struct box : public implicit_primitive<T>
{
	typedef typename implicit_base<T>::vec_type vec_type;
	typedef typename implicit_base<T>::pnt_type pnt_type;

	box() {}
	std::string get_type_name() const { return "box"; }
	void on_set(void* member_ptr) { implicit_base<T>::update_scene(); }

	/*********************************************************************************/
	/* Task 2.1a: If you need any auxiliary functions for this task, put them here.  */

	// < your code >

	/* [END] Task 2.1a
	/*********************************************************************************/

	/// Evaluate the implicit box function at p
	T evaluate(const pnt_type& p) const
	{
		double f_p = std::numeric_limits<double>::infinity();

		// Task 2.1a: Implement a function of p that evaluates to 0 on the unit cube.
		//            You may use any suitable distance metric.

		f_p = std::max(std::abs(p.x()), std::max(std::abs(p.y()), std::abs(p.z()))) - 1;

		return f_p;
	}

	/// Evaluate the gradient of the implicit box function at p
	vec_type evaluate_gradient(const pnt_type& p) const
	{
		vec_type grad_f_p(0, 0, 0);

		// Task 2.1a: Return the gradient of the function at p.

		int x_grad=0, y_grad=0, z_grad=0;

		if (std::max(std::abs(p.x()), std::max(std::abs(p.y()), std::abs(p.z()))) == std::abs(p.x())) {
			x_grad = 1;
			if (p.x() < 0) {
				x_grad = -1;
			}
		}
		if (std::max(std::abs(p.x()), std::max(std::abs(p.y()), std::abs(p.z()))) == std::abs(p.y())) {
			y_grad = 1;
			if (p.y() < 0) {
				y_grad = -1;
			}
		}
		if (std::max(std::abs(p.x()), std::max(std::abs(p.y()), std::abs(p.z()))) == std::abs(p.z())) {
			z_grad = 1;
			if (p.z() < 0) {
				z_grad = -1;
			}
		}

		
		grad_f_p.set(x_grad, y_grad, z_grad);

		return grad_f_p;
	}

	void create_gui()
	{
		implicit_primitive<T>::create_gui();
	}
};

scene_factory_registration<box<double> > sfr_box("box;B");
