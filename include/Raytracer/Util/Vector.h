//!=============================================================================
//! 
//! @file     Vector.h
//! @brief    Configurable-size mathematical vector
//! 
//! @details  
//! 
//! @note
//! 
//!=============================================================================
#pragma once

#include "Concepts.h"
#include <limits>



namespace Raytracer::Util {

	// Forward declarations
	template <Numeric T, std::size_t N>
	struct Vector;

	namespace internal {

		//! @name   FloatResultType
		//! @brief  Floating return type, dependent on numeric vector type used.
		//!			Matches the specified type if it is a floating-point type,
		//!			otherwise it remains a double by default
		//! 
		template <Numeric T>
		using FloatResultType = std::conditional_t<
			std::floating_point<T>,
			T,
			double
		>;

		//! @class  VectorBase
		//! @brief  Base vector implementation
		//! 
		template <Numeric T, std::size_t N>
		struct VectorBase {
			// Vector operations
			constexpr void Normalize()
				requires std::floating_point<T>;

			[[nodiscard]] constexpr Vector<T, N> Normalized() const
				requires std::floating_point<T>;

			constexpr void Negate();
			[[nodiscard]] constexpr Vector<T, N> Negated() const;
			FloatResultType<T> Magnitude() const;
			constexpr T Dot(const Vector<T, N>& rhs) const;
			FloatResultType<T> AngleBetween(const Vector<T, N>& rhs) const;

			// Operators
			constexpr T& operator[](std::size_t idx);
			constexpr const T& operator[](std::size_t idx) const;

			constexpr bool operator==(const Vector<T, N>& rhs) const;

			constexpr Vector<T, N> operator+() const;
			constexpr Vector<T, N> operator-() const;

			// Multiplication
			constexpr Vector<T, N> operator*(T scalar) const;
			constexpr friend Vector<T, N> operator*(T scalar, const Vector<T, N>& rhs);
			constexpr Vector<T, N>& operator*=(T scalar);
			constexpr Vector<T, N> operator*(const Vector<T, N>& rhs) const;
			constexpr Vector<T, N>& operator*=(const Vector<T, N>& rhs);

			// Division
			constexpr Vector<T, N> operator/(T scalar) const;
			constexpr friend Vector<T, N> operator/(T scalar, const Vector<T, N>& rhs);
			constexpr Vector<T, N>& operator/=(T scalar);
			constexpr Vector<T, N> operator/(const Vector<T, N>& rhs) const;
			constexpr Vector<T, N>& operator/=(const Vector<T, N>& rhs);

			// Addition
			constexpr Vector<T, N> operator+(T scalar) const;
			constexpr friend Vector<T, N> operator+(T scalar, const Vector<T, N>& rhs);
			constexpr Vector<T, N>& operator+=(T scalar);
			constexpr Vector<T, N> operator+(const Vector<T, N>& rhs) const;
			constexpr Vector<T, N>& operator+=(const Vector<T, N>& rhs);

			// Subtraction
			constexpr Vector<T, N> operator-(T scalar) const;
			constexpr friend Vector<T, N> operator-(T scalar, const Vector<T, N>& rhs);
			constexpr Vector<T, N>& operator-=(T scalar);
			constexpr Vector<T, N> operator-(const Vector<T, N>& rhs) const;
			constexpr Vector<T, N>& operator-=(const Vector<T, N>& rhs);
		};

	} // namespace internal


	//! @class  Vector
	//! @brief  Mathematical vector with a configurable size
	//! 
	template <Numeric T, std::size_t N>
	struct Vector : internal::VectorBase<T, N> {
		static_assert(N >= 2, "Size must be at least 2");

		T data[N];	//! Raw data buffer
		
		constexpr Vector();

		template <typename... Args>
			requires (sizeof...(Args) == N)
		constexpr Vector(Args... args);
	};

	//! @class  Vector2
	//! @brief  Fixed 2-element mathematical vector
	//! 
	template <Numeric T>
	struct Vector<T, 2> : internal::VectorBase<T, 2> {
		union {
			struct {
				T x, y;
			};
			T data[2];
		};

		// Constructors
		constexpr Vector();
		constexpr Vector(T x, T y);
	};

	//! @class  Vector3
	//! @brief  Fixed 3-element mathematical vector
	//! 
	template <Numeric T>
	struct Vector<T, 3> : internal::VectorBase<T, 3> {
		union {
			struct {
				T x, y, z;
			};
			T data[3];
		};

		// Constructors
		constexpr Vector();
		constexpr Vector(T x, T y, T z);

		// Vector operations
		[[nodiscard]] constexpr Vector<T, 3> Cross(const Vector<T, 3>& rhs) const;
	};

	//! @class  Vector
	//! @brief  4-element vector specialization
	//! 
	template <Numeric T>
	struct Vector<T, 4> : internal::VectorBase<T, 4> {
		union {
			struct {
				T x, y, z, w;
			};
			T data[4];
		};

		// Constructors
		constexpr Vector();
		constexpr Vector(T x, T y, T z, T w);
	};

	// CTAD type inference
	template <typename... Args>
	Vector(Args...) -> Vector<
		std::common_type_t<Args...>,
		sizeof...(Args)
	>;

	// Common vector aliases
	template <Numeric T>
	using Vector2 = Vector<T, 2>;

	template <Numeric T>
	using Vector3 = Vector<T, 3>;

	template <Numeric T>
	using Vector4 = Vector<T, 4>;

} // namespace Raytracer::Util


namespace Raytracer::Util {

	namespace internal {

		//! @fn     Normalize
		//! @brief  Normalizes the vector to have a magnitude of 1
		//! 
		template <Numeric T, std::size_t N>
		constexpr void VectorBase<T, N>::Normalize()
			requires std::floating_point<T> {
			FloatResultType<T> magnitude = Magnitude();
			if (magnitude == 0) {
				return;
			}
			
			*this /= magnitude;
		}

		//! @fn     Normalize
		//! @brief  Returns a normalized vector with a magnitude of 1
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::Normalized() const
			requires std::floating_point<T> {
			FloatResultType<T> magnitude = Magnitude();
			if (magnitude == 0) {
				return Vector<T, N>();
			}

			return static_cast<const Vector<T, N>&>(*this) / magnitude;
		}

		//! @fn     Negate
		//! @brief  Reverses the sign of all values
		//! 
		template <Numeric T, std::size_t N>
		constexpr void VectorBase<T, N>::Negate() {
			*this *= -1;
		}

		//! @fn     Negated
		//! @brief  Returns the vector with all values negated
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::Negated() const {
			return -static_cast<const Vector<T, N>&>(*this);
		}

		//! @fn     Magnitude
		//! @brief  Returns the magnitude of the vector
		//! 
		template <Numeric T, std::size_t N>
		FloatResultType<T> VectorBase<T, N>::Magnitude() const {
			T sum{};
			for (std::size_t i = 0; i < N; ++i) {
				sum += (*this)[i] * (*this)[i];
			}
			return std::sqrt(sum);
		}

		//! @fn     Dot
		//! @brief  Returns the dot product of the vector
		//! 
		template <Numeric T, std::size_t N>
		constexpr T VectorBase<T, N>::Dot(const Vector<T, N>& rhs) const {
			T sum{};
			for (std::size_t i = 0; i < N; ++i) {
				sum += (*this)[i] * rhs[i];
			}
			return sum;
		}

		//! @fn     AngleBetween
		//! @brief  Returns the absolute angle between this and another vector
		//! 
		template <Numeric T, std::size_t N>
		FloatResultType<T> VectorBase<T, N>::AngleBetween(const Vector<T, N>& rhs) const {
			FloatResultType<T> magLhs = Magnitude();
			FloatResultType<T> magRhs = rhs.Magnitude();

			if (magLhs == 0 || magRhs == 0) {
				return std::numeric_limits<FloatResultType<T>>::quiet_NaN();
			}

			FloatResultType<T> cosine = Dot(rhs) / (magLhs * magRhs);
			return std::acos(
				std::clamp(cosine, FloatResultType<T>{-1}, FloatResultType<T>{1})
			);
		}

		//! @fn     operator[]
		//! @brief  Square bracket operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr T& VectorBase<T, N>::operator[](std::size_t idx) {
			return static_cast<Vector<T, N>&>(*this).data[idx];
		}

		//! @fn     operator[]
		//! @brief  Square bracket operator for constant references
		//! 
		template <Numeric T, std::size_t N>
		constexpr const T& VectorBase<T, N>::operator[](std::size_t idx) const {
			return static_cast<const Vector<T, N>&>(*this).data[idx];
		}

		//! @fn     operator==
		//! @brief  Equality operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr bool VectorBase<T, N>::operator==(const Vector<T, N>& rhs) const {
			for (std::size_t i = 0; i < N; ++i) {
				if ((*this)[i] != rhs.data[i]) {
					return false;
				}
			}
			return true;
		}

		//! @fn     operator+
		//! @brief  Unary plus operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator+() const {
			return static_cast<const Vector<T, N>&>(*this);
		}

		//! @fn     operator-
		//! @brief  Unary minus operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator-() const {
			return static_cast<const Vector<T, N>&>(*this) * -T{1};
		}

		//! @fn     operator*
		//! @brief  RHS Scalar multiplication operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator*(T scalar) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result *= scalar;
			return result;
		}

		//! @fn     operator*
		//! @brief  LHS Scalar multiplication operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> operator*(T scalar, const Vector<T, N>& rhs) {
			return rhs * scalar;
		}

		//! @fn     operator*=
		//! @brief  Scalar multiplication assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator*=(T scalar) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] *= scalar;
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator*
		//! @brief  Vector multiplication operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator*(const Vector<T, N>& rhs) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result *= rhs;
			return result;
		}

		//! @fn     operator*=
		//! @brief  Vector multiplication assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator*=(const Vector<T, N>& rhs) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] *= rhs[i];
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator/
		//! @brief  RHS Scalar division operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator/(T scalar) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result /= scalar;
			return result;
		}

		//! @fn     operator/
		//! @brief  LHS Scalar division operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> operator/(T scalar, const Vector<T, N>& rhs) {
			Vector<T, N> result;
			
			for (std::size_t i = 0; i < N; ++i) {
				result[i] = scalar / rhs[i];
			}
			return result;
		}

		//! @fn     operator/=
		//! @brief  Scalar division assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator/=(T scalar) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] /= scalar;
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator/
		//! @brief  Vector division operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator/(const Vector<T, N>& rhs) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result /= rhs;
			return result;
		}

		//! @fn     operator/=
		//! @brief  Vector division assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator/=(const Vector<T, N>& rhs) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] /= rhs[i];
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator+
		//! @brief  RHS Scalar addition operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator+(T scalar) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result += scalar;
			return result;
		}

		//! @fn     operator+
		//! @brief  LHS Scalar addition operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> operator+(T scalar, const Vector<T, N>& rhs) {
			return rhs + scalar;
		}

		//! @fn     operator+=
		//! @brief  Scalar addition assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator+=(T scalar) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] += scalar;
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator+
		//! @brief  Vector addition operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator+(const Vector<T, N>& rhs) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result += rhs;
			return result;
		}

		//! @fn     operator+=
		//! @brief  Vector addition assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator+=(const Vector<T, N>& rhs) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] += rhs[i];
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator-
		//! @brief  RHS Scalar subtraction operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator-(T scalar) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result -= scalar;
			return result;
		}

		//! @fn     operator-
		//! @brief  LHS Scalar subtraction operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> operator-(T scalar, const Vector<T, N>& rhs) {
			Vector<T, N> result;

			for (std::size_t i = 0; i < N; ++i) {
				result[i] = scalar - rhs[i];
			}
			return result;
		}

		//! @fn     operator-=
		//! @brief  Scalar subtraction assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator-=(T scalar) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] -= scalar;
			}
			return static_cast<Vector<T, N>&>(*this);
		}

		//! @fn     operator-
		//! @brief  Vector subtraction operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N> VectorBase<T, N>::operator-(const Vector<T, N>& rhs) const {
			auto result = static_cast<const Vector<T, N>&>(*this);
			result -= rhs;
			return result;
		}

		//! @fn     operator-=
		//! @brief  Vector subtraction assignment operator
		//! 
		template <Numeric T, std::size_t N>
		constexpr Vector<T, N>& VectorBase<T, N>::operator-=(const Vector<T, N>& rhs) {
			for (std::size_t i = 0; i < N; ++i) {
				(*this)[i] -= rhs[i];
			}
			return static_cast<Vector<T, N>&>(*this);
		}

	} // namespace internal


	//! @fn     Vector
	//! @brief  Constructor for N-size vector
	//! 
	template <Numeric T, std::size_t N>
	constexpr Vector<T, N>::Vector()
		: data{}
	{}

	//! @fn     Vector
	//! @brief  Parameterized constructor for N-size vector
	//! 
	template <Numeric T, std::size_t N>
	template <typename... Args>
		requires (sizeof...(Args) == N)
	constexpr Vector<T, N>::Vector(Args... args)
		: data{ static_cast<T>(args)... }
	{}

	//! @fn     Vector
	//! @brief  Constructor for 2-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 2>::Vector()
		: data{}
	{}

	//! @fn     Vector
	//! @brief  Parameterized constructor for 2-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 2>::Vector(T x, T y)
		: data{ x,y }
	{}

	//! @fn     Vector
	//! @brief  Constructor for 3-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 3>::Vector()
		: data{}
	{}

	//! @fn     Vector
	//! @brief  Parameterized constructor for 3-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 3>::Vector(T x, T y, T z)
		: data{ x,y,z }
	{}

	//! @fn     Cross
	//! @brief  Returns the cross product with given vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 3> Vector<T, 3>::Cross(const Vector<T, 3>& rhs) const {
		return Vector<T, 3>(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}

	//! @fn     Vector
	//! @brief  Constructor for 3-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 4>::Vector()
		: data{}
	{}

	//! @fn     Vector
	//! @brief  Parameterized constructor for 4-size vector
	//! 
	template <Numeric T>
	constexpr Vector<T, 4>::Vector(T x, T y, T z, T w)
		: data{ x,y,z,w }
	{}

} // namespace Raytracer::Util
