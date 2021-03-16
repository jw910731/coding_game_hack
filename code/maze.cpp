#include "maze.h"
#include <memory.h>

static Maze *_instatnce = nullptr;

void Maze::initMap(uint32_t x, uint32_t y)
{
    this->_mazeMap = new MazeStatus *[this->_y];
    for (auto i = 0; i < this->_y; i++)
    {
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
        delete [] _mazeMap[i];
    delete [] _mazeMap;
    _mazeMap = nullptr;
    this->_x = 0;
    this->_y = 0;
}

Maze::~Maze()
{
    removeMap();
    _instatnce = nullptr;
}

Maze* Maze::GetInstance()
{
    if (_instatnce == nullptr)
        _instatnce = new Maze();
    
    return _instatnce;
}

Maze* Maze::GetInstance(uint32_t x, uint32_t y)
{
    //TODO: finish it
    auto _ins = Maze::GetInstance();
    if (_ins->_mazeMap == nullptr)
    if (_instatnce == nullptr)
    {
        _instatnce = new Maze(x, y);
        _instatnce->initMap(x, y);
    }
    else
    {
        if (_instatnce->_x == x && _instatnce->_y == y)
            return _instatnce;
        _instatnce->removeMap();
        _instatnce->initMap(x, y);
    }
    return _instatnce;
}

bool Maze::Place(uint32_t wx, uint32_t wy, MazeStatus it)
{
    if (wx < 0 || wx >= this->_x || wy < 0 || wy >= this->_y)
        return false;
    _mazeMap[wy][wx] = it;
    return true;
}
