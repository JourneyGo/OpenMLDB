/*
 * Copyright 2021 4Paradigm
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_UDF_LITERAL_TRAITS_H_
#define SRC_UDF_LITERAL_TRAITS_H_

#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base/fe_status.h"
#include "codec/fe_row_codec.h"
#include "node/node_manager.h"
#include "node/sql_node.h"

namespace hybridse {
namespace udf {

using hybridse::base::Status;
using hybridse::codec::StringRef;
using hybridse::node::ExprListNode;
using hybridse::node::ExprNode;
using hybridse::node::SqlNode;

template <typename T>
struct DataTypeTrait {
    static std::string to_string();
    static node::TypeNode* to_type_node(node::NodeManager* nm);
    static node::ExprNode* to_const(node::NodeManager* nm, const T&);
    static T minimum_value();
    static T maximum_value();
};

struct AnyArg {
    AnyArg() = delete;
};

template <typename T>
struct Opaque {
    Opaque() = delete;
};

template <typename T>
struct Nullable {
    Nullable(std::nullptr_t) : data_(0), is_null_(true) {}  // NOLINT
    Nullable(const T& t) : data_(t), is_null_(false) {}     // NOLINT
    Nullable() : is_null_(false) {}

    const T& value() const { return data_; }
    bool is_null() const { return is_null_; }
    T* ptr() { return &data_; }

    T data_;
    bool is_null_;
};
template <>
struct Nullable<StringRef> {
    Nullable(std::nullptr_t) : data_(""), is_null_(true) {}      // NOLINT
    Nullable(const StringRef& t) : data_(t), is_null_(false) {}  // NOLINT
    Nullable() : is_null_(false) {}

    const StringRef& value() const { return data_; }
    bool is_null() const { return is_null_; }
    StringRef* ptr() { return &data_; }

    StringRef data_;
    bool is_null_;
};
template <typename T>
struct IsNullableTrait {
    static bool value;
};
template <typename T>
bool IsNullableTrait<T>::value = false;

template <typename T>
struct IsNullableTrait<Nullable<T>> {
    static bool value;
};

template <typename T>
bool IsNullableTrait<Nullable<T>>::value = true;

template <typename T>
static bool operator==(const Nullable<T>& x, const Nullable<T>& y) {
    if (x.is_null()) {
        return y.is_null();
    } else {
        return !y.is_null() && x.value() == y.value();
    }
}

template <typename... T>
struct Tuple {
    Tuple() {}
    Tuple(T&&... t)  // NOLINT
        : tuple(std::make_tuple(std::forward<T>(t)...)) {}
    std::tuple<T...> tuple;

    template <size_t I>
    auto& get() const {
        return std::get<I>(this->tuple);
    }
};

template <typename... T>
Tuple<T...> MakeTuple(T&&... t) {
    return Tuple<T...>(std::forward<T>(t)...);
}

template <typename... T>
static bool operator==(const Tuple<T...>& x, const Tuple<T...>& y) {
    return x.tuple == y.tuple;
}

template <>
struct DataTypeTrait<AnyArg> {
    static std::string to_string() { return "?"; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nullptr;
    }
};

template <>
struct DataTypeTrait<bool> {
    static std::string to_string() { return "bool"; }
    static node::DataType to_type_enum() { return node::kBool; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kBool);
    }
    static node::ExprNode* to_const(node::NodeManager* nm, const bool& v) {
        return nm->MakeConstNode(v);
    }
    static bool zero_value() { return false; }
    using CCallArgType = bool;
};

template <>
struct DataTypeTrait<int16_t> {
    static std::string to_string() { return "int16"; }
    static node::DataType to_type_enum() { return node::kInt16; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kInt16);
    }
    static int32_t codec_type_enum() { return hybridse::type::kInt16; }
    static node::ExprNode* to_const(node::NodeManager* nm, const int16_t& v) {
        return nm->MakeConstNode(v);
    }
    static int16_t minimum_value() {
        return std::numeric_limits<int16_t>::lowest();
    }
    static int16_t maximum_value() {
        return std::numeric_limits<int16_t>::max();
    }
    static int16_t zero_value() { return 0; }
    using CCallArgType = int16_t;
};

template <>
struct DataTypeTrait<int32_t> {
    static std::string to_string() { return "int32"; }
    static node::DataType to_type_enum() { return node::kInt32; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kInt32);
    }
    static int32_t codec_type_enum() { return hybridse::type::kInt32; }
    static node::ExprNode* to_const(node::NodeManager* nm, const int32_t& v) {
        return nm->MakeConstNode(v);
    }
    static int32_t minimum_value() {
        return std::numeric_limits<int32_t>::lowest();
    }
    static int32_t maximum_value() {
        return std::numeric_limits<int32_t>::max();
    }
    static int32_t zero_value() { return 0; }
    using CCallArgType = int32_t;
};

template <>
struct DataTypeTrait<int64_t> {
    static std::string to_string() { return "int64"; }
    static node::DataType to_type_enum() { return node::kInt64; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kInt64);
    }
    static int32_t codec_type_enum() { return hybridse::type::kInt64; }
    static node::ExprNode* to_const(node::NodeManager* nm, const int64_t& v) {
        return nm->MakeConstNode(v);
    }
    static int64_t minimum_value() {
        return std::numeric_limits<int64_t>::lowest();
    }
    static int64_t maximum_value() {
        return std::numeric_limits<int64_t>::max();
    }
    static int64_t zero_value() { return 0; }
    using CCallArgType = int64_t;
};

template <>
struct DataTypeTrait<float> {
    static std::string to_string() { return "float"; }
    static node::DataType to_type_enum() { return node::kFloat; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kFloat);
    }
    static int32_t codec_type_enum() { return hybridse::type::kFloat; }
    static node::ExprNode* to_const(node::NodeManager* nm, const float& v) {
        return nm->MakeConstNode(v);
    }
    static float minimum_value() {
        return std::numeric_limits<float>::lowest();
    }
    static float maximum_value() { return std::numeric_limits<float>::max(); }
    static float zero_value() { return 0; }
    using CCallArgType = float;
};

template <>
struct DataTypeTrait<double> {
    static std::string to_string() { return "double"; }
    static node::DataType to_type_enum() { return node::kDouble; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kDouble);
    }
    static int32_t codec_type_enum() { return hybridse::type::kDouble; }
    static node::ExprNode* to_const(node::NodeManager* nm, const double& v) {
        return nm->MakeConstNode(v);
    }
    static double minimum_value() {
        return std::numeric_limits<double>::lowest();
    }
    static double maximum_value() { return std::numeric_limits<double>::max(); }
    static double zero_value() { return 0; }
    using CCallArgType = double;
};

template <>
struct DataTypeTrait<codec::Timestamp> {
    static std::string to_string() { return "timestamp"; }
    static node::DataType to_type_enum() { return node::kTimestamp; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kTimestamp);
    }
    static node::ExprNode* to_const(node::NodeManager* nm,
                                    const codec::Timestamp& v) {
        return nm->MakeConstNode(v.ts_, node::kTimestamp);
    }
    static int32_t codec_type_enum() { return hybridse::type::kTimestamp; }
    static codec::Timestamp minimum_value() { return codec::Timestamp(0); }
    static codec::Timestamp maximum_value() {
        return codec::Timestamp(std::numeric_limits<int64_t>::max());
    }
    static codec::Timestamp zero_value() { return codec::Timestamp(0); }
    using CCallArgType = codec::Timestamp*;
};

template <>
struct DataTypeTrait<codec::Date> {
    static std::string to_string() { return "date"; }
    static node::DataType to_type_enum() { return node::kDate; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kDate);
    }
    static int32_t codec_type_enum() { return hybridse::type::kDate; }
    static node::ExprNode* to_const(node::NodeManager* nm,
                                    const codec::Date& v) {
        return nm->MakeConstNode(v.date_, node::kDate);
    }
    static codec::Date minimum_value() { return codec::Date(0); }
    static codec::Date maximum_value() {
        return codec::Date(std::numeric_limits<int32_t>::max());
    }
    static codec::Date zero_value() { return codec::Date(0); }
    using CCallArgType = codec::Date*;
};

template <>
struct DataTypeTrait<codec::StringRef> {
    static std::string to_string() { return "string"; }
    static node::DataType to_type_enum() { return node::kVarchar; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeTypeNode(node::kVarchar);
    }
    static int32_t codec_type_enum() { return hybridse::type::kVarchar; }
    using CCallArgType = codec::StringRef*;

    static node::ExprNode* to_const(node::NodeManager* nm,
                                    const std::string& str) {
        return nm->MakeConstNode(str);
    }
    static node::ExprNode* to_const(node::NodeManager* nm,
                                    const StringRef& str) {
        return nm->MakeConstNode(str.ToString());
    }
    static codec::StringRef zero_value() { return codec::StringRef(""); }
    static codec::StringRef minimum_value() { return codec::StringRef(""); }
};

template <typename T>
struct DataTypeTrait<codec::ListRef<T>> {
    static std::string to_string() {
        return "list_" + DataTypeTrait<T>::to_string();
    }
    static node::DataType to_type_enum() { return node::kList; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        auto list_type =
            nm->MakeTypeNode(node::kList, DataTypeTrait<T>::to_type_node(nm));
        list_type->generics_nullable_[0] = IsNullableTrait<T>::value;
        return list_type;
    }
    using CCallArgType = codec::ListRef<T>*;
};

template <typename T>
struct DataTypeTrait<Opaque<T>> {
    static std::string to_string() {
        return "opaque<" + std::to_string(sizeof(T)) + ">";
    }
    static node::DataType to_type_enum() { return node::kOpaque; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeOpaqueType(sizeof(T));
    }
    using CCallArgType = T*;
};

template <typename T>
struct DataTypeTrait<Nullable<T>> {
    static std::string to_string() { return DataTypeTrait<T>::to_string(); }
    static node::DataType to_type_enum() {
        return DataTypeTrait<T>::to_type_enum();
    }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return DataTypeTrait<T>::to_type_node(nm);
    }
};

template <typename... T>
struct DataTypeTrait<Tuple<T...>> {
    static std::string to_string() {
        std::stringstream ss;
        ss << "tuple_";
        size_t idx = 0;
        for (auto name : {DataTypeTrait<T>::to_string()...}) {
            ss << name;
            if (idx < sizeof...(T) - 1) {
                ss << "_";
            }
            idx += 1;
        }
        return ss.str();
    }
    static node::DataType to_type_enum() { return node::kTuple; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        auto tuple_type = nm->MakeTypeNode(node::kTuple);
        tuple_type->generics_ = {DataTypeTrait<T>::to_type_node(nm)...};
        tuple_type->generics_nullable_ = {IsNullableTrait<T>::value...};
        return tuple_type;
    }
    static node::ExprNode* to_const(node::NodeManager* nm,
                                    const Tuple<T...>& tuple) {
        return to_const_impl(nm, tuple, std::index_sequence_for<T...>());
    }

    template <size_t... I>
    static node::ExprNode* to_const_impl(node::NodeManager* nm,
                                         const Tuple<T...>& tuple,
                                         const std::index_sequence<I...>&) {
        std::vector<node::ExprNode*> fields(
            {DataTypeTrait<T>::to_const(nm, std::get<I>(tuple.tuple))...});
        return nm->MakeFuncNode("make_tuple", fields, nullptr);
    }
};

template <typename T>
struct CCallDataTypeTrait {
    using LiteralTag = T;
};
template <typename V>
struct CCallDataTypeTrait<V*> {
    using LiteralTag = Opaque<V>;
};
template <>
struct CCallDataTypeTrait<codec::Timestamp*> {
    using LiteralTag = codec::Timestamp;
};
template <>
struct CCallDataTypeTrait<codec::Date*> {
    using LiteralTag = codec::Date;
};
template <>
struct CCallDataTypeTrait<codec::StringRef*> {
    using LiteralTag = codec::StringRef;
};
template <typename V>
struct CCallDataTypeTrait<codec::ListRef<V>*> {
    using LiteralTag = codec::ListRef<V>;
};

template <>
struct CCallDataTypeTrait<codec::Timestamp> {
    using LiteralTag = AnyArg;
};
template <>
struct CCallDataTypeTrait<codec::Date> {
    using LiteralTag = AnyArg;
};
template <>
struct CCallDataTypeTrait<codec::StringRef> {
    using LiteralTag = AnyArg;
};
template <typename V>
struct CCallDataTypeTrait<codec::ListRef<V>> {
    using LiteralTag = AnyArg;
};

template <typename... LiteralArgTypes>
const std::string LiteralToArgTypesSignature() {
    std::stringstream ss;
    size_t idx = 0;
    for (auto type_str : {DataTypeTrait<LiteralArgTypes>::to_string()...}) {
        ss << type_str;
        if (idx < sizeof...(LiteralArgTypes) - 1) {
            ss << ", ";
        }
        idx += 1;
    }
    return ss.str();
}

template <typename... LiteralArgTypes>
codec::Schema MakeLiteralSchema() {
    codec::Schema schema;
    std::vector<int32_t> types = {
        DataTypeTrait<LiteralArgTypes>::codec_type_enum()...};
    for (size_t i = 0; i < types.size(); ++i) {
        ::hybridse::type::ColumnDef* col = schema.Add();
        col->set_name("col_" + std::to_string(i));
        col->set_type(static_cast<::hybridse::type::Type>(types[i]));
    }
    return schema;
}

template <typename... LiteralArgTypes>
struct LiteralTypedRow {
    int8_t* row_ptr;
    explicit LiteralTypedRow(int8_t* row_ptr) : row_ptr(row_ptr) {}
    explicit LiteralTypedRow(const codec::Row& row)
        : row_ptr(reinterpret_cast<int8_t*>(const_cast<codec::Row*>(&row))) {}
};

template <typename... LiteralArgTypes>
struct DataTypeTrait<LiteralTypedRow<LiteralArgTypes...>> {
    static std::string to_string() { return "row"; }
    static codec::Schema schema;
    static node::DataType to_type_enum() { return node::kRow; }
    static node::TypeNode* to_type_node(node::NodeManager* nm) {
        return nm->MakeRowType({&schema});
    }
};

template <typename... LiteralArgTypes>
codec::Schema DataTypeTrait<LiteralTypedRow<LiteralArgTypes...>>::schema =
    MakeLiteralSchema<LiteralArgTypes...>();

}  // namespace udf
}  // namespace hybridse

#endif  // SRC_UDF_LITERAL_TRAITS_H_
