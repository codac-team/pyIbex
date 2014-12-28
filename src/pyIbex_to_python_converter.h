// pyIbex_to_python_converter.h

#ifndef __PYIBEX_TO_PYTHON_CONVERTER_H__
#define __PYIBEX_TO_PYTHON_CONVERTER_H__

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include <ibex_Vector.h>
#include <vector>
namespace py = boost::python;

template< typename T >
inline
std::vector< T > to_std_vector( const py::object& iterable )
{
    return std::vector< T >( py::stl_input_iterator< T >( iterable ),
                             py::stl_input_iterator< T >( ) );
}

// Converts a std::pair instance to a Python tuple.
template <typename T1, typename T2>
struct std_pair_to_tuple
{
    static PyObject* convert(std::pair<T1, T2> const& p)
    {
        return boost::python::incref(
                    boost::python::make_tuple(p.first, p.second).ptr());
    }
    static PyTypeObject const *get_pytype () {return &PyTuple_Type; }
};

// Helper for convenience.
template <typename T1, typename T2>
struct std_pair_to_python_converter
{
    std_pair_to_python_converter()
    {
        boost::python::to_python_converter<
                std::pair<T1, T2>,
                std_pair_to_tuple<T1, T2>,
                true //std_pair_to_tuple has get_pytype
                >();
    }
};

struct Vector_to_list
{
    static PyObject* convert(const ibex::Vector& container)
    {
        py::list result;
        for(uint i = 0; i < container.size(); i++){
            result.append(container[i]);
        }
        return py::incref(result.ptr());
    }
};


struct Vector_from_python_list
{

    Vector_from_python_list()
    {
        using namespace boost::python;
        using namespace boost::python::converter;
        registry::push_back(&Vector_from_python_list::convertible,
                            &Vector_from_python_list::construct,
                            type_id<ibex::Vector>());
    }

    // Determine    if obj_ptr can be converted in a std::vector<T>
    static void* convertible(PyObject* obj_ptr)
    {
        if (!PyList_Check(obj_ptr)){
            return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<T>
    static void construct(
            PyObject* obj_ptr,
            boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        using namespace boost::python;
        // Extract the character data from the python string
        //      const char* value = PyString_AsString(obj_ptr);
        list l(handle<>(borrowed(obj_ptr)));

        // // Verify that obj_ptr is a string (should be ensured by convertible())
        // assert(value);

        // Grab pointer to memory into which to construct the new std::vector<T>
        void* storage = (
                    (boost::python::converter::rvalue_from_python_storage<ibex::Vector>*)
                    data)->storage.bytes;

        int le = len(l);
        // in-place construct the new std::vector<T> using the character data
        // extraced from the python object
        ibex::Vector& v = *(new (storage) ibex::Vector(le));

        // populate the vector from list contains !!!
        v.resize(le);
        for(int i = 0;i!=le;++i){
            v[i] = extract<double>(l[i]);
        }
        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};


#endif // __PYIBEX_TO_PYTHON_CONVERTER_H__
