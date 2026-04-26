#ifndef DEFINES_H
#define DEFINES_H

#ifdef API_EXPORT
// Exports
#	ifdef _MSC_VER
#		define _API __declspec(dllexport)
#	else
#		define _API __attribute__((visibility("default")))
#	endif
#else
// Imports
#	ifdef _MSC_VER
/** @brief Import/export qualifier */
#		define _API __declspec(dllimport)
#	else
/** @brief Import/export qualifier */
#		define _API
#	endif
#endif

#endif
