inline void __errExit (const char *format, ...) {

	va_list args;
	va_start( args, format );
	vfprintf( stderr, format, args );
	va_end( args );

	exit( 1 );

}


inline void __print (const char *format, ...) {

	va_list args;
	va_start( args, format );
	vfprintf( stderr, format, args );
	va_end( args );

}

