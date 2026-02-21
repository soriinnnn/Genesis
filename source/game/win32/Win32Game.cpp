#include <game/Game.h>
#include <windows.h>

using namespace genesis;

void Game::run() 
{
    MSG msg = {};

    while (m_isRunning) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                m_isRunning = false;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        onInternalUpdate();
    }
}