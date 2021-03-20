#include "mapAdapter.h"
#include "errors.h"
#include <iostream>
#include <stdexcept>

#pragma region interface
bool           IMapAdapter::Init(Maze* const src) { throw NotImplementedError(); }

bool IMapAdapter::Consume(void* const input, const uint32_t x, const uint32_t y)
{
    throw NotImplementedError();
}
#pragma endregion
#pragma region coding_game_adapter
CDGMapAdapter::CDGMapAdapter()
{
}
CDGMapAdapter::~CDGMapAdapter()
{
    if (this->_startPoint != nullptr) {
        delete[] this->_startPoint;
        this->_startPoint = nullptr;
    }
    if (this->_maze != nullptr) {
        this->_maze = nullptr;
    }
}

bool CDGMapAdapter::Init(Maze* const src)
{
    if (src == nullptr)
        return false;
    this->_maze = src;
    return true;
}

bool CDGMapAdapter::Consume(void* const input, const uint32_t x, const uint32_t y)
{
#ifdef DEBUG
    std::cerr << "[start]\n";
#endif

    for (auto i = 0; i < y; ++i) {
        for (auto j = 0; j < x; ++j) {
            auto c = ((char** const)input)[i][j];

#ifdef DEBUG
            std::cerr << c;
            std::cerr.flush();
#endif

            switch (c) {
            case '.':
                break;
            case '#':
                if (this->_maze->Place(j, i, Maze::wall))
                    break;
                return false;
            case 'O':
                this->_startPoint = new std::pair<uint32_t, uint32_t>(j, i);
                break;
            default:
                std::string s = "unexpected char ";
                s += c;
                throw std::range_error(s);
            }
        }
#ifdef DEBUG
        std::cerr << '\n'
#endif
    }
#ifdef DEBUG
    std::cerr << "[end]\n";
    std::cerr.flush();
#endif

    return true;
}

const std::pair<uint32_t, uint32_t>& CDGMapAdapter::StartPoint()
{
    return *this->_startPoint;
}
#pragma endregion
