#pragma once
#include "maze.h"
#include <stdint.h>
#include <utility>

#pragma region interface
class IRobot {
public:
    virtual void Init(const uint32_t x, const uint32_t y, const uint32_t step, Maze* const mz);
    virtual void Walk();
};
#pragma endregion
#pragma region coding_game
class CDGRobot final : public IRobot {
private:
    enum DIR { UP = 0,
        RIGHT,
        DOWN,
        LEFT };

    uint32_t                       _step;
    std::pair<uint32_t, uint32_t>* _pos  = nullptr;
    uint8_t                        _dir  = DIR::UP;
    Maze*                          _maze = nullptr;

public:
    ~CDGRobot();
    void                                 Init(const uint32_t x, const uint32_t y, const uint32_t step, Maze* const mz) override;
    void                                 Walk() override;
    const std::pair<uint32_t, uint32_t>& GetPos();
};
#pragma endregion