#pragma once
#include <stdint.h>
#include <utility>
#include "maze.h"

#pragma region interface
class IMapAdapter
{
    public:
        virtual bool Init(Maze* const src);
        virtual bool Consume(void* const input, 
                            const uint32_t x,
                            const uint32_t y);
};
#pragma endregion
#pragma region coding_game_adapter
class CDGMapAdapter final : public IMapAdapter
{
    private:
        Maze *_maze = nullptr;
        std::pair<uint32_t, uint32_t> *_startPoint = nullptr;
    public:
        virtual bool Init(Maze* const src) override;
        virtual bool Consume(void* const input, 
                            const uint32_t x,
                            const uint32_t y) override;
        
        const std::pair<uint32_t, uint32_t>& StartPoint();
        CDGMapAdapter();
        ~CDGMapAdapter();
};
#pragma endregion