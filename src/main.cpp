#include <cstddef>
#include <memory>
#include <pybind11/cast.h>
#include <pybind11/detail/common.h>
#include <pybind11/detail/descr.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include "algorithm.h"
#include "comparer.h"
#include "types.h"

namespace py = pybind11;

pybind11::handle GetPyValue(std::byte const* value, Type const* type) {
    if (auto int_type = dynamic_cast<IntType const*>(type)) {
        return PyLong_FromLong(int_type->GetInt(value));
    }
    if (auto str_type = dynamic_cast<StrType const*>(type)) {
        return PyUnicode_FromString(str_type->ValueToString(value).c_str());
    }
    assert(false);
}

class PyMetric : public Metric {
private:
    Type const* type_;
    py::object metric_;

public:
    PyMetric(py::object&& metric) : metric_(std::move(metric)) {}

    void SetType(Type const* type) override {
        type_ = type;
    }

    double operator()(std::byte const* l, std::byte const* r) const override {
        return py::cast<double>(metric_(GetPyValue(l, type_), GetPyValue(r, type_)));
    }
};

PYBIND11_MODULE(_core, mod, py::mod_gil_not_used()) {
    using namespace py::literals;

    py::class_<Algorithm>(mod, "Algorithm")
            .def(py::init<std::unique_ptr<Metric>&&, std::size_t>())
            .def("execute", &Algorithm::Execute);

    py::class_<Metric, py::smart_holder>(mod, "PrivateBaseTypeNevermind");
    py::class_<PyMetric, Metric, py::smart_holder>(mod, "Metric").def(py::init<py::object&&>());
}
