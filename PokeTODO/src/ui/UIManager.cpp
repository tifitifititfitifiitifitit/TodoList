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

        // 왼쪽 패널과 오른쪽 패널 출력 방식을 수정했습니다.
        // 선택된 옵션의 색을 바꿔서 출력하기 위함입니다.
        // 덕분에 코드가 많이 지저분해지고 읽기 힘들어졌다고 생각합니다만, 그 부분은 죄송합니다.

        std::cout << BORDER_CHAR;

        std::string outputString = padToWidth(menu_line_str, MENU_PANEL_WIDTH);
        size_t findPos = outputString.find('>'); // '>'의 위치 찾기

        // 현재 출력하고자 하는 옵션이 선택된 옵션이라면 색상을 변경
        if (findPos != std::string::npos && i != 0) { 
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // 콘솔 출력 색상 변경후 출력
            std::cout << outputString;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 출력 후  색상을 원상복구
        }
        // 현재 출력하고자 하는 옵션이 선택된 옵션이 아니라면 그대로 출력
        else std::cout << padToWidth(menu_line_str, MENU_PANEL_WIDTH);

        std::cout << BORDER_CHAR;

        outputString = padToWidth(content_line_str, CONTENT_PANEL_WIDTH);
        findPos = outputString.find('>'); // '>'의 위치 찾기
        if (findPos != std::string::npos) {
            std::cout << outputString.substr(0, findPos);         // '>'전까지 출력

            // 이 부분의 if-else는 '<'의 유무에 있습니다
            // 캘린더 보기에서는 선택된 날짜가 '> <' 커서 안에 들어가 있지만
            // 나머지 옵션들은 '>' 만 존재하고 '<' 는 존재하지 않기 때문입니다
            size_t endPos = outputString.find('<');
            if (endPos != std::string::npos) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout << outputString.substr(findPos, 1+endPos- findPos);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                std::cout << outputString.substr(endPos + 1);        // < 이후부터 끝까지
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout << outputString.substr(findPos);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            }
        }
        else std::cout << outputString;

        std::cout << BORDER_CHAR << std::endl;

        /*
        // 기존에 사용하던 출력방식 입니다. 혹시 몰라 남겨둡니다
        std::cout << BORDER_CHAR
            << padToWidth(menu_line_str, MENU_PANEL_WIDTH)
            << BORDER_CHAR
            << padToWidth(content_line_str, CONTENT_PANEL_WIDTH)
            << BORDER_CHAR << std::endl;
         */
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
