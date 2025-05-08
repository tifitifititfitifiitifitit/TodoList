#pragma once

class UserPointService {
private:
    int points;

public:
    UserPointService();

    // Point management
    void addPoints(int amount);
    int getPoints() const;
}; 