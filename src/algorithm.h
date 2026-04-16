#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

#include "table.h"
#include "types.h"

using Metric = std::function<double(TypedValue const&, TypedValue const&)>;

class Algorithm {
private:
    Metric metric_;
    std::size_t column_index_;

public:
    explicit Algorithm(Metric&& metric, std::size_t column_index)
        : metric_(std::move(metric)), column_index_(column_index) {}

    void Execute() const {
        auto const& types = kTestTable.ColumnTypes();
        auto const* type = types[column_index_].get();

        for (auto const& first_row : kTestTable.Rows()) {
            auto const* first_val = first_row[column_index_];
            TypedValue first{first_val, type};
            for (auto const& second_row : kTestTable.Rows()) {
                auto const* second_val = second_row[column_index_];
                TypedValue second{second_val, type};
                auto dist = metric_(first, second);
                std::cout << "d(" << type->ValueToString(first_val) << ", "
                          << type->ValueToString(second_val) << ") = " << dist << '\n';
            }
        }
    }
};
