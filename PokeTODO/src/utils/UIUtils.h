#pragma once
#include <string>
#include <vector>

// 전방 선언
enum class TaskStatus;
enum class Priority;

// 시각적 너비 계산 함수
int get_visual_width(const std::string& s);

// 문자열을 특정 시각적 너비로 채우는 함수
std::string padToWidth(const std::string& input, int target_visual_width, char padChar = ' ');

// TaskStatus와 Priority를 문자열로 변환하는 함수
std::string statusToString(TaskStatus status);
std::string priorityToString(Priority priority);

// ASCII 아트 데이터
extern std::vector<std::string> ASCII_ART; 