﻿#include "LongUI.h"


// 摧毁弹出菜单
void LongUI::CUIPopupMenu::Destroy() noexcept {
    if (m_hMenu) {
        ::DestroyMenu(m_hMenu);
        m_hMenu = nullptr;
    }
}

// 直接创建才菜单
bool LongUI::CUIPopupMenu::Create() noexcept {
    assert(!m_hMenu && "cannot create again!");
    m_hMenu = ::CreatePopupMenu();
    return !!m_hMenu;
}

// 使用XML字符串创建菜单
bool LongUI::CUIPopupMenu::Create(const char * xml) noexcept {
    pugi::xml_document document;
    document.load_string(xml);
    auto re = document.load_string(xml);
    // 错误
    if (re.status) {
        assert(!"failed to load string");
        ::MessageBoxA(nullptr, re.description(), "<LongUI::CUIPopupMenu::Create>: Failed to Parse/Load XML", MB_ICONERROR);
        return false;
    }
    // 创建节点
    return this->Create(document.first_child());
}

// 使用XML节点创建菜单
bool LongUI::CUIPopupMenu::Create(pugi::xml_node node) noexcept {
    assert(!m_hMenu && "cannot create again!");
    m_hMenu = ::CreatePopupMenu();
    return !!m_hMenu;
}

// 添加物品
bool LongUI::CUIPopupMenu::AppendItem(const ItemProperties& prop) noexcept {
    return false;
}

// 显示菜单
void LongUI::CUIPopupMenu::Show(HWND parent) noexcept {
    POINT pt = { 0,0 }; ::GetCursorPos(&pt);
    // 置前
    ::SetForegroundWindow(parent);
    // 跟踪菜单项的选择
    auto index = ::TrackPopupMenu(m_hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, parent, nullptr);
    if (m_pItemProc) {
        m_pItemProc(index);
    }
}