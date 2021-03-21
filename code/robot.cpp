#include "robot.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "errors.h"

#pragma region interface
void IRobot::Init(
    const uint32_t x,
    const uint32_t y,
    uint64_t *const step,
    Maze *const mz)
{
    throw new NotImplementedError();
}
void IRobot::Walk()
{
    throw new NotImplementedError();
}
#pragma endregion
#pragma region coding_game
const std::pair<int, int> DIR_VEC[4] = {
    std::make_pair(0, -1),  // UP
    std::make_pair(1, 0),   // RIGHT
    std::make_pair(0, 1),   // DOWN
    std::make_pair(-1, 0)   // LEFT
};

CDGRobot::~CDGRobot()
{
    if (this->_pos != nullptr) {
        delete[] this->_pos;
        this->_pos = nullptr;
    }
}

void CDGRobot::Init(
    const uint32_t x,
    const uint32_t y,
    uint64_t *const step,
    Maze *const mz)
{
    this->_pos = new std::pair<uint32_t, uint32_t>(x, y);
    this->_step = step;
    this->_maze = mz;
}
void CDGRobot::Walk()
{
    int64_t nx = this->_pos->first, ny = this->_pos->second;
    nx += DIR_VEC[this->_dir].first;
    ny += DIR_VEC[this->_dir].second;

    if (*this->_step == 0)
        goto end;
    if (nx < 0 || ny < 0)
        goto turn;
    if (nx > INT32_MAX || ny > INT32_MAX)
        throw new std::overflow_error("overflow in nx or ny");

    if (this->_maze->Walkable(nx, ny))
        goto wk_straight;
    else
        goto turn;

wk_straight:
    this->_pos->first = nx;
    this->_pos->second = ny;
    --(*this->_step);
    goto end;
turn:
    this->_dir = (this->_dir + 1) % 4;
    this->Walk();
    goto end;
end:
#ifdef DEBUG
    printf("%c[2J", 27);
    this->_maze->PrintAtLoc(
        this->_pos->first, this->_pos->second);
    sleep(1);
#endif
    return;
}
const std::pair<uint32_t, uint32_t> &CDGRobot::GetPos()
{
    return *this->_pos;
}
const int CDGRobot::GetDir()
{
    return this->_dir;
}
#pragma endregion
#pragma dp_robot
void CDGDPRobot::Init(
    const uint32_t x,
    const uint32_t y,
    uint64_t *const step,
    Maze *const mz)
{
    this->_robot = new CDGRobot();
    this->_robot->Init(x, y, step, mz);
    this->_step = step;

    auto msz = mz->GetSize();
    this->_mx = msz.first;
    this->_my = msz.second;

    this->_dpmap = new int32_t **[this->_my];
    for (auto i = 0; i != this->_my; ++i) {
        this->_dpmap[i] = new int32_t *[this->_mx];
        for (auto j = 0; j != msz.first; ++j) {
            this->_dpmap[i][j] = new int32_t[4];
            for (auto l = 0; l != 4; ++l)
                this->_dpmap[i][j][l] = -1;
        }
    }
    auto dir = this->_robot->GetDir();
    this->_dpmap[y][x][dir] = 0;
}
void CDGDPRobot::Walk()
{
    this->_robot->Walk();
    if (*this->_step == 0)
        return;

    auto curpos = this->_robot->GetPos();
    auto curdir = this->_robot->GetDir();

    for (auto i = 0; i != this->_my; ++i)
        for (auto j = 0; j != this->_mx; ++j)
            for (auto k = 0; k != 4; ++k)
                if (this->_dpmap[i][j][k] != -1)
                    this->_dpmap[i][j][k]++;

    if (this->_dpmap[curpos.second][curpos.first][curdir] != -1) {
#ifdef DEBUG
        std::cerr
            << "[step]"
            << this->_dpmap[curpos.second][curpos.first][curdir]
            << '\n';
#endif

        *this->_step %=
            this->_dpmap[curpos.second][curpos.first][curdir];
    } else
        this->_dpmap[curpos.second][curpos.first][curdir] = 0;
}
const std::pair<uint32_t, uint32_t> &CDGDPRobot::GetPos()
{
    return this->_robot->GetPos();
}

CDGDPRobot::~CDGDPRobot()
{
    if (this->_robot != nullptr) {
        delete this->_robot;
        this->_robot = nullptr;
    }
    if (this->_dpmap != nullptr) {
        for (auto i = 0; i != this->_my; ++i) {
            for (auto j = 0; j != this->_mx; ++j) {
                delete[] this->_dpmap[i][j];
            }
            delete[] this->_dpmap[i];
        }
        delete[] this->_dpmap;

        this->_dpmap = nullptr;
    }
}
#pragma endregion