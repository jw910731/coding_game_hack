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
    static Maze *GetInstance(uint32_t x, uint32_t y);
    static Maze *GetInstance();

    bool Place(uint32_t wx, uint32_t wy, MazeStatus it);
    ~Maze();
#pragma endregion
private:
#pragma region data_fields
    static Maze *_instatnce;

    uint32_t _x = 0, _y = 0;
    MazeStatus **_mazeMap = nullptr;
#pragma endregion
#pragma region methods
    Maze(uint32_t x, uint32_t y);
    Maze();

    void initMap(uint32_t x, uint32_t y);
    void removeMap();
#pragma endregion
};
