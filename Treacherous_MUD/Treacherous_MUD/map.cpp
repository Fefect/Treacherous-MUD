
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "rapidjson/document.h"
#include "map.hpp"
#include "Tile.hpp"

/* Load map from disk */
void Map::load(const std::string& filename, unsigned int width, unsigned int height,
    std::map<std::string, Tile>& tileAtlas)
	{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);
 
 //    this->width = width;
 //    this->height = height;
 //
	// for(int x = 0; x < this->width; x++)
	// {
	// 	for(int y = 0; y < this->width; y++)
	// 	{
	// 	TileType tileType;
 //        inputFile.read((char*)&tileType, sizeof(int));
 //        switch(tileType)
 //        {
 //            default:
 //            case TileType::VOID:
 //            case TileType::GRASS:
 //                this->tiles.push_back(tileAtlas.at("grass"));
 //                break;
 //            case TileType::FOREST:
 //                this->tiles.push_back(tileAtlas.at("forest"));
 //                break;
 //            case TileType::WATER:
 //                this->tiles.push_back(tileAtlas.at("water"));
 //                break;
 //            case TileType::RESIDENTIAL:
 //                this->tiles.push_back(tileAtlas.at("residential"));
 //                break;
 //            case TileType::COMMERCIAL:
 //                this->tiles.push_back(tileAtlas.at("commercial"));
 //                break;
 //            case TileType::INDUSTRIAL:
 //                this->tiles.push_back(tileAtlas.at("industrial"));
 //                break;
 //            case TileType::ROAD:
 //                this->tiles.push_back(tileAtlas.at("road"));
 //                break;
 //        }
 //        Tile& tile = this->tiles.back();
	// 	tile.corX = x;
	// 	tile.corY = y;
 //        inputFile.read((char*)&tile.tileVariant, sizeof(int));
 //        inputFile.read((char*)&tile.regions, sizeof(int)*1);
 //    }
	// }
 //
 //    inputFile.close();
 //
	// for(auto i = 0; i < width; i++)
	// {
	// 	for(auto b = 0; b < height; b++)
	// 	{
	// 		area[i][b] = 1;
	// 	}
	// }

	loadJSON(tileAtlas);
}

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
		vec_data.resize(mapHeight * mapWidth * 3);
		int amountOfLayers = 0;
		if(document.HasMember("layers"))
		{
			const rapidjson::Value& layerArray = document["layers"];
			assert(layerArray.IsArray());
			for (rapidjson::SizeType i = 0; i < layerArray.Size(); i++)
			{
				const rapidjson::Value & dataObject  = layerArray[i];
				if(dataObject.HasMember("data")){
					amountOfLayers += 1;
					for (rapidjson::SizeType i = 0; i < dataObject["data"].Size(); i++)
					{
						const rapidjson::Value & name  = dataObject["data"][i];
						vec_data[i].emplace_back(name.GetInt());
					}
				}
			}
		}
		this->tiles.resize(mapHeight * mapWidth * amountOfLayers);
		for(int pos = 0; pos < this->width * this->height; ++pos)
		{
			for(int i = 0; i < amountOfLayers; i++)
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
				//this->tiles[i].emplace_back(tileAtlas.at(std::to_string(tileID)));
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

void Map::save(const std::string& filename)
{
    
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
			//if(!(this->tiles[0][y*this->width+x].tileType == TileType::VOID))
			{
				this->tiles[0][y*this->width+x].draw(window, dt);
			}
		}
	}
}

void Map::drawObjects(sf::RenderWindow& window, float dt)
{
	for(int i = 1; i < 3; i++)
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
				//this->tiles[i][y*this->width+x].draw(window, dt);
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
