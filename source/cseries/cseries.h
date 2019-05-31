#pragma once

/* ---------- constants */

enum
{
    NONE= -1,

    MAXIMUM_TAG_STRING_LENGTH= 32,
    MAXIMUM_LONG_STRING_LENGTH= 256,
};

/* ---------- macros */

#define SGN(x) ((x)?((x)<0?-1:1):0)

#define ABS(x) ((x>=0) ? (x) : -(x))

#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)>(b)?(a):(b))

#define FLOOR(n,floor) ((n)<(floor)?(floor):(n))
#define CEILING(n,ceiling) ((n)>(ceiling)?(ceiling):(n))
#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : CEILING(n,ceiling))

#define FLAG(b) (1<<(b))

#define TEST_FLAG(f, b) ((f)&FLAG(b))
#define SWAP_FLAG(f, b) ((f)^=FLAG(b))
#define SET_FLAG(f, b, v) ((v) ? ((f)|=FLAG(b)) : ((f)&=~FLAG(b)))
#define FLAG_RANGE(first_bit, last_bit) ((FLAG((last_bit)+1-(first_bit))-1)<<(first_bit))

#define UNUSED_PARAMETER(x) (x=x)

#define HIGH_WORD(n) (((n)>>16)&0xffff)
#define LOW_WORD(n) ((n)&0xffff)

/* ---------- types */

typedef enum { FALSE, TRUE } boolean;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long long qword;

typedef unsigned long tag;

typedef char tag_string[MAXIMUM_TAG_STRING_LENGTH];
typedef char long_string[MAXIMUM_LONG_STRING_LENGTH];

typedef unsigned long string_id;
