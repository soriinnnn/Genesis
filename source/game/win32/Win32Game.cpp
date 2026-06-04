#include <game/Game.h>
#include <windows.h>

using namespace genesis;
using namespace std;

void Game::run() 
{
    MSG msg = {};

    onCreate();
    m_previousTime = chrono::steady_clock::now();
    m_isRunning = true;

    if (!m_hasMainCamera) {
        GENESIS_LOG_WARNING("The main camera has not been set.");
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