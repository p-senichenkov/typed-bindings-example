#pragma once

#include <any>
#include <cstddef>
#include <pybind11/detail/common.h>
#include <pybind11/pytypes.h>

class Type {
public:
    virtual ~Type() = default;
    virtual std::string ValueToString(std::byte const*) const = 0;
    virtual std::byte const* ValueFromString(std::string const&) const = 0;
    virtual void FreeValue(std::byte const*) const = 0;
    virtual std::any GetAnyValue(std::byte const*) const = 0;
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
    int GetInt(std::byte const* value) const {
        return GetValue<int>(value);
    }

    std::string ValueToString(std::byte const* value) const override {
        return std::to_string(GetInt(value));
    }

    std::byte const* ValueFromString(std::string const& str) const override {
        int value = std::stoi(str);
        return AllocateValue(std::move(value));
    }

    void FreeValue(std::byte const* value) const override {
        delete[] value;
    }

    std::any GetAnyValue(std::byte const* bytes) const override {
        return GetInt(bytes);
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

    std::any GetAnyValue(std::byte const* bytes) const override {
        return GetValue<std::string>(bytes);
    }
};

struct TypedValue {
    std::byte const* value;
    Type const* type;
};
