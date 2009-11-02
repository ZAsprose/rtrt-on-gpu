/*****************************************************************

	config.hpp

	Этот файл является частью библиотеки rt2

	Автор: Морозов Андрей

	Нижний Новгород, 2008-2009

*****************************************************************/
/**
	\file config.hpp
	\author Морозов Андрей
*/


#ifndef __config_hpp__
#define __config_hpp__

/// определяет, нужно ли собирать программу с использованием библиотеки MPI


//	#define _MPI_


/// определяет, нужно ли собирать программу с использованием библиотеки TBB

//	#define _TBB_

/// определяет, нужно ли использовать тип данных __m128 { инструкции SSE }
/// использовать вместе с precision_t = {float}

	#define m128_t

/// определяем использовать ли SSE3

//	#define use_sse3



typedef float precision_t;

typedef unsigned short         _uint16_t;
typedef short                  _sint16_t;

typedef unsigned int           _uint32_t;
typedef int                    _sint32_t;

typedef unsigned long long int _uint64_t;
typedef long long int          _sint64_t;

typedef unsigned long int _uint_t;
typedef long int          _sint_t;


#endif
