#include "talabarte/core/platform/detector.h"
#ifdef TALABARTE_PLATFORM_WINDOWS
#include "talabarte/core/runtime.h"
#include "talabarte/core/window/lifecycle.h"
#include "talabarte/core/platform/memory.h"
#include "talabarte/core/graphics/lifecycle.h"

#include <time.h>
#include <windows.h>
#include <windowsx.h>

#include "talabarte/logger.h"
#include "talabarte/event.h"

static HWND m_window;
static HINSTANCE m_hinstance;

LRESULT CALLBACK WndProc(HWND hwnd, u32 message, WPARAM wParam, LPARAM lParam);

b8 window_register_class(void);
b8 window_create(struct Runtime* runtime);

b8 window_initialize() {
    m_hinstance = GetModuleHandle(0);
    struct Runtime* runtime = runtime_get();

    runtime->Platform.Window.title = runtime->Game->Window.title;
    
    graphics_compute_size(
        runtime->Game->Window.aspectRatio, 
        runtime->Game->Window.resolution, 
        &runtime->Platform.Window.width, 
        &runtime->Platform.Window.height
    );

    if (!window_register_class()) return FALSE;
    if (!window_create(runtime)) return FALSE;

    SetWindowPos(
        m_window, 
        HWND_TOP,                                                                   // Bring the window to the top of non-modals
        (runtime->Platform.Monitor.width - runtime->Platform.Window.width) / 2,     // X
        (runtime->Platform.Monitor.height - runtime->Platform.Window.height) / 2,   // Y
        0, 0,                                                                       // width, height
        SWP_NOSIZE                                                                  // Do not resize while moving
    );

    return TRUE;
}

void window_terminate(void) {
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
    wc.lpszClassName = TEXT("talabarte_window_class");

    if (!RegisterClass(&wc)) { 
        TFATAL("RegisterClass(...): 0x%x", GetLastError());
        return FALSE;
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
        TEXT("talabarte_window_class"), 
        unicode, 
        window_style, 
        0, 0, 
        runtime->Platform.Window.width, runtime->Platform.Window.height, 
        0, 
        0, 
        m_hinstance, 0);

    if (m_window == 0) { 
        TFATAL("CreateWindowEx(...): 0x%x", GetLastError()) ;
        return FALSE;
    }

    return TRUE;
}

void window_show() {
    ShowWindow(m_window, SW_SHOW);
}

void window_hide() {
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
        case WM_USERCHANGED:
        case WM_CLOSE: { 
            struct Event event = {EVENT_APPLICATION_QUIT};
            event_fire(&event);
            return 0;
        } break;
        case WM_DESTROY: PostQuitMessage(0); return 0;

        case WM_SIZE: {
            struct Event event;

            switch (wParam) {
                case SIZE_RESTORED : event.code = EVENT_WINDOW_RESIZED  ; break;
                case SIZE_MAXIMIZED: event.code = EVENT_WINDOW_MAXIMIZED; break;
                case SIZE_MINIMIZED: event.code = EVENT_WINDOW_ICONIFIED; break;
            }

            event.payload.u32[0] = LOWORD(lParam);
            event.payload.u32[1] = HIWORD(lParam);
            event_fire(&event);
        } break;
       
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            struct Event event = { message == WM_KEYDOWN || message == WM_SYSKEYDOWN ? EVENT_KEY_PRESSED : EVENT_KEY_RELEASED };
            event.payload.u16[0] = wParam;
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
            struct Event event = { WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN ? EVENT_MOUSE_PRESSED : EVENT_MOUSE_RELEASED };

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