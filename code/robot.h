#pragma once
#include <stdint.h>
#include <utility>
#include "maze.h"

#pragma region interface
class IRobot
{
public:
    virtual void Init(
        const uint32_t x,
        const uint32_t y,
        uint64_t *const step,
        Maze *const mz);
    virtual void Walk();
};
#pragma endregion
#pragma region coding_game
class CDGRobot final : public IRobot
{
private:
    enum DIR { UP = 0, RIGHT, DOWN, LEFT };

    uint64_t *_step = nullptr;
    std::pair<uint32_t, uint32_t> *_pos = nullptr;
    uint8_t _dir = DIR::UP;
    Maze *_maze = nullptr;

public:
    ~CDGRobot();
    void Init(
        const uint32_t x,
        const uint32_t y,
        uint64_t *const step,
        Maze *const mz) override;
    void Walk() override;
    const std::pair<uint32_t, uint32_t> &GetPos();
    const int GetDir();
};
class CDGDPRobot final : public IRobot
{
private:
    enum DIR { UP = 0, RIGHT, DOWN, LEFT };
    int32_t ***_dpmap = nullptr;
    uint32_t _mx = 0, _my = 0;
    CDGRobot *_robot = nullptr;
    uint64_t *_step = nullptr;

public:
    void Init(
        const uint32_t x,
        const uint32_t y,
        uint64_t *const step,
        Maze *const mz) override;
    void Walk() override;
    const std::pair<uint32_t, uint32_t> &GetPos();
    ~CDGDPRobot();
};
#pragma endregion