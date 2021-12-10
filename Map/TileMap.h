#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"

class TileMap
{
private:
    float gridSizeF;
    unsigned gridSizeU;
    sf::Vector2u maxSize;
    unsigned layers;
    sf::Texture tileSheet;
    std::string textureFile; //the name of file for texture
    std::vector<std::vector<std::vector<Tile*>>> maps;

    void clear();

public:
    TileMap(float grid_size, unsigned width, unsigned height, std::string texture_file);
    virtual ~TileMap();

    //Accessors
    const sf::Texture* getTileSheet() const;


    //Functions
    void saveToFile(const std::string file_name);
    void loadFromFile(const std::string file_name);

    void update();
    void render(sf::RenderTarget& target);

    void addTile(const unsigned x, const unsigned y, const unsigned z, sf::IntRect& texture_rect);
    void removeTile(const unsigned x, const unsigned y, const unsigned z);
};

#endif // TILEMAP_H
