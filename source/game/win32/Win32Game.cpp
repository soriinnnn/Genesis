#include <game/Game.h>
#include <game/Display.h>
#include <windows.h>

using namespace genesis;

void Game::run() 
{
    MSG msg = {};

    m_previousTime = std::chrono::steady_clock::now();
    while (m_isRunning) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                m_isRunning = false;
                break;
            }

            if (msg.message == WM_KEYDOWN && msg.wParam == VK_F5) {
                m_display->resize(1024, 768);
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        onInternalUpdate();
    }
}