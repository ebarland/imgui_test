#pragma once

namespace err
{
	enum struct exception_type
	{
		STANDARD,
		GFX,
		FREETYPE,
		DEVICE_REMOVED,
		WINDOW,
		DATABASE
	};

	static const char * parse_exception_type( exception_type type )
	{
		switch( type )
		{
		case exception_type::STANDARD:
			return "STANDARD";
		case exception_type::GFX:
			return "GFX";
		case exception_type::FREETYPE:
			return "FREETYPE";
		case exception_type::DEVICE_REMOVED:
			return "DEVICE_REMOVED";
		case exception_type::WINDOW:
			return "WINDOW";
		case err::exception_type::DATABASE:
			return "DATABASE";
		default:
			return "UNKNOWN";
		}
	}
} // namespace err
