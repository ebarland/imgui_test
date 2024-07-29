#pragma once
#include <exception>
#include <string>
#include <format>
#include "exception_type.h"

namespace err
{
	struct scrooge_exception : public std::exception
	{
		scrooge_exception( int line, const char * file, std::string message, exception_type type ) noexcept : _what { std::format( "[Line] {}\n[File] {}\nMessage {}", line, file, message ) }, _type { type }
		{
		}

		const char * what() const noexcept
		{
			return _what.c_str();
		}

		const char * type() const noexcept
		{
			return parse_exception_type( _type );
		}

	private:
		const exception_type _type;
		const std::string _what;
	};
} // namespace err
