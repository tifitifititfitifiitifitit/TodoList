#pragma once
#include <string>
#include <vector>

class UIManager {
public:
    // 메뉴를 표시하는 함수
    static void displayMenu(
        const std::vector<std::string>& menuItems, 
        int selectedItem, 
        const std::vector<std::string>& rightPanelContent, 
        const std::string& inputPrompt = "", 
        std::vector<std::string> art = std::vector<std::string>());

    // 입력 버퍼를 클리어하는 함수들
    static void clearCin();
    static void clearKeyboardBuffer();
}; 