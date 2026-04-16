#pragma once

#include <cstddef>

#include <pybind11/detail/common.h>
#include <pybind11/pytypes.h>

class Type {
public:
    virtual ~Type() = default;
    virtual std::string ValueToString(std::byte const*) const = 0;
    virtual std::byte const* ValueFromString(std::string const&) const = 0;
    virtual void FreeValue(std::byte const*) const = 0;
    virtual pybind11::handle GetPythonObject(std::byte const*) const = 0;
};

template <typename T>
T const& GetValue(std::byte const* bytes) {
    return *reinterpret_cast<T const*>(bytes);
}

template <typename T>
std::byte* AllocateValue(T&& value) {
    std::byte* buf = new std::byte[sizeof(T)]();
    new (buf) T(std::move(value));
    return buf;
}

class IntType final : public Type {
public:
    std::string ValueToString(std::byte const* value) const override {
        return std::to_string(GetValue<int>(value));
    }

    std::byte const* ValueFromString(std::string const& str) const override {
        int value = std::stoi(str);
        return AllocateValue(std::move(value));
    }

    void FreeValue(std::byte const* value) const override {
        delete[] value;
    }

    pybind11::handle GetPythonObject(std::byte const* value) const override {
        return PyLong_FromLongLong(GetValue<int>(value));
    }
};

class StrType final : public Type {
private:
    using String = std::string;

public:
    std::string ValueToString(std::byte const* value) const override {
        return GetValue<std::string>(value);
    }

    std::byte const* ValueFromString(std::string const& str) const override {
        std::string cp{str};
        return AllocateValue(std::move(cp));
    }

    void FreeValue(std::byte const* value) const override {
        // Somehow std::~string doesn't work
        reinterpret_cast<std::string const*>(value)->~String();
    }

    pybind11::handle GetPythonObject(std::byte const* value) const override {
        return PyUnicode_FromString(GetValue<std::string>(value).c_str());
    }
};

struct TypedValue {
    std::byte const* value;
    Type const* type;
};
