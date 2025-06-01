#include <iostream>
#include <windows.h> // SetConsoleOutputCP 사용
#include "PokeTodoApp.h"

int main() {
    SetConsoleOutputCP(65001); // UTF-8 출력 설정

    try {
        PokeTodoApp app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << u8"오류가 발생했습니다: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}