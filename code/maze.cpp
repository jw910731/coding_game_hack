#include "maze.h"
#include <memory.h>
#include <iostream>

Maze *Maze::_instatnce = nullptr;

Maze::Maze() {}

Maze::~Maze()
{
    removeMap();
    this->_instatnce = nullptr;
}

void Maze::initMap(const uint32_t x, const uint32_t y)
{
    this->_x = x;
    this->_y = y;

    this->_mazeMap = new MazeStatus *[this->_y];
    for (auto i = 0; i < this->_y; i++) {
        _mazeMap[i] = new MazeStatus[this->_x];
        for (auto j = 0; j < this->_x; j++)
            _mazeMap[i][j] = MazeStatus::empty;
    }
}

void Maze::removeMap()
{
    if (this->_mazeMap == nullptr)
        return;

    for (auto i = 0; i < this->_y; i++)
        delete[] _mazeMap[i];
    delete[] _mazeMap;
    _mazeMap = nullptr;
    this->_x = 0;
    this->_y = 0;
}

Maze *Maze::GetInstance()
{
    if (_instatnce == nullptr)
        _instatnce = new Maze();

    return _instatnce;
}

Maze *Maze::GetInstance(const uint32_t x, const uint32_t y)
{
    Maze::GetInstance();
    if (_instatnce->_x == x && _instatnce->_y == y)
        return _instatnce;
    _instatnce->removeMap();
    _instatnce->initMap(x, y);
    return _instatnce;
}

bool Maze::Place(
    const uint32_t wx,
    const uint32_t wy,
    const MazeStatus it)
{
    if (wx < 0 || wx >= this->_x || wy < 0 || wy >= this->_y)
        return false;
    _mazeMap[wy][wx] = it;
    return true;
}

bool Maze::Walkable(const uint32_t wx, const uint32_t wy)
{
    if (wx < 0 || wx >= this->_x || wy < 0 || wy >= this->_y)
        return false;
    if (_mazeMap[wy][wx] == MazeStatus::wall)
        return false;

    return true;
}
void Maze::PrintAtLoc(const int x, const int y)
{
    for (int i = 0; i != this->_y; ++i, std::cerr << '\n')
        for (int j = 0; j != this->_x; ++j) {
            switch (this->_mazeMap[i][j]) {
            case MazeStatus::wall:
                std::cerr << '#';
                break;
            case MazeStatus::empty:
                if (i == y && j == x)
                    std::cerr << 'O';
                else
                    std::cerr << '.';
                break;
            default:
                throw "unexcpeted character " +
                    this->_mazeMap[i][j];
                break;
            }
        }
}
const std::pair<uint32_t, uint32_t> Maze::GetSize()
{
    return std::make_pair(this->_x, this->_y);
}