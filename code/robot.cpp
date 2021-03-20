#include "robot.h"
#include "errors.h"
#include <unistd.h>
#include <stdlib.h>

#include <stdexcept>
#include <iostream>

#pragma region interface
void IRobot::Init(const uint32_t x, const uint32_t y, const uint32_t step, Maze* const mz)
{
    throw new NotImplementedError();
}
void IRobot::Walk()
{
    throw new NotImplementedError();
}
#pragma endregion
#pragma region coding_game
const std::pair<int, int> DIR_VEC[4]=
{
    std::make_pair( 0,-1), //UP
    std::make_pair( 1, 0), //RIGHT
    std::make_pair( 0, 1), //DOWN
    std::make_pair(-1, 0)  //LEFT
};

CDGRobot::~CDGRobot()
{
    if (this->_pos != nullptr)
    {
        delete [] this->_pos;
        this->_pos = nullptr;
    }
}

void CDGRobot::Init(const uint32_t x, const uint32_t y, const uint32_t step, Maze* const mz)
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
    

    if (this->_step == 0)
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
        this->_step--;
        goto end;
    turn:
        this->_dir = (this->_dir + 1) % 4;
        this->Walk();
        goto end;
    end:
        #ifdef DEBUG
        system("clear");
        this->_maze->PrintAtLoc(this->_pos->first, this->_pos->second);
        sleep(1);
        #endif
        return;
}
const std::pair<uint32_t, uint32_t>& CDGRobot::GetPos()
{
    return *this->_pos;
}
#pragma endregion