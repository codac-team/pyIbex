#include <boost/python.hpp>
#include <numpy/arrayobject.h>
#include <boost/multi_array.hpp>
#include <boost/cstdint.hpp>
#include <complex>
#include <algorithm>
#include <stdexcept>

#include "ibex_PixelMap.h"
#include "ibex_CtcPixelMap.h"
using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python; 	

boost::shared_ptr<ibex::PixelMap> init_from_numpyArray(PyObject* obj, double x0, double y0, double lx, double ly) throw() {
	PyArrayObject* a;

  a = (PyArrayObject*)PyArray_FromObject(obj, NPY_UINT64, 2, 2);
  if (a == NULL) {
  	std::cerr << "Erreur conversion\n";
    throw boost::python::error_already_set();
  }
 

  npy_intp * dim = PyArray_DIMS(a);
  
  // unsigned int *data_ptr = (unsigned int *)PyArray_DATA(a);
  std::cerr << dim[0] << " " << dim[1] << std::endl;
  // npy_intp * strides_size = 
  boost::shared_ptr<ibex::PixelMap2D> ptr =  shared_ptr<ibex::PixelMap2D>(new ibex::PixelMap2D());
  ptr->set_origin(x0, y0);
  ptr->set_grid_size(dim[0], dim[1]);
  ptr->set_leaf_size(lx, ly);
  ptr->init();  
  for (int i = 0; i < dim[0]; i++){
  	for(int j = 0; j < dim[1]; j++){
  		(*ptr)(i, j) = *(unsigned int* )PyArray_GETPTR2(a, i, j);
  	}	
  }
  
  // delete dim;
  // delete strides_size;
  return ptr;
}

void print_II(PixelMap2D& p){
  std::cerr << p.grid_size_[0] << " " << p.grid_size_[1] << std::endl;
  std::cerr << p.leaf_size_[0] << " " << p.leaf_size_[1] << std::endl;

	for (int i = 0; i < p.grid_size_[0]; i++){
  	for(int j = 0; j < p.grid_size_[1]; j++){
  		std::cerr << p(i, j) << " ";
  	}	
  	std::cerr << "\n";
  }
}

int export_PixelMap(){
// Export PixelMap
		import_array();
		class_<ibex::PixelMap , boost::noncopyable, boost::shared_ptr<ibex::PixelMap2D> >("PixelMap2D", no_init);
    class_<ibex::PixelMap2D, bases<ibex::PixelMap> , boost::noncopyable, boost::shared_ptr<ibex::PixelMap2D> >("PixelMap2D", no_init)
            .def("__init__", make_constructor(&init_from_numpyArray) )
            .def("compute_integral_image", &PixelMap2D::compute_integral_image)
            .def("print", &print_II)
            ;
}
