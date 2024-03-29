#pragma once

#include <map>
#include <list>
#include <memory>

#include "variable.hpp"

namespace gl
{

class program;

namespace detail
{

class uniform_block_variable
{
	friend class gl::program;

public:
	std::string const& get_name() const
	{
		return m_name;
	}

private:
	uniform_block_variable(std::string const& _name)
		: m_name(_name)
		, m_location()
		, m_members()
	{}

	std::string m_name;
	uint_t m_location;
	// TODO: shared_ptr can be killed
	std::map<std::size_t, std::shared_ptr<variable_base>> m_members;
};

}

// TODO: name?
template <typename T>
class uniform_block_binding
{
	friend class uniform_block_binding_alloter;

public:
	uint_t get_index() const
	{
		return m_index;
	}

private:
	uniform_block_binding(uint_t _index)
		: m_index(_index)
	{}

	uint_t m_index;
};

// TODO: name?
class uniform_block_binding_alloter
{
public:
	// TODO: really need context?
	uniform_block_binding_alloter(context& _context)
		: m_current_index()
		, m_max_uniform_buffer_bindings()
	{
		GLint max_bindings{};
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max_bindings);
		m_max_uniform_buffer_bindings = max_bindings;
	}

	template <typename T>
	uniform_block_binding<T> allot()
	{
		if (m_current_index == m_max_uniform_buffer_bindings)
			throw exception();

		return {m_current_index++};
	}

private:
	uint_t m_current_index;
	uint_t m_max_uniform_buffer_bindings;
};

template <typename T>
class uniform_block_definition
{
	friend class program;

public:
	template <typename A>
	uniform_block_definition& operator()(A T::*_member, const std::string& _name)
	{
		register_member(_member, _name);
		return *this;
	}

	template <typename A>
	void register_member(A T::*_member, const std::string& _name)
	{
		m_members[detail::get_member_offset(_member)] =
			std::unique_ptr<detail::variable_base>(new detail::variable<A>(_name));
	}

private:
	// TODO: shared_ptr can be killed
	std::map<std::size_t, std::shared_ptr<detail::variable_base>> m_members;
};

template <typename T>
struct uniform_block
{
	friend class program;

public:
	std::string const& get_name() const
	{
		return m_iter->get_name();
	}

private:
	typedef std::list<detail::uniform_block_variable>::iterator iter_t;

	uniform_block(iter_t _iter)
		: m_iter(_iter)
	{}

	iter_t m_iter;
};

template <typename T>
struct uniform_block_align
{};

}
