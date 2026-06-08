#pragma once
#define APP_VERSION "1.26.0608.1"
#define MASTER_KEY "THIS_IS_A_TEST_KEY"
#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_LEN 16
#define MAX_NICKNAME_LEN 64
#define MAX_SIGTEXT_LEN 128
#define SERVER_IP "118.89.19.108"
#define LINSTEN_PORT 12580
// 编码
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
//MSVC编译时，源码中的字符能保持Utf-8
#pragma execution_character_set("utf-8")
#endif

// 平台定义
#if (defined _WIN32) || (defined _WIN64)
#define OS_WIN
#pragma warning(disable: 4100)
#endif

#ifdef __linux__
#ifndef OS_LINUX
#define OS_LINUX
#endif
#endif

#ifdef __arm32
#define ARM32
#define ARM
#define OS_ARM
#endif

#ifdef __arm64
#define ARM64
#define OS_ARM
#endif

#ifdef __arm__
#define OS_ARM
#endif

// 类型定义
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef long long longlong;

#ifdef OS_WIN
#define CHAR16_T		wchar_t
#define pid_t			DWORD
#define pthread_t		DWORD
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#endif //OS_WIN

#ifdef OS_LINUX
#define CHAR16_T		wchar_t
#define MAX_PATH		260
#define _MAX_FNAME		256

#ifndef LLONG_MAX
#define LLONG_MAX		9223372036854775807	// maximum signed long long int value
#endif // LLONG_MAX

#ifndef INT_MAX
#define INT_MAX			2147483647			// maximum (signed) int value
#endif // INT_MAX

#ifndef UINT_MAX
#define UINT_MAX		0xFFFFFFFF
#endif // UINT_MAX

#define MAXINT			INT_MAX
#define HWND			int
#define SOCKET_ERROR	(-1)
#endif // OS_LINUX