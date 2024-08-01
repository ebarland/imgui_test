#include "gfx/window_manager.h"
#include <cstdio>

gfx::window_manager window_manager {};

int main( int, char ** )
{
	try
	{
		while( window_manager.compose() )
		{
		}
	}
	catch( err::spec_exception e )
	{
		std::printf( "Exception Type %s:\n\n%s\n\n", e.type(), e.what() );
	}
	catch( std::exception e )
	{
		std::printf( "%s", e.what() );
	}

	return 0;
}
