#ifndef _MATRICES_H_
#define _MATRICES_H_

typedef float ElemType;

const int MAT2_ELEMENTS_NUMBER = 4;

//  / a[0] a[1] \
//  \ a[2] a[3] /

const int MAT3_ELEMENTS_NUMBER = 9;

//  / a[0] a[1] a[2] \
//  | a[3] a[4] a[5] |
//  \ a[6] a[7] a[8] /

typedef ElemType mat2[MAT2_ELEMENTS_NUMBER];

typedef ElemType mat3[MAT3_ELEMENTS_NUMBER];

const ElemType mat2_zero[] = { 0, 0, 0, 0 };
const ElemType mat2_unit[] = { 1, 0, 0, 1 };

const ElemType mat3_zero[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const ElemType mat3_unit[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

void mat2_add(mat2 & out_resuls, mat2 in_a, mat2 in_b );
void mat2_sub(mat2 & out_resuls, mat2 in_a, mat2 in_b );

void mat3_add(mat3 & out_resuls, mat3 in_a, mat3 in_b );
void mat3_sub(mat3 & out_resuls, mat3 in_a, mat3 in_b );

#endif //_MATRICES_H_