//main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <conio.h> // _getch() 사용
#include <windows.h> // system("cls"), SetConsoleOutputCP 사용
#include <algorithm> // std::max
#include <limits>    // std::numeric_limits

#include "services/UserPlanService.h" // UserPlanService.h 경로
#include "models/TaskCreateRequest.h" // TaskCreateRequest.h 경로
#include "models/Task.h"              // Task.h 경로 (TaskStatus, Priority enum 사용)

// 다마고치 테스트를 위한 헤더 추가
#include "services/UserTamagotchiService.h"
#include "models/Tamagotchi.h"




// TaskStatus 및 Priority를 문자열로 변환하는 도우미 함수
std::string statusToString(TaskStatus status) {
    switch (status) {
    case TaskStatus::PENDING: return u8"대기 중";
    case TaskStatus::IN_PROGRESS: return u8"진행 중";
    case TaskStatus::COMPLETED: return u8"완료됨";
    default: return u8"알 수 없음";
    }
}

std::string priorityToString(Priority priority) {
    switch (priority) {
    case Priority::LOW: return u8"낮음";
    case Priority::MEDIUM: return u8"중간";
    case Priority::HIGH: return u8"높음";
    default: return u8"알 수 없음";
    }
}

// Helper to calculate visual width (assuming ASCII=1, others=2)
int get_visual_width(const std::string& s) {
    int visual_width = 0;
    for (size_t i = 0; i < s.length(); ) {
        unsigned char c = s[i];
        if (c < 0x80) { // ASCII
            visual_width += 1;
            i += 1;
        }
        else { // Non-ASCII, assume full-width (2 visual cells)
            visual_width += 2;
            // Advance by the number of bytes in the UTF-8 character
            if ((c & 0xF8) == 0xF0) { // 4-byte character (starts with 11110xxx)
                i += 4;
            }
            else if ((c & 0xF0) == 0xE0) { // 3-byte character (starts with 1110xxxx)
                i += 3;
            }
            else if ((c & 0xE0) == 0xC0) { // 2-byte character (starts with 110xxxxx)
                i += 2;
            }
            else {
                // Invalid UTF-8 sequence or unexpected byte
                i += 1; // Skip one byte to prevent infinite loops
            }
        }
        if (i > s.length()) { // Ensure i does not go out of bounds
            i = s.length();
        }
    }
    return visual_width;
}

// 문자열을 특정 시각적 너비로 채우는 도우미 함수
std::string padToWidth(const std::string& input, int target_visual_width, char padChar = ' ') {
    std::string result = input;
    int current_visual_width = get_visual_width(input);

    if (current_visual_width < target_visual_width) {
        result.append(target_visual_width - current_visual_width, padChar);
    }
    else if (current_visual_width > target_visual_width) {
        // 현재는 문자열이 길 경우 자르지 않고 원본을 반환합니다.
        // 필요시, 시각적 너비에 맞춰 자르는 로직 추가 가능.
        // 예: result = truncateToVisualWidth(input, target_visual_width);
    }
    return result;
}

// 새로운 UI 상태를 정의합니다.
enum class UiState {
    SHOWING_MAIN_MENU,       // 기본 메뉴 표시 상태 (오른쪽 패널은 기본 정보)
    VIEWING_PLANS,           // 플랜 목록을 오른쪽 패널에 표시
    PROMPT_PLAN_ID_FOR_TITLE, // 플랜 제목 변경을 위해 플랜 ID 입력 대기
    PROMPT_NEW_PLAN_TITLE,   // 새 플랜 제목 입력 대기
    PROMPT_PLAN_ID_FOR_TASKS, // 작업 목록 보기를 위해 플랜 ID 입력 대기
    VIEWING_TASKS,           // 특정 플랜의 작업 목록을 오른쪽 패널에 표시
    PROMPT_PLAN_ID_FOR_ADD_TASK, // 작업 추가를 위해 플랜 ID 입력 대기
    ADDING_TASK_DETAILS_TITLE,
    ADDING_TASK_DETAILS_DESC,
    ADDING_TASK_DETAILS_PRIORITY,
    ADDING_TASK_DETAILS_DEADLINE,
    ADDING_TASK_DETAILS_TAGS,
    PROMPT_PLAN_ID_FOR_REMOVE_TASK,
    PROMPT_TASK_ID_FOR_REMOVE,
    PROMPT_PLAN_ID_FOR_COMPLETE_TASK,
    PROMPT_TASK_ID_FOR_COMPLETE,
    PROMPT_ENTER_TAMAGOTCHI_MODE
};

// 오른쪽 패널에 표시될 기본 내용을 반환하는 함수
std::vector<std::string> getDefaultRightPanelContent() {
    return {
        u8"   [PokeTODO에 오신 것을 환영합니다!]",
        u8"",
        u8"   무엇을 도와드릴까요?",
        u8"   왼쪽 메뉴에서 작업을 선택하세요.",
        u8"",
    };
}

std::vector<std::string> getPlanListStringsForPanel(UserPlanService& planService, const std::string& instruction = u8"엔터 키를 누르면 메뉴로 돌아갑니다.") {
    std::vector<std::string> content;
    content.push_back(u8"--- 플랜 목록 ---");
    const auto& plans = planService.getPlans();
    if (plans.empty()) {
        content.push_back(u8"  표시할 플랜이 없습니다.");
    }
    else {
        for (const auto& pair : plans) {
            const std::vector<Plan>& planlist = pair.second;
            for (const auto& plan : planlist) {
                content.push_back(u8"  [" + plan.getPlanId() + u8"] 제목: " + plan.getTitle());
            }
        }
    }
    content.push_back(u8"");
    if (!instruction.empty()) {
        content.push_back(instruction);
    }
    return content;
}

std::vector<std::string> getTaskListStringsForPanel(Plan* plan, const std::string& instruction = u8"엔터 키를 누르면 메뉴로 돌아갑니다.") {
    std::vector<std::string> content;
    if (!plan) {
        content.push_back(u8"  플랜이 유효하지 않습니다.");
        if (!instruction.empty()) content.push_back(instruction);
        return content;
    }
    content.push_back(u8"--- '" + plan->getTitle() + u8"'의 작업 목록 ---");
    const std::vector<Task>& tasks = plan->getTasks();
    if (tasks.empty()) {
        content.push_back(u8"  표시할 작업이 없습니다.");
    }
    else {
        for (const auto& task : tasks) {
            content.push_back(u8"  [" + task.getTaskId() + u8"] " + task.getTitle() +
                u8" (우선순위: " + priorityToString(task.getPriority()) +
                u8", 상태: " + statusToString(task.getStatus()) + u8")");
        }
    }
    content.push_back(u8"");
    if (!instruction.empty()) {
        content.push_back(instruction);
    }
    return content;
}

const std::vector<std::string> ASCII_ART = {
    u8"⠀⠀⠀⠀⠀⠀⠀⡀⠠⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    u8"⠀⠀⠀⠀⠀⡔⠁⠀⠀⠀⠈⠑⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    u8"⠀⠀⠀⠀⣜⠃⠀⠀⠀⢘⢳⢆⠘⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    u8"⠀⠀⢀⠔⠉⠀⠀⠀⠀⣜⠀⢸⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
    u8"⠀⠀⢸⠀⡀⠀⠀⠀⠀⠈⠉⠁⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠀⠀",
    u8"⠀⡤⢄⣳⣦⠤⠤⠤⠄⣄⡲⡪⠀⡇⠀⢀⡀⢤⠀⠀⠀⢠⠒⢋⠤⠀",
    u8"⠘⢝⠁⠈⠙⠷⠒⠒⠾⠓⢎⠀⠀⠁⠉⠁⠈⢛⠆⠀⠀⠈⢷⣿⠀⣆",
    u8"⠀⠀⠑⢄⠀⡘⠀⠀⠀⠀⠀⠣⡀⠀⠀⣀⠔⠁⠀⠀⠀⢀⠃⠹⢷⡄",
    u8"⠀⠀⠀⠀⠑⡇⠀⠀⠀⠀⠀⠀⢡⠀⠈⡄⠀⠀⠀⠀⠀⠈⠣⢤⡼⠀",
    u8"⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⡆⠀⠰⠀⠀⠀⠀⠀⠀⠀⡌⡇⠀",
    u8"⠀⠀⠀⠀⠀⢠⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⡇⠀⠀⠀⠀⢀⠌⢠⠃⠀",
    u8"⠀⠀⠀⠀⡐⠉⠣⡀⠀⠀⠀⠀⢀⠃⠂⠐⡎⠁⠒⠂⠈⠀⣠⠏⠀⠀",
    u8"⠀⠀⠀⠀⡀⠀⠀⠈⠒⡤⠀⠠⠊⠀⠀⠀⡠⣀⣀⠠⢄⠾⠃⠀⠀⠀",
    u8"⠀⠀⣀⡤⠚⠲⠀⠀⠸⡁⠀⢘⠄⠀⠀⣠⠋⠁⠀⠉⠁⠀⠀⠀⠀⠀",
    u8"⠀⠈⠛⡊⠂⠀⠀⠒⠂⠁⠀⠘⢖⣔⣶⡲⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀"
};

void displayMenu(const std::vector<std::string>& menuItems, int selectedItem, const std::vector<std::string>& rightPanelContent, const std::string& inputPrompt = "") {
    system("cls");

    const std::string BORDER_CHAR = u8"■";
    const int TOTAL_WIDTH = 70;
    const int MENU_PANEL_WIDTH = 28;
    const int CONTENT_PANEL_WIDTH = TOTAL_WIDTH - MENU_PANEL_WIDTH - 3;
    const int INNER_ASCII_WIDTH = TOTAL_WIDTH - 2;

    for (int i = 0; i < TOTAL_WIDTH; ++i) std::cout << BORDER_CHAR;
    std::cout << std::endl;

    std::cout << BORDER_CHAR << padToWidth("", INNER_ASCII_WIDTH) << BORDER_CHAR << std::endl;

    for (const auto& artLine : ASCII_ART) {
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

void clearCin() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void clearKeyboardBuffer() {
    while (_kbhit()) {
        _getch();
    }
}

// 임시 작업 데이터 저장을 위한 구조체
struct TempTaskData {
    std::string planId;
    std::string title;
    std::string description;
    Priority priority = Priority::MEDIUM;
    long long deadlineHours = 1;
    std::vector<std::string> tags;
};


int main() {
    SetConsoleOutputCP(65001); // UTF-8 출력 설정

#if 1 // 기존 UI 로직 비활성화
    UserPlanService planService;

    UserTamagotchiService tamagotchiService;
    Tamagotchi::Tamagotchi* myPet = new Tamagotchi::Tamagotchi(1, u8"피카츄");
    tamagotchiService.assignTamagotchi(myPet);

    Plan plan1 = planService.createPlan(std::time(0), u8"오늘의 할 일");
    Plan plan2 = planService.createPlan(std::time(0) + 10, u8"내일의 준비물");

    if (plan1.getPlanId() != "error-plan") {
        planService.addTaskToPlan(plan1.getPlanId(), TaskCreateRequest(u8"C++ UI 개선", u8"아스키 아트와 메뉴 시스템 통합", Priority::HIGH, std::time(0) + 3600, { u8"개발", u8"UI" }));
        planService.addTaskToPlan(plan1.getPlanId(), TaskCreateRequest(u8"저녁 식사 준비", u8"된장찌개 만들기", Priority::MEDIUM, std::time(0) + 7200, { u8"요리" }));
    }
    if (plan2.getPlanId() != "error-plan") {
        planService.addTaskToPlan(plan2.getPlanId(), TaskCreateRequest(u8"발표 자료 준비", u8"최종 검토 및 수정", Priority::HIGH, std::time(0) + 86400, { u8"업무" }));
    }

    std::vector<std::string> menuItems = {
        u8"플랜 보기",
        u8"플랜 추가하기",
        u8"플랜 제목 변경",
        u8"작업 목록 보기",
        u8"작업 추가하기",
        u8"작업 제거하기",
        u8"작업 완료하기",
        u8"다마고치 모드",
        u8"종료"
    };
    int selectedItem = 0;
    bool running = true;

    UiState currentAppState = UiState::SHOWING_MAIN_MENU;
    std::vector<std::string> rightPanelDynamicContent = getDefaultRightPanelContent();
    std::string currentPlanIdContext;
    std::string currentTaskIdContext;
    TempTaskData tempTaskData; // 작업 추가 시 임시 데이터 저장
    std::string inputPrompt = "";


    while (running) {
        displayMenu(menuItems, selectedItem, rightPanelDynamicContent, inputPrompt);
        inputPrompt = ""; // 사용 후 초기화

        // 상태에 따른 입력 처리 (std::cin, std::getline 사용 부분)
        // _getch()는 비차단형 입력이 아니므로, 특정 상태에서 입력을 기다려야 함
        if (currentAppState == UiState::PROMPT_PLAN_ID_FOR_TASKS ||
            currentAppState == UiState::PROMPT_PLAN_ID_FOR_TITLE ||
            currentAppState == UiState::PROMPT_NEW_PLAN_TITLE ||
            currentAppState == UiState::PROMPT_PLAN_ID_FOR_ADD_TASK ||
            currentAppState == UiState::ADDING_TASK_DETAILS_TITLE ||
            currentAppState == UiState::ADDING_TASK_DETAILS_DESC ||
            currentAppState == UiState::ADDING_TASK_DETAILS_PRIORITY ||
            currentAppState == UiState::ADDING_TASK_DETAILS_DEADLINE ||
            currentAppState == UiState::ADDING_TASK_DETAILS_TAGS ||
            currentAppState == UiState::PROMPT_PLAN_ID_FOR_REMOVE_TASK ||
            currentAppState == UiState::PROMPT_TASK_ID_FOR_REMOVE ||
            currentAppState == UiState::PROMPT_PLAN_ID_FOR_COMPLETE_TASK ||
            currentAppState == UiState::PROMPT_ENTER_TAMAGOTCHI_MODE ||
            currentAppState == UiState::PROMPT_TASK_ID_FOR_COMPLETE
            ) {
            // 이러한 상태들은 displayMenu 이후 바로 입력을 받아야 함.
            // _getch()를 여기서 또 호출하면 안됨.
            // 해당 상태의 Enter 처리 부분에서 입력을 받도록 로직 구성.
        }

        clearKeyboardBuffer();
        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (currentAppState == UiState::SHOWING_MAIN_MENU) {
                switch (key) {
                case 72:
                    selectedItem = (selectedItem - 1 + static_cast<int>(menuItems.size())) % static_cast<int>(menuItems.size());
                    break;
                case 80:
                    selectedItem = (selectedItem + 1) % static_cast<int>(menuItems.size());
                    break;
                }
            }
        }
        else if (key == 13) { // Enter 키

            std::string userInput; // getline 등을 위한 임시 변수

            switch (currentAppState) {
            case UiState::SHOWING_MAIN_MENU:
                if (menuItems[selectedItem] == u8"플랜 보기") {
                    currentAppState = UiState::VIEWING_PLANS;
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService);
                }
                else if (menuItems[selectedItem] == u8"플랜 추가하기") {
                    rightPanelDynamicContent = { u8"--- 새 플랜 추가 ---" };
                    inputPrompt = u8"플랜 제목을 입력하세요: ";
                    currentAppState = UiState::PROMPT_NEW_PLAN_TITLE; // 새 제목 입력 상태로 변경
                }
                else if (menuItems[selectedItem] == u8"플랜 제목 변경") {
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService, ""); // 안내 문구 없이 목록만
                    inputPrompt = u8"제목을 변경할 플랜 ID: ";
                    currentAppState = UiState::PROMPT_PLAN_ID_FOR_TITLE;
                }
                else if (menuItems[selectedItem] == u8"작업 목록 보기") {
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService, "");
                    inputPrompt = u8"작업을 볼 플랜 ID: ";
                    currentAppState = UiState::PROMPT_PLAN_ID_FOR_TASKS;
                }
                else if (menuItems[selectedItem] == u8"작업 추가하기") {
                    tempTaskData = {}; // 임시 데이터 초기화
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService, "");
                    inputPrompt = u8"작업을 추가할 플랜 ID: ";
                    currentAppState = UiState::PROMPT_PLAN_ID_FOR_ADD_TASK;
                }
                else if (menuItems[selectedItem] == u8"작업 제거하기") {
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService, "");
                    inputPrompt = u8"작업을 제거할 플랜 ID: ";
                    currentAppState = UiState::PROMPT_PLAN_ID_FOR_REMOVE_TASK;
                }
                else if (menuItems[selectedItem] == u8"작업 완료하기") {
                    rightPanelDynamicContent = getPlanListStringsForPanel(planService, "");
                    inputPrompt = u8"작업을 완료할 플랜 ID: ";
                    currentAppState = UiState::PROMPT_PLAN_ID_FOR_COMPLETE_TASK;
                }
                else if (menuItems[selectedItem] == u8"다마고치 모드") {
                    rightPanelDynamicContent = {
                        u8"다마고치에게 먹이를 주거나 놀아주세요",
                    };
                    currentAppState = UiState::PROMPT_ENTER_TAMAGOTCHI_MODE;
                }
                else if (menuItems[selectedItem] == u8"종료") {
                    running = false;
                    system("cls");
                    std::cout << u8"PokeTODO를 종료합니다. 다음에 또 만나요!" << std::endl;
                }
                break;

            case UiState::VIEWING_PLANS:
            case UiState::VIEWING_TASKS:
                currentAppState = UiState::SHOWING_MAIN_MENU;
                rightPanelDynamicContent = getDefaultRightPanelContent();
                break;

            case UiState::PROMPT_NEW_PLAN_TITLE:
                std::getline(std::cin, userInput);
                if (!userInput.empty()) {
                    planService.createPlan(std::time(0), userInput);
                    rightPanelDynamicContent = { u8"플랜 '" + userInput + u8"'이(가) 생성되었습니다." };
                }
                else {
                    rightPanelDynamicContent = { u8"플랜 생성이 취소되었습니다." };
                }
                rightPanelDynamicContent.push_back(u8"엔터를 눌러 메뉴로 돌아가세요.");
                currentAppState = UiState::VIEWING_PLANS; // 임시로 내용 표시 후 엔터 대기 상태로
                break;

            case UiState::PROMPT_PLAN_ID_FOR_TITLE:
                std::cin >> userInput; clearCin();
                currentPlanIdContext = userInput;
                if (planService.getPlan(currentPlanIdContext)) {
                    rightPanelDynamicContent = { u8"선택된 플랜: [" + currentPlanIdContext + u8"] " + planService.getPlan(currentPlanIdContext)->getTitle() };
                    inputPrompt = u8"새 플랜 제목: ";
                    currentAppState = UiState::PROMPT_NEW_PLAN_TITLE; // 실제로는 새 제목 입력 상태로. PROMPT_NEW_PLAN_TITLE 로직 재활용? 아니면 분리?
                    // 일단은 플랜 생성/제목변경을 같은 상태로 처리. 더 명확히 하려면 상태 분리.
                    // 이 경우, PROMPT_NEW_PLAN_TITLE이 아니라 PROMPT_RENAME_PLAN_TITLE 같은 새 상태 필요.
                    // 여기서는 간결하게 하기 위해 새 제목을 입력받는 상태로 바로 넘어가지 않고,
                    // 다시 메뉴에서 "플랜 추가"를 선택해야 제목 변경이 되는 것처럼 동작.
                    // --> 수정: 새 상태를 만들고, 해당 상태에서 새 제목을 입력받도록 해야 함.
                    // --> 지금은 임시로, ID 입력 받으면 바로 메뉴로.
                    rightPanelDynamicContent.push_back(u8"기능 구현 중... ID: " + currentPlanIdContext);
                    rightPanelDynamicContent.push_back(u8"엔터를 누르면 메뉴로 돌아갑니다.");
                    currentAppState = UiState::VIEWING_PLANS; // 임시
                }
                else {
                    rightPanelDynamicContent = { u8"잘못된 플랜 ID: " + userInput };
                    rightPanelDynamicContent.push_back(u8"엔터를 눌러 메뉴로 돌아가세요.");
                    currentAppState = UiState::VIEWING_PLANS; // 임시
                }
                break;


            case UiState::PROMPT_PLAN_ID_FOR_TASKS:
                std::cin >> userInput; clearCin();
                {
                    Plan* selectedPlan = planService.getPlan(userInput);
                    if (selectedPlan) {
                        currentPlanIdContext = userInput;
                        rightPanelDynamicContent = getTaskListStringsForPanel(selectedPlan);
                        currentAppState = UiState::VIEWING_TASKS;
                    }
                    else {
                        rightPanelDynamicContent = { u8"잘못된 플랜 ID 입니다: " + userInput };
                        rightPanelDynamicContent.push_back(u8"엔터를 누르면 메뉴로 돌아갑니다.");
                        currentAppState = UiState::VIEWING_PLANS;
                    }
                }
                break;

            case UiState::PROMPT_PLAN_ID_FOR_ADD_TASK:
                std::cin >> userInput; clearCin();
                tempTaskData.planId = userInput;
                if (planService.getPlan(tempTaskData.planId)) {
                    rightPanelDynamicContent = { u8"플랜 [" + tempTaskData.planId + u8"]에 작업 추가 중..." };
                    inputPrompt = u8"작업 제목: ";
                    currentAppState = UiState::ADDING_TASK_DETAILS_TITLE;
                }
                else {
                    rightPanelDynamicContent = { u8"잘못된 플랜 ID: " + userInput };
                    rightPanelDynamicContent.push_back(u8"엔터를 누르면 메뉴로 돌아가세요.");
                    currentAppState = UiState::VIEWING_PLANS; // 임시
                }
                break;

            case UiState::ADDING_TASK_DETAILS_TITLE:
                std::getline(std::cin, userInput);
                if (userInput.empty()) {
                    rightPanelDynamicContent = { u8"작업 제목은 필수입니다. 추가 취소." };
                    rightPanelDynamicContent.push_back(u8"엔터를 누르면 메뉴로 돌아가세요.");
                    currentAppState = UiState::VIEWING_PLANS; // 임시
                    break;
                }
                tempTaskData.title = userInput;
                rightPanelDynamicContent.push_back(u8"제목: " + tempTaskData.title);
                inputPrompt = u8"작업 설명: ";
                currentAppState = UiState::ADDING_TASK_DETAILS_DESC;
                break;

            case UiState::ADDING_TASK_DETAILS_DESC:
                std::getline(std::cin, userInput);
                tempTaskData.description = userInput;
                rightPanelDynamicContent.push_back(u8"설명: " + tempTaskData.description);
                inputPrompt = u8"우선순위 (1:높음,2:중간,3:낮음): ";
                currentAppState = UiState::ADDING_TASK_DETAILS_PRIORITY;
                break;

                // ... (ADDING_TASK_DETAILS_PRIORITY, DEADLINE, TAGS 처리) ...
                // 이 부분은 코드가 길어져 생략. 이전 버전의 작업 추가 로직을 참고하여 유사하게 구현.
                // 모든 정보 입력 후 TaskCreateRequest 만들고 addTaskToPlan 호출.

            case UiState::PROMPT_ENTER_TAMAGOTCHI_MODE:
                while (1) {
                    displayMenu(menuItems, selectedItem, rightPanelDynamicContent, inputPrompt);
                    //clearKeyboardBuffer();
                    int TagamotchiInput = _getch();
                    if (TagamotchiInput == 'F' || TagamotchiInput == 'f') {
                        tamagotchiService.interactWithTamagotchi(TamagotchiAction::FEED);
                    }
                    else if (TagamotchiInput == 'P' || TagamotchiInput == 'p') {
                        tamagotchiService.interactWithTamagotchi(TamagotchiAction::PLAY);
                    }
                    else if (TagamotchiInput == 27) {
                        currentAppState = UiState::SHOWING_MAIN_MENU;
                        rightPanelDynamicContent = getDefaultRightPanelContent();
                        break;
                    }

                    Tamagotchi::Tamagotchi* pet = tamagotchiService.getTamagotchi();
                    rightPanelDynamicContent = {
                        u8"===== 다마고치 상태 =====",
                        u8"이름: " + pet->getName(),
                        u8"상태: " + pet->getCurrentStateName(),
                        u8"행복도: " + std::to_string(pet->getHappiness()),
                        u8"배고픔: " + std::to_string(pet->getHunger()),
                        u8"",
                        u8"[F] 먹이 주기",
                        u8"[P] 놀아주기",
                        u8"[ESC] 나가기"
                    };
                }
                break;

            default:
                currentAppState = UiState::SHOWING_MAIN_MENU;
                rightPanelDynamicContent = getDefaultRightPanelContent();
                break;
            }

        }
        else if (key == 27) {
            if (currentAppState != UiState::SHOWING_MAIN_MENU) {
                currentAppState = UiState::SHOWING_MAIN_MENU;
                rightPanelDynamicContent = getDefaultRightPanelContent();
                inputPrompt = "";
            }
            else {
                running = false;
                system("cls");
                std::cout << u8"PokeTODO를 종료합니다." << std::endl;
            }
        }
    }
#else
    // 다마고치 기능 테스트 코드
    std::cout << u8"===== 다마고치 테스트 시작 =====" << std::endl;

    UserTamagotchiService tamagotchiService;

    // 새 다마고치 생성 및 할당
    Tamagotchi::Tamagotchi* myPet = new Tamagotchi::Tamagotchi(1, u8"피카츄");
    tamagotchiService.assignTamagotchi(myPet);

    Tamagotchi::Tamagotchi* currentPet = tamagotchiService.getTamagotchi();
    if (currentPet) {
        std::cout << u8"\n다마고치 이름: " << currentPet->getName() << std::endl;
        std::cout << u8"초기 상태: " << currentPet->getCurrentStateName() << std::endl;
        std::cout << u8"행복도: " << currentPet->getHappiness() << u8", 배고픔: " << currentPet->getHunger() << std::endl;

        // 먹이주기 테스트
        std::cout << u8"\n먹이를 줍니다..." << std::endl;
        tamagotchiService.interactWithTamagotchi(TamagotchiAction::FEED);
        std::cout << u8"상태 변경 후: " << currentPet->getCurrentStateName() << std::endl;
        std::cout << u8"행복도: " << currentPet->getHappiness() << u8", 배고픔: " << currentPet->getHunger() << std::endl;

        // 놀아주기 테스트
        std::cout << u8"\n놀아줍니다..." << std::endl;
        tamagotchiService.interactWithTamagotchi(TamagotchiAction::PLAY);
        std::cout << u8"상태 변경 후: " << currentPet->getCurrentStateName() << std::endl;
        std::cout << u8"행복도: " << currentPet->getHappiness() << u8", 배고픔: " << currentPet->getHunger() << std::endl;

        // 여러 번 상호작용하여 상태 변화 관찰 (예: 배고픔 상태 만들기)
        std::cout << u8"\n계속 놀아줘서 배고프게 만들기..." << std::endl;
        for (int i = 0; i < 15; ++i) { // 여러 번 놀아주면 배고픔 수치가 올라갈 것으로 예상
            tamagotchiService.interactWithTamagotchi(TamagotchiAction::PLAY);

            std::cout << u8"놀이 " << (i + 1) << u8"회 후: " << currentPet->getCurrentStateName()
                << u8", 행복도: " << currentPet->getHappiness()
                << u8", 배고픔: " << currentPet->getHunger() << std::endl;
        }
        std::cout << u8"최종 상태: " << currentPet->getCurrentStateName() << std::endl;
        std::cout << u8"행복도: " << currentPet->getHappiness() << u8", 배고픔: " << currentPet->getHunger() << std::endl;

    }
    else {
        std::cout << u8"다마고치가 할당되지 않았습니다." << std::endl;
    }

    std::cout << u8"\n===== 다마고치 테스트 종료 =====" << std::endl;

    // UserTamagotchiService의 소멸자가 할당된 다마고치 메모리를 해제합니다.
#endif // 0

    return 0;
}
