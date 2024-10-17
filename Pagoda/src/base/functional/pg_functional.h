#pragma once
#include "pgpch.h"

namespace Pagoda::Base {

	// Operators

	template <typename T>
	using UnaryOperator = std::function<T(const T&)>;

	template <typename T>
	using BinaryOperator = std::function<T(const T&, const T&)>;

	// Supplier

	template <typename T>
	using Supplier = std::function<T()>;

	// Consumers

	template <typename T>
	using Consumer = std::function<void(const T&)>;

	template <typename T, typename U>
	using BiConsumer = std::function<void(const T&, const U&)>;

	// Predicates

	template <typename T>
	using Predicate = std::function<bool(const T&)>;

	template <typename T, typename U>
	using BiPredicate = std::function<bool(const T&, const U&)>;

}