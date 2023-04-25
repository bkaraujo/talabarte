#include "talabarte/core/platform/detector.h"
#ifdef TALABARTE_PLATFORM_WINDOWS

#include <time.h>
#include <windows.h>
#include <windowsx.h>

#include "talabarte/core/runtime.h"
#include "talabarte/core/platform/lifecycle.h"
#include "talabarte/core/platform/console.h"
#include "talabarte/core/platform/memory.h"
#include "talabarte/core/platform/input.h"
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

    runtime->Platform.Monitor.width = GetSystemMetrics(SM_CXSCREEN);
    runtime->Platform.Monitor.height = GetSystemMetrics(SM_CYSCREEN);

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

    if (!RegisterClass(&wc)) { FATAL("RegisterClass(...): 0x%x", GetLastError()) return FALSE; }
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

    if (m_window == 0) { FATAL("CreateWindowEx(...): 0x%x", GetLastError()) return FALSE; }
    return TRUE;
}

void window_center(struct Runtime* runtime) {
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

b8 platform_update() {
    MSG message;

    // Dispatches incoming nonqueued messages, checks the thread message 
    // queue for a posted message, and retrieves the message (if any exist).
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        // Translates virtual-key messages into character messages. 
        // The character messages are posted to the calling thread's 
        // message queue, to be read the next time the thread calls 
        // the GetMessage or PeekMessage function.
        TranslateMessage(&message);

        // Dispatches a message to a window procedure.
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
            struct Event event = {message == WM_KEYDOWN || message == WM_SYSKEYDOWN ? EVENT_KEY_PRESSED : EVENT_KEY_RELEASED};
            event.payload.u16[0] = platform_input_key(wParam);
            event_fire(&event);
        } break;

        case WM_MOUSEMOVE: {
            struct Event event = {EVENT_MOUSE_MOVED};
            event.payload.i32[0] = GET_X_LPARAM(lParam);
            event.payload.i32[1] = GET_Y_LPARAM(lParam);
            
            event_fire(&event);
        } break;

        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (z_delta != 0) {
                struct Event event = {EVENT_MOUSE_SCROLLED};
                // Flatten the input to an OS-independent (-1, 1)
                event.payload.i32[0] = z_delta < 0 ? -1 : 1;
                event_fire(&event);
            }
        } break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            struct Event event = {WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN ? EVENT_MOUSE_PRESSED : EVENT_MOUSE_RELEASED};

            switch (message) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    event.payload.u8[0] = TALABARTE_BUTTON_1;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    event.payload.u8[0] = TALABARTE_BUTTON_2;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    event.payload.u8[0] = TALABARTE_BUTTON_3;
                    break;
            }

            event_fire(&event);
        } break;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#endif
// ##############################################################################
// input.h
// ##############################################################################

// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
enum TALABARTE_KEY platform_input_key(u16 hid) {
    switch (hid) {
        case VK_ESCAPE: return TALABARTE_KEY_ESCAPE;
        case VK_F1: return TALABARTE_KEY_F1;
        case VK_F2: return TALABARTE_KEY_F2;
        case VK_F3: return TALABARTE_KEY_F3;
        case VK_F4: return TALABARTE_KEY_F4;
        case VK_F5: return TALABARTE_KEY_F5;
        case VK_F6: return TALABARTE_KEY_F6;
        case VK_F7: return TALABARTE_KEY_F7;
        case VK_F8: return TALABARTE_KEY_F8;
        case VK_F9: return TALABARTE_KEY_F9;
        case VK_F10: return TALABARTE_KEY_F10;
        case VK_F11: return TALABARTE_KEY_F11;
        case VK_F12: return TALABARTE_KEY_F12;
        case VK_F13: return TALABARTE_KEY_F13;
        case VK_F14: return TALABARTE_KEY_F14;
        case VK_F15: return TALABARTE_KEY_F15;
        case VK_F16: return TALABARTE_KEY_F16;
        case VK_F17: return TALABARTE_KEY_F17;
        case VK_F18: return TALABARTE_KEY_F18;
        case VK_F19: return TALABARTE_KEY_F19;
        case VK_F20: return TALABARTE_KEY_F20;
        case VK_F21: return TALABARTE_KEY_F21;
        case VK_F22: return TALABARTE_KEY_F22;
        case VK_F23: return TALABARTE_KEY_F23;
        case VK_F24: return TALABARTE_KEY_F24;
        case VK_PRINT: return TALABARTE_KEY_PRINT;
        case VK_SCROLL: return TALABARTE_KEY_SCROLL_LOCK;
        case VK_PAUSE: return TALABARTE_KEY_PAUSE;

        case VK_OEM_3: return TALABARTE_KEY_OEM_3;
        case 0x31: return TALABARTE_KEY_1;
        case 0x32: return TALABARTE_KEY_2;
        case 0x33: return TALABARTE_KEY_3;
        case 0x34: return TALABARTE_KEY_4;
        case 0x35: return TALABARTE_KEY_5;
        case 0x36: return TALABARTE_KEY_6;
        case 0x37: return TALABARTE_KEY_7;
        case 0x38: return TALABARTE_KEY_8;
        case 0x39: return TALABARTE_KEY_9;
        case 0x30: return TALABARTE_KEY_0;
        case VK_OEM_MINUS: return TALABARTE_KEY_OEM_MINUS;
        case VK_OEM_PLUS: return TALABARTE_KEY_OEM_PLUS;
        case VK_BACK: return TALABARTE_KEY_OEM_BACKSPACE;
        case VK_INSERT: return TALABARTE_KEY_INSERT;
        case VK_HOME: return TALABARTE_KEY_HOME;
        case VK_PRIOR: return TALABARTE_KEY_PAGEUP;
        
        case VK_TAB: return TALABARTE_KEY_TAB;
        case 0x51: return TALABARTE_KEY_Q;
        case 0x57: return TALABARTE_KEY_W;
        case 0x45: return TALABARTE_KEY_E;
        case 0x52: return TALABARTE_KEY_R;
        case 0x54: return TALABARTE_KEY_T;
        case 0x59: return TALABARTE_KEY_Y;
        case 0x55: return TALABARTE_KEY_U;
        case 0x49: return TALABARTE_KEY_I;
        case 0x4F: return TALABARTE_KEY_O;
        case 0x50: return TALABARTE_KEY_P;
        case VK_OEM_4: return TALABARTE_KEY_OEM_4;
        case VK_OEM_5: return TALABARTE_KEY_OEM_5;
        case VK_RETURN: return TALABARTE_KEY_RETURN;
        case VK_DELETE: return TALABARTE_KEY_DELETE;
        case VK_END: return TALABARTE_KEY_END;
        case VK_NEXT: return TALABARTE_KEY_PAGEDOWN;

        case 0x41: return TALABARTE_KEY_A;
        case 0x42: return TALABARTE_KEY_B;
        case 0x43: return TALABARTE_KEY_C;
        case 0x44: return TALABARTE_KEY_D;
        case 0x46: return TALABARTE_KEY_F;
        case 0x47: return TALABARTE_KEY_G;
        case 0x48: return TALABARTE_KEY_H;
        case 0x4A: return TALABARTE_KEY_J;
        case 0x4B: return TALABARTE_KEY_K;
        case 0x4C: return TALABARTE_KEY_L;
        case 0x4D: return TALABARTE_KEY_M;
        case 0x4E: return TALABARTE_KEY_N;
        case 0x53: return TALABARTE_KEY_S;
        case 0x56: return TALABARTE_KEY_V;
        case 0x58: return TALABARTE_KEY_X;
        case 0x5A: return TALABARTE_KEY_Z;
    }

    return TALABARTE_KEY_MAXIMUM;
}