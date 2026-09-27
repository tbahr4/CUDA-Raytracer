//!=============================================================================
//! 
//! @file     Concepts.h
//! @brief    General template concepts
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include <concepts>
#include <type_traits>



namespace Raytracer::Util {

	//! @concept  Numeric
	//! @brief    Any number
	//! 
	template <typename T>
	concept Numeric = std::is_arithmetic_v<T> && !std::same_as<T, bool>;

} // namespace Raytracer::Util
