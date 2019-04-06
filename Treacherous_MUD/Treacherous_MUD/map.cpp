
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "rapidjson/document.h"
#include "map.hpp"
#include "tile_data.hpp"

/* Load map from disk using JSON/Tiled */
void Map::loadJSON(std::map<std::string, Tile>& tileAtlas)
{
	std::vector<std::vector<int>> vec_data;
	std::ifstream in("media/testmap2.json");
	std::string contents((std::istreambuf_iterator<char>(in)), 
    std::istreambuf_iterator<char>());
	rapidjson::Document document;
	std::string foundName;
	document.Parse(contents.c_str());
	if(document.IsObject())
	{
		int mapHeight = document["height"].GetInt();
		int mapWidth = document["width"].GetInt();
		this->height = mapHeight;
		this->width = mapWidth;
		if(document.HasMember("layers"))
		{
			const rapidjson::Value& layerArray = document["layers"];
			assert(layerArray.IsArray());
			this->layers = layerArray.Size();
			vec_data.resize(mapHeight * mapWidth * this->layers);

			for (rapidjson::SizeType i = 0; i < layerArray.Size(); i++)
			{
				const rapidjson::Value & dataObject  = layerArray[i];
				if(dataObject.HasMember("data")){
					for (rapidjson::SizeType i = 0; i < dataObject["data"].Size(); i++)
					{
						const rapidjson::Value & name  = dataObject["data"][i];
						vec_data[i].emplace_back(name.GetInt());
					}
				}
			}
		}
		this->tiles.resize(mapHeight * mapWidth * this->layers);
		for(int pos = 0; pos < this->width * this->height; ++pos)
		{
			for(int i = 0; i < this->layers; i++)
			{
				int tileID = vec_data[pos][i];
				if(tileID == 0)
				{
					this->tiles[i].emplace_back(tileAtlas.at("void"));
				}
				else
				{
					this->tiles[i].emplace_back(tileAtlas.at(std::to_string(tileID)));
				}
			}
		}
		
	for(auto i = 0; i < width; i++)
	{
		for(auto b = 0; b < height; b++)
		{
			area[i][b] = 1;
		}
	}
	}
}

void Map::draw(sf::RenderWindow& window, float dt)
{
    for(auto y = 0; y < this->height; ++y)
    {
        for(auto x = 0; x < this->width; ++x)
        {
            /* Set the position of the tile in the 2d world */
            sf::Vector2f pos;
            pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
            pos.y = (x + y) * this->tileSize * 0.5;
            this->tiles[0][y*this->width+x].sprite.setPosition(pos);
			
			if(!this->bfs.pathBackup.empty()) {
				for (auto& yy : this->bfs.pathBackup)
				{
					if(yy.first < this->height && yy.second < this->width)
					{
						int x1 = yy.first;
						int y1 = yy.second;
						if(!(this->tiles[0][y*this->width+x].tileType == TileType::VOID)) {
						this->tiles[0][y1*this->width+x1].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
						}
					}
				}
			this->bfs.pathBackup.clear();
			}
            /* Draw the tile */
			if(!(this->tiles[0][y*this->width+x].tileType == TileType::VOID))
			{
				this->tiles[0][y*this->width+x].draw(window, dt);
			}
		}
	}
}

void Map::drawObjects(sf::RenderWindow& window, float dt)
{
	for(int i = 1; i < this->layers; i++)
	{
	for(auto y = 0; y < this->height; ++y)
    {
        for(auto x = 0; x < this->width; ++x)
        {
			sf::Vector2f pos;
            pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
            pos.y = (x + y) * this->tileSize * 0.5;
            this->tiles[i][y*this->width+x].sprite.setPosition(pos);

			if(!(this->tiles[i][y*this->width+x].tileType == TileType::VOID))
			{
				this->tiles[i][y*this->width+x].drawStatic(window, dt);
			}

		}
	 }
	}
}

void Map::clearPath()
{
	for(auto y = 0; y < this->height; ++y)
    {
        for(auto x = 0; x < this->width; ++x)
		{
		this->tiles[0][y*this->width+x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));
		}
	}
}

void Map::select(sf::Vector2i location, std::vector<TileType> blacklist)
{
	const double x_diff = location.x - 0;
    const double y_diff = location.y - 0;
	if(std::sqrt(x_diff * x_diff + y_diff * y_diff) < 60)
	{
		clearPath();
		this->bfs.findPath(area,location.x,location.y,0,0);
	}
}