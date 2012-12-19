#ifndef KRV8_HPP
#define KRV8_HPP

#if __GNUC__ >= 4
	#if __GNUC_MINOR__ >= 6
		#pragma GCC diagnostic push
	#endif
	#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <v8.h>

#if __GNUC__ >= 4
	#if __GNUC_MINOR__ >= 6
		#pragma GCC diagnostic pop
	#else
		#pragma GCC diagnostic warning "-Wunused-parameter"
	#endif
#endif

#endif
