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
#include "talabarte/logger.h"
#include "talabarte/event.h"

b8 platform_initialize_memory(void);
b8 platform_initialize_console(void);
b8 platform_initialize_window(void);

b8 platform_initialize() {
    if (!platform_initialize_memory()) return FALSE;
    if (!platform_initialize_console()) return FALSE;
    if (!platform_initialize_window()) return FALSE;

    return TRUE;
}

void platform_terminate_memory(void);
void platform_terminate_console(void);
void platform_terminate_window(void);

void platform_terminate() {
    platform_terminate_window();
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

// ##############################################################################
// window.h
// ##############################################################################
static HWND m_window;
static HINSTANCE m_hinstance;

LRESULT CALLBACK WndProc(HWND hwnd, u32 message, WPARAM wParam, LPARAM lParam);

b8 window_register_class(void);
b8 window_compute_size(struct Runtime* runtime);
b8 window_create(struct Runtime* runtime);
void window_center(struct Runtime* runtime);

b8 platform_initialize_window() {
    m_hinstance = GetModuleHandle(0);
    struct Runtime* runtime = runtime_get();

    if (!window_register_class()) return FALSE;
    if (!window_compute_size(runtime)) return FALSE;
    if (!window_create(runtime)) return FALSE;

    window_center(runtime);
    return TRUE;
}

void platform_terminate_window(void) {
    if (m_window != 0)
        DestroyWindow(m_window);
}

b8 window_register_class(void) {
    // Setup and register window class.
    HICON icon = LoadIcon(m_hinstance, IDI_APPLICATION);

    WNDCLASSW wc;
    platform_memory_set(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // Manage the cursor manually
    wc.hbrBackground = NULL;                   // Transparent
    wc.lpszClassName = L"talabarte_window_class";

    if (!RegisterClass(&wc)) { FATAL("Failed to register window class: 0x%x", GetLastError()) return FALSE; }
    return TRUE;
}

b8 window_compute_size(struct Runtime* runtime) {
    runtime->Platform.Window.title = runtime->Game->Window.title;

    switch (runtime->Game->Window.resolution) {
        case RESOLUTION_HD : runtime->Platform.Window.height =  720; break;
        case RESOLUTION_FHD: runtime->Platform.Window.height = 1080; break;
        case RESOLUTION_QHD: runtime->Platform.Window.height = 1440; break;
        case RESOLUTION_UHD: runtime->Platform.Window.height = 2160; break;
    }

    switch (runtime->Game->Window.aspectRatio) {
        case ASPECT_RATIO_4x3 : runtime->Platform.Window.width = (runtime->Platform.Window.height *  4) / 3; break;
        case ASPECT_RATIO_16x9: runtime->Platform.Window.width = (runtime->Platform.Window.height * 16) / 9; break;
        case ASPECT_RATIO_21x9: runtime->Platform.Window.width = (runtime->Platform.Window.height * 21) / 9; break;
    }

    return TRUE;
}

b8 window_create(struct Runtime* runtime) {
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // Grow by the size of the OS border.
    runtime->Platform.Window.width += border_rect.right - border_rect.left;
    runtime->Platform.Window.height += border_rect.bottom - border_rect.top;

    i32 length = strlen(runtime->Platform.Window.title);
    unsigned short unicode[length];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, runtime->Platform.Window.title, -1, unicode, length);

    m_window = CreateWindowEx(
        window_ex_style, 
        L"talabarte_window_class", 
        unicode, 
        window_style, 
        0, 0, 
        runtime->Platform.Window.width, runtime->Platform.Window.height, 
        0, 
        0, 
        m_hinstance, 0);

    if (m_window == 0) { FATAL("Failed to create window: 0x%x", GetLastError()) return FALSE; }
    return TRUE;
}

void window_center(struct Runtime* runtime) {
    runtime->Platform.Monitor.width = GetSystemMetrics(SM_CXSCREEN);
    runtime->Platform.Monitor.height = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(
        m_window, 
        HWND_TOP,                                                                   // Bring the window to the top of non-modals
        (runtime->Platform.Monitor.width - runtime->Platform.Window.width) / 2,     // X
        (runtime->Platform.Monitor.height - runtime->Platform.Window.height) / 2,   // Y
        0, 0,                                                                       // width, height
        SWP_NOSIZE                                                                  // Do not resize while moving
        );
}

void platform_window_show() {
    ShowWindow(m_window, SW_SHOW);
}

void platform_window_hide() {
    ShowWindow(m_window, SW_HIDE);
}

b8 platform_onEvent() {
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, u32 message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        // handled by the application to prevent flicker.
        case WM_ERASEBKGND: return 1; 
        case WM_CLOSE: { 
            struct Event event = {EVENT_APPLICATION_QUIT};
            event_fire(&event);
            return 0;
        } break;
        case WM_DESTROY: PostQuitMessage(0); return 0;
        case WM_SIZE: {
            // Get the updated size.
            // RECT r;
            // GetClientRect(hwnd, &r);
            // u32 width = r.right - r.left;
            // u32 height = r.bottom - r.top;

            // TODO: Event for window resize.
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Key pressed/released
            //b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            // TODO: Event for keyboard input

        } break;
        case WM_MOUSEMOVE: {
            // Mouse move
            //i32 x_position = GET_X_LPARAM(l_param);
            //i32 y_position = GET_Y_LPARAM(l_param);
            // TODO: Event for mouse move
        } break;
        case WM_MOUSEWHEEL: {
            // i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if (z_delta != 0) {
            //     // Flatten the input to an OS-independent (-1, 1)
            //     z_delta = (z_delta < 0) ? -1 : 1;
            //     // TODO: Event for mouse scroll
            // }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            //b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            // TODO: Event for mouse button
        } break;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#endif