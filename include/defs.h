#ifndef _DEFS_H_
#define	_DEFS_H_

typedef enum{
    false, true
}BOOL_TYPE;

typedef enum {
    NEUTRAL, DIR_A, DIR_B
} UNI_DIM_VECTOR_TYPE;

#define bool BOOL_TYPE
#define UniDimVector UNI_DIM_VECTOR_TYPE
#define NULL ((void*)0)

typedef unsigned char byte;
typedef signed int INT16;
typedef unsigned int UINT16;
typedef unsigned long INT32;
typedef unsigned long UINT32;

#endif	/* _DEFS_H_ */

