#pragma once
#include <stdint.h>

class Maze
{
public:
#pragma region structures
    enum MazeStatus
    {
        empty = 0,
        wall
    };
#pragma endregion
#pragma region methods
    static Maze *GetInstance(const uint32_t x, const uint32_t y);
    static Maze *GetInstance();

    bool Place(const uint32_t wx, const uint32_t wy, const MazeStatus it);
    bool Walkable(const uint32_t wx, const uint32_t wy);
    void PrintAtLoc(const int x, const int y);
    ~Maze();
#pragma endregion
private:
#pragma region data_fields
    static Maze *_instatnce;

    uint32_t _x = 0, _y = 0;
    MazeStatus **_mazeMap = nullptr;
#pragma endregion
#pragma region methods
    Maze(const uint32_t x, const uint32_t y);
    Maze();

    void initMap(const uint32_t x, const uint32_t y);
    void removeMap();
#pragma endregion
};
