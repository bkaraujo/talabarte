#include "talabarte/core/platform/detector.h"
#ifdef TALABARTE_PLATFORM_WINDOWS

#include <time.h>
#include <windows.h>
#include <windowsx.h>

#include "talabarte/core/runtime.h"
#include "talabarte/core/platform/lifecycle.h"
#include "talabarte/core/platform/console.h"
#include "talabarte/core/platform/memory.h"
#include "talabarte/core/platform/time.h"
#include "talabarte/core/window/lifecycle.h"
#include "talabarte/logger.h"
#include "talabarte/event.h"

b8 platform_initialize_memory(void);
b8 platform_initialize_console(void);
b8 platform_initialize_window(void);

b8 platform_initialize() {
    if (!platform_initialize_memory()) return FALSE;
    if (!platform_initialize_console()) return FALSE;

    struct Runtime* runtime = runtime_get();
    runtime->Platform.Monitor.width = GetSystemMetrics(SM_CXSCREEN);
    runtime->Platform.Monitor.height = GetSystemMetrics(SM_CYSCREEN);

    return TRUE;
}

void platform_terminate_memory(void);
void platform_terminate_console(void);

void platform_terminate() {
    platform_terminate_console();
    platform_terminate_memory();
}

// ##############################################################################
// console.h
// ##############################################################################
static HANDLE m_hconsole;
static CONSOLE_SCREEN_BUFFER_INFO m_csbi;
// FATAL, ERROR, WARN, INFO, DEBUG, TRACE
static u8 COLORS[6] = {FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_RED, 6, FOREGROUND_GREEN , FOREGROUND_BLUE, 8};

b8 platform_initialize_console(void) {
    m_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(m_hconsole, &m_csbi);

    return TRUE;
}

void platform_stdout(const char* message, u8 colour) {
    i32 length = strlen(message);
    i32 needed = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, message, length, NULL, 0);
    
    u16 unicode[needed];
    i32 written = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, message, length, unicode, needed);
    if (needed != written) { OutputDebugString(TEXT("MultiByteToWideChar needed != written")); }

    DWORD lpNumberOfCharsWritten = 0;
    SetConsoleTextAttribute(m_hconsole, COLORS[colour]);
    WriteConsole(m_hconsole, unicode, length, &lpNumberOfCharsWritten, 0);
    SetConsoleTextAttribute(m_hconsole, m_csbi.wAttributes);
}

void platform_terminate_console(void) { }
// ##############################################################################
// memory.h
// ##############################################################################
static HANDLE m_heap;

b8 platform_initialize_memory(void) {
    m_heap = GetProcessHeap();
    return TRUE;
}

void* platform_memory_allocate(u64 size) {
    return (void*) HeapAlloc(m_heap, HEAP_ZERO_MEMORY, size);
}

void platform_memory_free(void* block) {
    HeapFree(m_heap, 0, block);
}

void* platform_memory_set(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* platform_memory_copy(void* source, void* target, u64 size) {
    return memcpy(target, source, size);
}

void platform_terminate_memory(void) {
    m_heap = NULL;
}

// ##############################################################################
// time.h
// ##############################################################################
u64 __get_epoch() {
    FILETIME ft;
    LARGE_INTEGER li;

    /* current system date and time with the highest possible level of precision (<1us). */
    GetSystemTimePreciseAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    return li.QuadPart - 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
}

u64 platform_time_millis(void) {
    return __get_epoch() / 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */
}

u64 platform_time_nanos(void) {
    return __get_epoch() / 100;
}

LocalTime platform_time_now(void) {
    SYSTEMTIME time;
    GetLocalTime(&time);

    LocalTime lt;
    lt.year = time.wYear;
    lt.month = time.wMonth;
    lt.day = time.wDay;
    lt.hour = time.wHour;
    lt.minute = time.wMinute;
    lt.second = time.wSecond;
    lt.millis = time.wMilliseconds;

    return lt;
}
#endif