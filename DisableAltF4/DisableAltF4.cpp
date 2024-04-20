#include <windows.h>
#include <thread>
#include <iostream>
#include <conio.h>

// Callback function to intercept keystrokes
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            if (pkbhs->vkCode == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x8000)) {
                // Do nothing when Alt + F4 is pressed
                return 1;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Sets a low-level keyboard hook and runs a message loop
void HookThread() {
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hhkLowLevelKybd);
}

void disableAltF4() {
    // Start the hook in a separate thread
    std::thread hookThread(HookThread);
    hookThread.detach();  // Allow the thread to run independently
}

int main()
{
    disableAltF4(); // Disables Alt + F4 temporarily

    // Press any key to exit
    std::cout << "Press any key to exit..." << std::endl;
    _getch();


    return 0;
}