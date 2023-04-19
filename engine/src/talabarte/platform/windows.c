#include "talabarte/platform/detector.h"
#ifdef TALABARTE_PLATFORM_WINDOWS

#include <time.h>
#include <windows.h>
#include <windowsx.h>

#include "talabarte/logger.h"
#include "talabarte/platform/lifecycle.h"
#include "talabarte/platform/console.h"
#include "talabarte/platform/memory.h"
#include "talabarte/platform/time.h"

// memory.h
static HANDLE m_heap;
// console.h
static HANDLE m_hconsole;
static CONSOLE_SCREEN_BUFFER_INFO m_csbi;

b8 platform_initialize() {
    // memory.h
    m_heap = GetProcessHeap();
    // console. h
    m_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(m_hconsole, &m_csbi);

    DEBUG("plataform_initialize()")

    return TRUE;
}

// FATAL,ERROR,WARN,INFO,DEBUG,TRACE
static u8 COLORS[6] = {FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_RED, 6, FOREGROUND_GREEN , FOREGROUND_BLUE, 8};
void platform_stdout(const char* message, u8 colour) {
    SetConsoleTextAttribute(m_hconsole, COLORS[colour]);
    
    DWORD lpNumberOfCharsWritten = 0;
    WriteConsole(m_hconsole, message, (DWORD) strlen(message), &lpNumberOfCharsWritten, 0);
    SetConsoleTextAttribute(m_hconsole, m_csbi.wAttributes);
}

void platform_terminate() {
    DEBUG("platform_terminate()")
}

void* platform_memory_allocate(u64 size) {
    return (void *) HeapAlloc(m_heap, HEAP_ZERO_MEMORY, size);
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

void platform_window_show() {
    
}

void platform_window_hide() {
    
}
#endif