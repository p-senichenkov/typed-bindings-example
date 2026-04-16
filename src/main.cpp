#include <cstddef>
#include <pybind11/cast.h>
#include <pybind11/detail/common.h>
#include <pybind11/detail/descr.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include "algorithm.h"
#include "types.h"

namespace py = pybind11;

namespace pybind11::detail {
template <>
struct type_caster<TypedValue> {
    PYBIND11_TYPE_CASTER(TypedValue, const_name("Any"));

    static handle cast(TypedValue const& value, return_value_policy, handle) {
        return value.type->GetPythonObject(value.value);
    }
};
}  // namespace pybind11::detail

PYBIND11_MODULE(_core, mod, py::mod_gil_not_used()) {
    using namespace py::literals;

    py::class_<Algorithm>(mod, "Algorithm")
            .def(py::init<Metric&&, std::size_t>())
            .def("execute", &Algorithm::Execute);
}
