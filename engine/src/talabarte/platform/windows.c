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
// window.h
static HWND m_window;
static HINSTANCE m_hinstance;

b8 platform_initialize_memory(void);
b8 platform_initialize_console(void);
b8 platform_initialize_window(void);

b8 platform_initialize() {
    if (!platform_initialize_memory()) return FALSE;
    if (!platform_initialize_console()) return FALSE;
    if (!platform_initialize_window()) return FALSE;
    return TRUE;
}

b8 platform_initialize_console(void) {
    m_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(m_hconsole, &m_csbi);

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
    DestroyWindow(m_window);
}

b8 platform_initialize_memory(void) {
    m_heap = GetProcessHeap();
    return TRUE;
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

LRESULT CALLBACK onEvent(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 platform_initialize_window() {
    m_hinstance = GetModuleHandle(0);

    // Setup and register window class.
    HICON icon = LoadIcon(m_hinstance, IDI_APPLICATION);

    WNDCLASSA wc;
    platform_memory_set(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = onEvent;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
    wc.hbrBackground = NULL;                   // Transparent
    wc.lpszClassName = "talabarte_window_class";
    if (!RegisterClassA(&wc)) {
        FATAL("Failed to register window class: 0x%x", GetLastError())
        return FALSE;
    }
    
    // Create window
    u32 client_x = 100;
    u32 client_y = 100;
    u32 client_width = 1280;
    u32 client_height = 720;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    m_window = CreateWindowExA(window_ex_style, "talabarte_window_class", "Talabarte",window_style, window_x, window_y, window_width, window_height, 0, 0, m_hinstance, 0);
    if (m_window == 0) {
        FATAL("Failed to create window: 0x%x", GetLastError())
        return FALSE;
    }

    return TRUE;
}

void platform_window_show() {
    ShowWindow(m_window, SW_SHOW);
}

void platform_window_hide() {
    ShowWindow(m_window, SW_HIDE);
}

b8 platform_onEvent() {
    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    
    return TRUE;
}

LRESULT CALLBACK onEvent(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    return DefWindowProc(hwnd, msg, w_param, l_param);
}

#endif