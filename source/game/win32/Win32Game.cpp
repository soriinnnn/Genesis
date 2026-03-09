#include <game/Game.h>
#include <windows.h>

using namespace genesis;
using namespace std;

void Game::run() 
{
    MSG msg = {};
    
    m_previousTime = chrono::steady_clock::now();
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