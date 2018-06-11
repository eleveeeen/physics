#ifndef _Common_h_
#define _Common_h_

#ifdef _WIN32

#include<Windows.h>
#include<stdio.h>
#include<tchar.h>

#else

#include<stdio.h>
#include<stdint.h>

#endif

#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<d3dx9.h>

typedef char				Int8;
typedef unsigned char		UInt8;
typedef short				Int16;
typedef unsigned short		UInt16;
typedef int					Int32;
typedef unsigned int		UInt32;
typedef long long			Int64;
typedef unsigned long long	Uint64;


//アラインメントされているデータかどうかの判定等

#define BYTES_ALIGN16(bytes) (((bytes)+15)&(~15))
#define BYTES_ALIGN128(bytes) (((bytes)+127)&(~127))

#define PTR_ALIGN16(ptr) (((uintptr_t)(ptr)+15)&(~15))
#define PTR_ALIGN128(ptr) (((uintptr_t)(ptr)+127)&(~127))

#define PTR_IS_ALIGNED16(ptr) (((uintptr_t)(ptr)&0x0f)==0)
#define PTR_IS_ALIGNED128(ptr) (((uintptr_t)(ptr)&0x7f)==0)

#define AVAILABLE_BYTES_ALIGN16(ptr,bytes)  (bytes-((uintptr_t)(ptr)&0x0f))
#define AVAILABLE_BYTES_ALIGN128(ptr,bytes) (bytes-((uintptr_t)(ptr)&0x7f))



#endif