
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>
 
#include "map.hpp"
#include "Tile.hpp"


/* Load map from disk */
void Map::load(const std::string& filename, unsigned int width, unsigned int height,
    std::map<std::string, Tile>& tileAtlas)
	{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);
 
    this->width = width;
    this->height = height;
 
    for(int pos = 0; pos < this->width * this->height; ++pos)
    {
        this->resources.push_back(255);
		this->selected.push_back(0);
 
        TileType tileType;
        inputFile.read((char*)&tileType, sizeof(int));
        switch(tileType)
        {
            default:
            case TileType::VOID:
            case TileType::GRASS:
                this->tiles.push_back(tileAtlas.at("grass"));
                break;
            case TileType::FOREST:
                this->tiles.push_back(tileAtlas.at("forest"));
                break;
            case TileType::WATER:
                this->tiles.push_back(tileAtlas.at("water"));
                break;
            case TileType::RESIDENTIAL:
                this->tiles.push_back(tileAtlas.at("residential"));
                break;
            case TileType::COMMERCIAL:
                this->tiles.push_back(tileAtlas.at("commercial"));
                break;
            case TileType::INDUSTRIAL:
                this->tiles.push_back(tileAtlas.at("industrial"));
                break;
            case TileType::ROAD:
                this->tiles.push_back(tileAtlas.at("road"));
                break;
        }
        Tile& tile = this->tiles.back();
        inputFile.read((char*)&tile.tileVariant, sizeof(int));
        inputFile.read((char*)&tile.regions, sizeof(int)*1);
        inputFile.read((char*)&tile.population, sizeof(double));
        inputFile.read((char*)&tile.storedGoods, sizeof(float));
    }
 
    inputFile.close();


	for(int i = 0; i < width -1; i++)
	{
		for(int y = 0; y < width -1; y++)
		{
			area[i][y] = 1;
		}
	}
}

void Map::save(const std::string& filename)
{
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);
 
    for(auto tile : this->tiles)
    {
        outputFile.write((char*)&tile.tileType, sizeof(int));
        outputFile.write((char*)&tile.tileVariant, sizeof(int));
        outputFile.write((char*)&tile.regions, sizeof(int)*1);
        outputFile.write((char*)&tile.population, sizeof(double));
        outputFile.write((char*)&tile.storedGoods, sizeof(float));
    }
 
    outputFile.close();
}

void Map::draw(sf::RenderWindow& window, float dt)
{
    for(int y = 0; y < this->height; ++y)
    {
        for(int x = 0; x < this->width; ++x)
        {
            /* Set the position of the tile in the 2d world */
            sf::Vector2f pos;
            pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
            pos.y = (x + y) * this->tileSize * 0.5;
            this->tiles[y*this->width+x].sprite.setPosition(pos);

			if(this->selected[y*this->width+x])
			{
			this->tiles[y*this->width+x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
				if(!this->bfs.pathBackup.empty()) {
				for(int yy = 0; yy < this->bfs.pathBackup.size(); yy++)
				{
				if(this->bfs.pathBackup[yy].first < 64 && this->bfs.pathBackup[yy].second < 64)
				{
					int x1 = this->bfs.pathBackup[yy].first;
					int y1 = this->bfs.pathBackup[yy].second;

				this->tiles[y1*this->width+x1].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
				}
				}
					this->bfs.pathBackup.clear();
				}
			}
            /* Draw the tile */
            this->tiles[y*this->width+x].draw(window, dt);
        }
    }
}

void Map::clearPath()
{
	for(int y = 0; y < this->height; ++y)
    {
        for(int x = 0; x < this->width; ++x)
		{
		this->tiles[y*this->width+x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));
	}
	}
}

void Map::clearSelected()
{
    for(auto& tile : this->selected) tile = 0;
 
    this->numSelected = 0;
}

void Map::select(sf::Vector2i location, std::vector<TileType> blacklist)
{

	clearPath();
	this->bfs.findPath(area,location.x,location.y,0,0);

    this->selected[location.y*this->width+location.x] = 1;
    ++this->numSelected;
    for(auto type : blacklist)
    {
        if(this->tiles[location.y*this->width+location.x].tileType == type)
        {
            this->selected[location.y*this->width+location.x] = 2;
            --this->numSelected;
            break;
        }
    }
}
