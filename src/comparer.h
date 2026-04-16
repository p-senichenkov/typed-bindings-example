#pragma once

#include <any>
#include <cstddef>
#include <functional>

#include "types.h"

class Metric {
public:
    virtual ~Metric() = default;

    virtual double operator()(std::byte const* l, std::byte const* r) const = 0;

    virtual void SetType(Type const* type) {}
};

class AnyMetric : public Metric {
private:
    Type const* type_;
    std::function<double(std::any const&, std::any const&)> metric_;

public:
    AnyMetric(std::function<double(std::any const&, std::any const&)>&& metric)
        : metric_(std::move(metric)) {}

    void SetType(Type const* type) override {
        type_ = type;
    }

    double operator()(std::byte const* l, std::byte const* r) const override {
        return metric_(type_->GetAnyValue(l), type_->GetAnyValue(r));
    }
};
