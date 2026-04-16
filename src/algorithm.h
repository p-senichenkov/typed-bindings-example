#pragma once

#include <cstddef>
#include <iostream>
#include <memory>

#include "comparer.h"
#include "table.h"
#include "types.h"

class Algorithm {
private:
    std::unique_ptr<Metric> metric_;
    std::size_t column_index_;

public:
    explicit Algorithm(std::unique_ptr<Metric>&& metric, std::size_t column_index)
        : metric_(std::move(metric)), column_index_(column_index) {
        metric_->SetType(kTestTable.ColumnTypes()[column_index_].get());
    }

    void Execute() const {
        auto const& types = kTestTable.ColumnTypes();
        auto const* type = types[column_index_].get();

        for (auto const& first_row : kTestTable.Rows()) {
            auto const* first_val = first_row[column_index_];
            for (auto const& second_row : kTestTable.Rows()) {
                auto const* second_val = second_row[column_index_];
                auto dist = (*metric_)(first_val, second_val);
                std::cout << "d(" << type->ValueToString(first_val) << ", "
                          << type->ValueToString(second_val) << ") = " << dist << '\n';
            }
        }
    }
};
