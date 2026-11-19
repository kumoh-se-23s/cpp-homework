#ifndef TRAITS_CONCEPT_H
#define TRAITS_CONCEPT_H
#include <concepts>

#include "../base/MyString.h"

template<typename Traits, typename T>
concept CompareTraits = requires(const T &left, const T &right) {
    { Traits::less(left, right) } -> std::convertible_to<bool>;
};

template<typename Traits, typename T>
concept RowTraits = CompareTraits<Traits, T> && requires(const T &item) {
    { Traits::row_key(item) } -> std::convertible_to<int>;
};

template<typename Traits, typename T>
concept ColumnTraits = CompareTraits<Traits, T> && requires(const T &item) {
    { Traits::has_column(item) } -> std::convertible_to<bool>;
    { Traits::column_key(item) } -> std::convertible_to<MyString>;
};

template<typename Traits, typename T>
concept MultiTraits = RowTraits<Traits, T> && ColumnTraits<Traits, T> && requires(const T &left, const T &right) {
    { Traits::same(left, right) } -> std::convertible_to<bool>;
};

#endif //TRAITS_CONCEPT_H
