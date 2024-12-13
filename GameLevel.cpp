/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "GameLevel.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (GameObject& tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject& tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    // Calculate dimensions of each tile
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / static_cast<float>(height);

    // Initialize each tile
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if (tileData[y][x] > 0) { // Only process non-zero tiles
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                // Determine color based on arrow shape
                glm::vec3 color = glm::vec3(1.0f); // Default: white
                if (y == height / 2) {
                    // Horizontal line of the arrow
                    color = glm::vec3(1.0f, 0.0f, 0.0f); // Red
                }
                else if (x == width / 2 && y < height / 2) {
                    // Vertical part of the arrowhead
                    color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
                }
                else if (abs(static_cast<int>(x - width / 2)) == (height / 2 - y) && y < height / 2) {
                    // Diagonal part of the arrowhead
                    color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
                }

                // Create and add the tile
                GameObject obj(pos, size, ResourceManager::GetTexture("block"), color);
                this->Bricks.push_back(obj);
            }
        }
    }
}
