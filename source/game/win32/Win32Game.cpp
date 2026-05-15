#include <game/Game.h>
#include <windows.h>

using namespace genesis;
using namespace std;
using namespace chrono;

void Game::run() 
{
    MSG msg = {};

    onCreate();
    m_previousTime = steady_clock::now();
    m_isRunning = true;

    if (m_mainCamera == GENESIS_INVALID_ENTITY) {
        GENESIS_LOG_WARNING("Main camera is not set.");
    }

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