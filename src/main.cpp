#define STATIC_CONCPP
#include "io_handler.h"
#include <cstdio>

int main( int, char ** )
{
	try
	{
		io_handler {}.run();
	}
	catch( err::scrooge_exception e )
	{
		std::printf( "Exception Type %s:\n\n%s\n\n", e.type(), e.what() );
	}
	catch( std::exception e )
	{
		std::printf( "%s", e.what() );
	}

	return 0;
}
