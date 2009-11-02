/*****************************************************************

	config.hpp

	���� ���� �������� ������ ���������� rt2

	�����: ������� ������

	������ ��������, 2008-2009

*****************************************************************/
/**
	\file config.hpp
	\author ������� ������
*/


#ifndef __config_hpp__
#define __config_hpp__

/// ����������, ����� �� �������� ��������� � �������������� ���������� MPI


//	#define _MPI_


/// ����������, ����� �� �������� ��������� � �������������� ���������� TBB

//	#define _TBB_

/// ����������, ����� �� ������������ ��� ������ __m128 { ���������� SSE }
/// ������������ ������ � precision_t = {float}

	#define m128_t

/// ���������� ������������ �� SSE3

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
