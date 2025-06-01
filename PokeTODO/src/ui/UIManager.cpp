#include "UIManager.h"
#include "../utils/UIUtils.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <limits>

void UIManager::displayMenu(const std::vector<std::string>& menuItems, int selectedItem, const std::vector<std::string>& rightPanelContent, const std::string& inputPrompt, std::vector<std::string> art) {
    system("cls");

    // 기본 ASCII 아트가 제공되지 않은 경우 기본값 사용
    if (art.empty()) {
        art = ASCII_ART;
    }

    const std::string BORDER_CHAR = u8"■";
    const int TOTAL_WIDTH = 110;
    const int MENU_PANEL_WIDTH = 28;
    const int CONTENT_PANEL_WIDTH = TOTAL_WIDTH - MENU_PANEL_WIDTH - 3;
    const int INNER_ASCII_WIDTH = TOTAL_WIDTH - 2;

    for (int i = 0; i < TOTAL_WIDTH; ++i) std::cout << BORDER_CHAR;
    std::cout << std::endl;

    std::cout << BORDER_CHAR << padToWidth("", INNER_ASCII_WIDTH) << BORDER_CHAR << std::endl;

    for (const auto& artLine : art) {
        int artLineWidth = 0;
        const char* ptr = artLine.c_str();
        while (*ptr) {
            if ((*ptr & 0xC0) != 0x80) {
                artLineWidth++;
            }
            ptr++;
        }
        int padding_total = INNER_ASCII_WIDTH - artLineWidth;
        int padding_left = (std::max)(0, padding_total / 2);
        int padding_right = (std::max)(0, padding_total - padding_left);
        std::cout << BORDER_CHAR;
        std::cout << padToWidth("", padding_left);
        std::cout << artLine;
        std::cout << padToWidth("", padding_right);
        std::cout << BORDER_CHAR << std::endl;
    }

    std::cout << BORDER_CHAR << padToWidth("", INNER_ASCII_WIDTH) << BORDER_CHAR << std::endl;

    for (int i = 0; i < TOTAL_WIDTH; ++i) std::cout << BORDER_CHAR;
    std::cout << std::endl;

    size_t num_menu_header_lines = 1;
    size_t num_menu_footer_lines = 1;
    size_t num_menu_items_lines = menuItems.size();
    size_t total_menu_lines = num_menu_header_lines + num_menu_items_lines + num_menu_footer_lines;

    size_t display_lines = (std::max)(total_menu_lines, rightPanelContent.size());
    if (!inputPrompt.empty()) { // 입력 프롬프트가 있다면 추가 공간 고려
        display_lines = (std::max)(display_lines, rightPanelContent.size() + 1);
    }
    display_lines = (std::max)(display_lines, (size_t)4);

    for (size_t i = 0; i < display_lines; ++i) {
        std::string menu_line_str;
        if (i == 0) {
            menu_line_str = u8"   < 메뉴 >";
        }
        else if ((i - num_menu_header_lines) < num_menu_items_lines) {
            std::string prefix = ((i - num_menu_header_lines) == selectedItem) ? u8" > " : u8"   ";
            menu_line_str = prefix + menuItems[i - num_menu_header_lines];
        }
        else if (i == (num_menu_header_lines + num_menu_items_lines)) {
            menu_line_str = u8" " + padToWidth("", MENU_PANEL_WIDTH - 2, '-') + u8" ";
        }

        std::string content_line_str;
        if (i < rightPanelContent.size()) {
            content_line_str = rightPanelContent[i];
        }
        else if (i == rightPanelContent.size() && !inputPrompt.empty()) {
            // 오른쪽 패널 내용 바로 다음에 입력 프롬프트 표시
            content_line_str = inputPrompt;
        }

        std::cout << BORDER_CHAR
            << padToWidth(menu_line_str, MENU_PANEL_WIDTH)
            << BORDER_CHAR
            << padToWidth(content_line_str, CONTENT_PANEL_WIDTH)
            << BORDER_CHAR << std::endl;
    }

    for (int i = 0; i < TOTAL_WIDTH; ++i) std::cout << BORDER_CHAR;
    std::cout << std::endl;

    std::cout << u8" 방향키로 이동, Enter로 선택 (ESC: 이전/종료)" << std::endl;
}

void UIManager::clearCin() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void UIManager::clearKeyboardBuffer() {
    while (_kbhit()) {
        _getch();
    }
} 