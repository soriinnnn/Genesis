#include <input/win32/Win32InputManager.h>
#include <windows.h>

using namespace genesis;

static Key mapWin32ToGenesisKey(int vkCode);
static int mapGenesisKeyToWin32(Key key);
static MouseButton mapWin32ToGenesisMouse(int vkCode);
static int mapGenesisMouseToWin32(MouseButton button);

Win32InputManager::Win32InputManager(const InputManagerDesc& desc): InputManager(desc), m_currentKeys({}), m_previousKeys({}) 
{
    m_lostFocus = true;
    m_mouseVisible = true;
}

Win32InputManager::~Win32InputManager() {}

void Win32InputManager::update()
{
    if (!m_window.hasFocus()) {
        if (!m_lostFocus) {
            ShowCursor(true);
            memset(m_currentKeys, 0, sizeof(uint8) * KEYBOARD_STATE_SIZE);
            memset(m_previousKeys, 0, sizeof(uint8) * KEYBOARD_STATE_SIZE);
            m_lostFocus = true;
        }
        return;
    }

    updateKeyboard();
    updateMouse();
}

bool Win32InputManager::isKeyDown(Key key) const noexcept
{
    int vkCode = mapGenesisKeyToWin32(key);
    if (vkCode == 0) {
        return false;
    }

    return m_currentKeys[vkCode] & 0x80;
}

bool Win32InputManager::isKeyUp(Key key) const noexcept
{
    int vkCode = mapGenesisKeyToWin32(key);
    if (vkCode == 0) {
        return false;
    }

    return !(m_currentKeys[vkCode] & 0x80);
}

bool Win32InputManager::isKeyPressed(Key key) const noexcept
{
    int vkCode = mapGenesisKeyToWin32(key);
    if (vkCode == 0) {
        return false;
    }

    return (m_currentKeys[vkCode] & 0x80) && !(m_previousKeys[vkCode] & 0x80);
}

bool Win32InputManager::isKeyReleased(Key key) const noexcept
{
    int vkCode = mapGenesisKeyToWin32(key);
    if (vkCode == 0) {
        return false;
    }

    return !(m_currentKeys[vkCode] & 0x80) && (m_previousKeys[vkCode] & 0x80);
}

bool Win32InputManager::isMouseDown(MouseButton button) const noexcept
{
    int vkCode = mapGenesisMouseToWin32(button);
    if (vkCode == 0) {
        return false;
    }

    return m_currentKeys[vkCode] & 0x80;
}

bool Win32InputManager::isMouseUp(MouseButton button) const noexcept
{
    int vkCode = mapGenesisMouseToWin32(button);
    if (vkCode == 0) {
        return false;
    }

    return !(m_currentKeys[vkCode] & 0x80);
}

bool Win32InputManager::isMousePressed(MouseButton button) const noexcept
{
    int vkCode = mapGenesisMouseToWin32(button);
    if (vkCode == 0) {
        return false;
    }

    return (m_currentKeys[vkCode] & 0x80) && !(m_previousKeys[vkCode] & 0x80);
}

bool Win32InputManager::isMouseReleased(MouseButton button) const noexcept
{
    int vkCode = mapGenesisMouseToWin32(button);
    if (vkCode == 0) {
        return false;
    }

    return !(m_currentKeys[vkCode] & 0x80) && (m_previousKeys[vkCode] & 0x80);
}

Point Win32InputManager::getMousePosition() const noexcept
{
    return m_currentMousePos;
}

Point Win32InputManager::getMouseDelta() const noexcept
{
    return Point{m_currentMousePos.x - m_previousMousePos.x, m_currentMousePos.y - m_previousMousePos.y};
}

void Win32InputManager::setMousePosition(Point pos)
{
    if (m_lostFocus) {
        return;
    }

    POINT cursorPos{pos.x, pos.y};
    if (!ClientToScreen(static_cast<HWND>(m_window.getHandle()), &cursorPos)) {
        GENESIS_LOG_WARNING("ClientToScreen failed.\nError code: 0x{:08X}", GetLastError());
        return;
    }

    if (!SetCursorPos(cursorPos.x, cursorPos.y)) {
        GENESIS_LOG_WARNING("SetCursorPos failed.\nError code: 0x{:08X}", GetLastError());
    }

    m_currentMousePos = pos;
}

void Win32InputManager::setMouseVisibility(bool visible)
{
    if (m_lostFocus || m_mouseVisible == visible) {
        return;
    }

    ShowCursor(visible);
    m_mouseVisible = visible;
}

void Win32InputManager::updateKeyboard()
{
    memcpy(m_previousKeys, m_currentKeys, sizeof(uint8) * KEYBOARD_STATE_SIZE);
    if (!GetKeyboardState(m_currentKeys)) {
        GENESIS_LOG_WARNING("GetKeyboardState failed.\nError code: 0x{:08X}", GetLastError());
        return;
    }

    for (int i = 0; i < KEYBOARD_STATE_SIZE; i++) {
        bool isDown = (m_currentKeys[i] & 0x80);
        bool wasDown = (m_previousKeys[i] & 0x80);

        Key key = mapWin32ToGenesisKey(i);
        if (key == Key::Unknown) {
            continue;
        }

        if (isDown) {
            for (auto listener : m_listeners) {
                listener->onKeyDown(key);
            }
        }
        else if (wasDown) {
            for (auto listener : m_listeners) {
                listener->onKeyUp(key);
            }
        }
    }
}

void Win32InputManager::updateMouse()
{
    POINT pos{};
    if (!GetCursorPos(&pos)) {
        GENESIS_LOG_WARNING("GetCursorPos failed.\nError code: 0x{:08x}", GetLastError());
        return;
    }

    if (!ScreenToClient(static_cast<HWND>(m_window.getHandle()), &pos)) {
        GENESIS_LOG_WARNING("ScreenToClient failed.\nError code: 0x{:08x}", GetLastError());
        return;
    }

    m_previousMousePos = m_currentMousePos;
    m_currentMousePos = Point{pos.x, pos.y};

    if (m_lostFocus) {
        ShowCursor(false);
        m_previousMousePos = m_currentMousePos;
        m_lostFocus = false;
    }

    if (m_currentMousePos.x != m_previousMousePos.x || m_currentMousePos.y != m_previousMousePos.y) {
        Point delta = getMouseDelta();

        for (auto listener : m_listeners) {
            listener->onMouseMove(delta, m_currentMousePos);
        }
    }

    for (int i = 1; i < static_cast<int>(MouseButton::Count); i++) {
        MouseButton button = static_cast<MouseButton>(i);

        int vkCode = mapGenesisMouseToWin32(button);
        if (vkCode == 0) {
            continue;
        }

        bool isDown = (m_currentKeys[vkCode] & 0x80);
        bool wasDown = (m_previousKeys[vkCode] & 0x80);

        if (isDown && !wasDown) {
            for (auto listener : m_listeners) {
                listener->onMouseDown(button, m_currentMousePos);
            }
        }
        else if (!isDown && wasDown) {
            for (auto listener : m_listeners) {
                listener->onMouseUp(button, m_currentMousePos);
            }
        }
    }
}

/* STATIC FUNCTION DEFINITIONS */

static Key mapWin32ToGenesisKey(int vkCode)
{
	switch (vkCode) {
    case 'A': return Key::A; 
    case 'B': return Key::B; 
    case 'C': return Key::C;
    case 'D': return Key::D; 
    case 'E': return Key::E; 
    case 'F': return Key::F;
    case 'G': return Key::G; 
    case 'H': return Key::H; 
    case 'I': return Key::I;
    case 'J': return Key::J; 
    case 'K': return Key::K; 
    case 'L': return Key::L;
    case 'M': return Key::M; 
    case 'N': return Key::N; 
    case 'O': return Key::O;
    case 'P': return Key::P; 
    case 'Q': return Key::Q; 
    case 'R': return Key::R;
    case 'S': return Key::S; 
    case 'T': return Key::T; 
    case 'U': return Key::U;
    case 'V': return Key::V; 
    case 'W': return Key::W; 
    case 'X': return Key::X;
    case 'Y': return Key::Y; 
    case 'Z': return Key::Z;
    case '0': return Key::Num0; 
    case '1': return Key::Num1; 
    case '2': return Key::Num2;
    case '3': return Key::Num3; 
    case '4': return Key::Num4; 
    case '5': return Key::Num5;
    case '6': return Key::Num6; 
    case '7': return Key::Num7; 
    case '8': return Key::Num8;
    case '9': return Key::Num9;
    case VK_ESCAPE: return Key::Escape;
    case VK_SPACE: return Key::Space;
    case VK_RETURN: return Key::Enter;
    case VK_TAB: return Key::Tab;
    case VK_BACK: return Key::Backspace;
    case VK_LSHIFT: return Key::LeftShift;
    case VK_RSHIFT: return Key::RightShift;
    case VK_LCONTROL: return Key::LeftControl;
    case VK_RCONTROL: return Key::RightControl;
    case VK_LMENU: return Key::LeftAlt;
    case VK_RMENU: return Key::RightAlt;
    case VK_UP: return Key::Up;
    case VK_DOWN: return Key::Down;
    case VK_LEFT: return Key::Left;
    case VK_RIGHT: return Key::Right;
    default: return Key::Unknown;
	}
}

static int mapGenesisKeyToWin32(Key key)
{
	switch (key) {
    case Key::A: return 'A'; 
    case Key::B: return 'B'; 
    case Key::C: return 'C';
    case Key::D: return 'D'; 
    case Key::E: return 'E'; 
    case Key::F: return 'F';
    case Key::G: return 'G'; 
    case Key::H: return 'H'; 
    case Key::I: return 'I';
    case Key::J: return 'J'; 
    case Key::K: return 'K'; 
    case Key::L: return 'L';
    case Key::M: return 'M'; 
    case Key::N: return 'N'; 
    case Key::O: return 'O';
    case Key::P: return 'P'; 
    case Key::Q: return 'Q'; 
    case Key::R: return 'R';
    case Key::S: return 'S'; 
    case Key::T: return 'T'; 
    case Key::U: return 'U';
    case Key::V: return 'V'; 
    case Key::W: return 'W'; 
    case Key::X: return 'X';
    case Key::Y: return 'Y'; 
    case Key::Z: return 'Z';
    case Key::Num0: return '0'; 
    case Key::Num1: return '1'; 
    case Key::Num2: return '2';
    case Key::Num3: return '3'; 
    case Key::Num4: return '4'; 
    case Key::Num5: return '5';
    case Key::Num6: return '6'; 
    case Key::Num7: return '7'; 
    case Key::Num8: return '8';
    case Key::Num9: return '9';
    case Key::Escape: return VK_ESCAPE;
    case Key::Space: return VK_SPACE;
    case Key::Enter: return VK_RETURN;
    case Key::Tab: return VK_TAB;
    case Key::Backspace: return VK_BACK;
    case Key::LeftShift: return VK_LSHIFT;
    case Key::RightShift: return VK_RSHIFT;
    case Key::LeftControl: return VK_LCONTROL;
    case Key::RightControl: return VK_RCONTROL;
    case Key::LeftAlt: return VK_LMENU;
    case Key::RightAlt: return VK_RMENU;
    case Key::Up: return VK_UP;
    case Key::Down: return VK_DOWN;
    case Key::Left: return VK_LEFT;
    case Key::Right: return VK_RIGHT;
    default: return 0;
	}
}

static MouseButton mapWin32ToGenesisMouse(int vkCode)
{
    switch(vkCode) {
    case VK_LBUTTON: return MouseButton::Left;
    case VK_RBUTTON: return MouseButton::Right;
    case VK_MBUTTON: return MouseButton::Middle;
    case VK_XBUTTON1: return MouseButton::Extra1;
    case VK_XBUTTON2: return MouseButton::Extra2;
    default: return MouseButton::Unknown;
    }
}

static int mapGenesisMouseToWin32(MouseButton button)
{
    switch (button) {
    case MouseButton::Left: return VK_LBUTTON;
    case MouseButton::Right: return VK_RBUTTON;
    case MouseButton::Middle: return VK_MBUTTON;
    case MouseButton::Extra1: return VK_XBUTTON1;
    case MouseButton::Extra2: return VK_XBUTTON2;
    default: return 0;
    }
}