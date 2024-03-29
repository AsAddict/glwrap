#pragma once

#include <string>
#include <functional>

//#include <boost/format.hpp>
#include <sstream>

#include "../vector.hpp"
#include "../declarations.hpp"

namespace gl
{
namespace detail
{
namespace glslvar
{

typedef std::string type_name_t;

// glsl variable name suffix
template <typename T, typename Enable = void>
struct glsl_var_suffix;

template <typename T>
type_name_t get_type_suffix()
{
	return glsl_var_suffix<T>::suffix();
}

template <typename T>
struct glsl_var_suffix<T, std::enable_if<std::is_array<T>::value>>
{
	static type_name_t suffix()
	{
		return "";
	}
};

// glsl variable type name
template <typename T, typename Enable = void>
struct glsl_var_type;

template <typename T>
type_name_t get_type_name()
{
	return glsl_var_type<T>::name();
}

template <typename T>
struct glsl_var_type<T, typename std::enable_if<std::is_array<T>::value>::type>
{
	static type_name_t name()
	{
		return get_type_name<typename std::remove_extent<T>::type>();
	}
};

// scalars
template <>
inline type_name_t get_type_name<bool_t>()
{
	return "bool";
}

template <>
inline type_name_t get_type_name<int_t>()
{
	return "int";
}

template <>
inline type_name_t get_type_name<uint_t>()
{
	return "uint";
}

template <>
inline type_name_t get_type_name<float_t>()
{
	return "float";
}

template <>
inline type_name_t get_type_name<double_t>()
{
	return "double";
}

// vectors

template <typename T>
const char* vec_prefix();

template <>
inline const char* vec_prefix<bool_t>()
{
	return "b";
}

template <>
inline const char* vec_prefix<int_t>()
{
	return "i";
}

template <>
inline const char* vec_prefix<uint_t>()
{
	return "u";
}

template <>
inline const char* vec_prefix<float_t>()
{
	return "";
}

template <>
inline const char* vec_prefix<double_t>()
{
	return "d";
}

template <typename T, int D>
struct glsl_var_type<basic_vec<T, D>>
{
	static type_name_t name()
	{
#if 0
		return (boost::format("%svec%d") % vec_prefix<T>() % D).str();
#else
		std::ostringstream ss;
		ss << vec_prefix<T>() << "vec" << D;
		return ss.str();
#endif
	}
};

// matrix
template <typename T, int R, int C>
struct glsl_var_type<basic_mat<T, R, C>>
{
	static_assert(std::is_same<T, float_t>::value ||
		std::is_same<T, double_t>::value,
		"only float and double matrices");

	static type_name_t name()
	{
#if 0
		return (boost::format("%smat%dx%d") % vec_prefix<T>() % R % C).str();
#else
		std::ostringstream ss;
		ss << vec_prefix<T>() << "mat" << R << 'x' << C;
		return ss.str();
#endif
	}
};

template <>
inline type_name_t get_type_name<texture_1d>()
{
	return "sampler1D";
}

template <>
inline type_name_t get_type_name<texture_2d>()
{
	return "sampler2D";
}

template <>
inline type_name_t get_type_name<texture_rectangle>()
{
	return "sampler2DRect";
}

template <>
inline type_name_t get_type_name<texture_3d>()
{
	return "sampler3D";
}

template <>
inline type_name_t get_type_name<texture_cube_map>()
{
	return "samplerCube";
}

template <>
inline type_name_t get_type_name<texture_buffer>()
{
	return "samplerBuffer";
}

template <typename T, typename Enable = void>
struct index_count;

template <typename T>
struct index_count<T, typename std::enable_if<
	std::is_same<T, int_t>::value ||
	std::is_same<T, uint_t>::value ||
	std::is_same<T, float_t>::value
	, void>::type>
{
	static const std::size_t value = 1;
};

template <typename T>
struct index_count<T, typename std::enable_if<
	std::is_same<T, double_t>::value
	, void>::type>
{
	static const std::size_t value = 2;
};

template <typename T, int D>
struct index_count<basic_vec<T, D>>
{
	static const std::size_t value = index_count<T>::value;
};

// TODO: wrong!
template <typename T, int R, int C>
struct index_count<basic_mat<T, R, C>>
{
	static const std::size_t value = index_count<T>::value * R * C;
};

}
}
}
