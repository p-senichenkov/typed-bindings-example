#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "types.h"

using ColTypes = std::vector<std::unique_ptr<Type const>>;
using Row = std::vector<std::byte const*>;
using Rows = std::vector<Row>;
using StringRows = std::vector<std::vector<std::string>>;

class Table {
private:
    ColTypes column_types_;
    Rows rows_;

public:
    Table(ColTypes&& column_types, Rows&& rows)
        : column_types_(std::move(column_types)), rows_(std::move(rows)) {}

    ~Table() {
        for (auto const& row : rows_) {
            for (std::size_t i = 0; i < column_types_.size(); ++i) {
                column_types_[i]->FreeValue(row[i]);
            }
        }
    }

    ColTypes const& ColumnTypes() const {
        return column_types_;
    }

    Rows const& Rows() const {
        return rows_;
    }
};

template <typename... ConcreteTypes>
inline Table MakeTable(StringRows&& string_rows) {
    // Vector constructor copies its arguments, so unique ptrs cannot be initialized in-place
    std::vector<std::unique_ptr<Type const>> types;
    Type const* type_ptrs[sizeof...(ConcreteTypes)] = {new ConcreteTypes()...};
    for (auto const* type_ptr : type_ptrs) {
        types.emplace_back(std::unique_ptr<Type const>(type_ptr));
    }

    std::vector<std::vector<std::byte const*>> rows;
    for (auto const& str_row : string_rows) {
        auto& row = rows.emplace_back();
        for (std::size_t i = 0; i < types.size(); ++i) {
            row.emplace_back(types[i]->ValueFromString(str_row[i]));
        }
    }
    return Table{std::move(types), std::move(rows)};
}

static auto const kTestTable = MakeTable<IntType, IntType, StrType, StrType>(
        {{"1", "10", "a", "x"}, {"2", "20", "b", "xx"}, {"3", "30", "c", "xxx"}});
