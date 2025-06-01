#include "UIUtils.h"
#include "../models/Task.h" // TaskStatus, Priority enum 사용

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
std::string padToWidth(const std::string& input, int target_visual_width, char padChar) {
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

std::vector<std::string> ASCII_ART = {
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