#include<iostream>
#include<cmath>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include <unordered_map>
#include <queue>
#include "player.cpp"
#define N 20

using namespace sf;

std::vector<std::pair<int, int>> pathBackup;
std::queue<int> instructions; // 0 = top, 1 = left, 2 = bottom, 3 = right
int matrix2[N][N];
bool focus;

struct Node
{
    // (x, y) represents coordinates of a cell in matrix
    int x, y;
    
    // path stores complete path from source cell to cell (x, y)
	std::vector<std::pair<int, int>> path;
 
    // As we are using struct as a key in a std::map,
    // we need to overload below operators
    
    bool const operator==(const Node& ob) const
    {
        return x == ob.x && y == ob.y;
    }
 
    bool operator<(const Node& ob) const
    {
        return x < ob.x || (x == ob.x && y < ob.y);
    }
};
 
// Below arrays details all 4 possible movements from a cell
int row[] = { -1, 0, 0, 1 };
int col[] = { 0, -1, 1, 0 };
 
// The function returns false if pt is not a valid position
bool isValid(int x, int y)
{
    return (x >= 0 && x < N) && (y >= 0 && y < N);
}
 
// Function to print the complete path from source to destination
void printPath(std::vector<std::pair<int, int>> path)
{
	pathBackup = path;
	//std::cout << "Destination Found:\t";
   // for (std::pair<int, int> p: pathBackup)
	//    std::cout << "(" << p.first << ", " << p.second << ") ";
	//std::cout << '\n';
	while(!instructions.empty())
	{
		instructions.pop();
	}

	for(int i = 0; i < pathBackup.size() -1; i++)
	{
		int x1 = pathBackup[i].first;
		int y1 = pathBackup[i].second;
		int x2 = pathBackup[i+1].first;
		int y2 = pathBackup[i+1].second;
		int newCoors = x1 - x2;
		int newCoors2 = y1 - y2;
		if(newCoors > 0 && newCoors2 == 0)
			{
				instructions.push(3);
			}
		else if(newCoors2 > 0 && newCoors == 0)
			{
				instructions.push(0);
			}
		else if(newCoors < 0 && newCoors2 == 0)
			{
				instructions.push(1);
			}
		else
			{
				instructions.push(2);
			}
	}

	std::cout << "START" << " ";
	while(!instructions.empty())
	{
		switch(instructions.front())
		{
			case 0: 
			std::cout << "North" << " ";
			break;
			case 1:
				std::cout << "East" << " ";
			break;
			case 2:
				std::cout << "South" << " ";
			break;
			case 3:
				std::cout << "West" << " ";
			break;
		}
		instructions.pop();
	}
	std::cout << "END" << " ";
}
 
// Find shortest route in the matrix from source cell (x, y) to
// destination cell (N - 1, N - 1)
int findPath(int matrix[][N], int x, int y, int endX, int endY)
{
	std::vector<std::pair<int, int>> path;
    path.push_back({x, y});

	pathBackup.clear();
    
    // create a queue and enqueue first node
	std::queue<Node> Q;
    Node src = {x, y, path};
    Q.push(src);
 
    // map to check if matrix cell is visited before or not
	std::map<Node, bool> visited;
    visited[src] = true;
 
    // run till queue is not empty
    while (!Q.empty())
    {
        // pop front node from queue and process it
        Node curr = Q.front();
        Q.pop();
 
        int i = curr.x, j = curr.y;
        path = curr.path;
 
        // if destination is found, return true
        if (i == endX && j == endY)
        {
            printPath(path);
            return path.size() - 1;
        }
 
        // value of current cell
        int n = matrix[i][j];
 
        // check all 4 possible movements from current cell
        // and recurse for each valid movement
        for (int k = 0; k < 4; k++)
        {
            // get next position coordinates using value of current cell
            int x = i + row[k] * n;
            int y = j + col[k] * n;
 
            // check if it is possible to go to next position
            // from current position
            if (isValid(x, y) && matrix[x][y] != 10)
            {
                // include next vertex in the path
                path.push_back({x, y});
 
                // construct next cell node
                Node next = {x, y, path};
            
                if (!visited.count(next))
                {
                    Q.push(next);
                    visited[next] = true;
                }
 
                // exclude next vertex in the path
                path.pop_back();
            }
        }
    }
 
    // return INFINITY if path is not possible
    return INT_MAX;
}

void updatePath(std::vector<RectangleShape> &path, RectangleShape &pathShape, float gridSize )
{
	int len = findPath(matrix2, 0, 0, 8, 8);

	// if (len != INT_MAX)
	// 	std::cout << "Shortest Path length is " << len;
	// else
	// 	std::cout << "Error: you dun goofed " << std::endl;

	path.clear();

	for(auto &i : pathBackup)
	{
		pathShape.setPosition(i.first * gridSize, i.second * gridSize);
		path.push_back(pathShape);
	}
}

int main()
{
	int yy = 0;
	int zz = 0;
	Player player;

	for(int x = 0; x < N ; x++)
	{
		for(int y = 0; y < N ; y++)
		{
			matrix2[x][y] = 1;
		}
	}

	const unsigned WINDOW_WIDTH = 1024;
	const unsigned WINDOW_HEIGHT = 768;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Treacherous MUD");
	window.setFramerateLimit(120);


	float dt;
	Clock dt_clock;
	const float gridSize = 50.f;


	Vector2i mousePosGrid;

	//Rectangle grid selection
	RectangleShape selection;
	selection.setSize(Vector2f(gridSize,gridSize));
	selection.setOutlineThickness(2);
	selection.setOutlineColor(Color::Yellow);
	selection.setFillColor(Color::Transparent);

	RectangleShape gridGround;
	std::vector<RectangleShape> gridGroundList;
	gridGround.setSize(Vector2f(gridSize,gridSize));
	gridGround.setOutlineColor(Color::White);
	gridGround.setOutlineThickness(-0.5f);
	gridGround.setFillColor(Color::Transparent);

	for(int i = 0; i < N; i++)
	{
		for(int z = 0; z < N; z++)
		{
			gridGround.setPosition(i * gridSize, z * gridSize);
			gridGroundList.push_back(gridGround);
		}
	}

	//text
	std::vector<Text> texts(gridSize * gridSize);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("arial.ttf");

	int i = 0;
	int b = 0;
 
	 // for (auto &texts_value : texts)
	 // {
	 // 	if(i > gridSize)
	 // 	{
	 // 		i = 0;
	 // 		b += 1;
	 // 	}
	 // 	texts_value.setFont(font);
	 // 	texts_value.setPosition(gridSize * i, gridSize * b);
		// texts_value.setCharacterSize(20);
	 // 	texts_value.setString(std::to_string(i));
		//  i += 1;
	 // }


	//Walls
	std::vector<RectangleShape> walls;
	RectangleShape wall;
	wall.setFillColor(Color::Red);
	wall.setSize(Vector2f(gridSize, gridSize));

	std::vector<RectangleShape> path;
	RectangleShape pathShape;
	pathShape.setFillColor(Color::Blue);
	pathShape.setSize(Vector2f(gridSize, gridSize));

	updatePath(path,pathShape,gridSize);

	//Collision
	FloatRect nextPos;


	while (window.isOpen())
	{
		dt = dt_clock.restart().asSeconds();

		
		mousePosGrid.x = Mouse::getPosition(window).x / int(gridSize);
		mousePosGrid.y = Mouse::getPosition(window).y / int(gridSize);

		selection.setPosition(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//Add walls
		if (Mouse::isButtonPressed(Mouse::Middle))
		{
			bool exists = false;
			for (size_t i = 0; i < walls.size() && !exists; i++)
			{
				if (walls[i].getPosition().x / (int)gridSize == mousePosGrid.x
					&& walls[i].getPosition().y / (int)gridSize == mousePosGrid.y)
				{
					exists = true;
				}
			}

			if (!exists)
			{
				if(mousePosGrid.x || mousePosGrid.y <= N)
				{
				matrix2[mousePosGrid.x][mousePosGrid.y] = 10;
				wall.setPosition(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);
				walls.push_back(wall);
				//updatePath(path,pathShape,gridSize);
				}
			}
		}

		if(Mouse::isButtonPressed(Mouse::Left))
		{
			findPath(matrix2,(int)player.selection.getPosition().x / gridSize,(int) player.selection.getPosition().y / gridSize, mousePosGrid.x,mousePosGrid.y);
			path.clear();

		for(auto &i : pathBackup)
	{
			pathShape.setPosition(i.first * gridSize, i.second * gridSize);
			path.push_back(pathShape);
	}
		}

		//Remove wall
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			bool exists = false;
			int index = -1;
			for (size_t i = 0; i < walls.size() && !exists; i++)
			{
				if (walls[i].getPosition().x / (int)gridSize == mousePosGrid.x
					&& walls[i].getPosition().y / (int)gridSize == mousePosGrid.y)
				{
					exists = true;
					index = i;
				}
			}

			if (exists)
			{
				if(mousePosGrid.x || mousePosGrid.y <= N)
				{
				matrix2[mousePosGrid.x][mousePosGrid.y] = 1;
				walls.erase(walls.begin() + index);
				//updatePath(path,pathShape,gridSize);
				}
			}
		}
		//Render
		window.clear();

		
		for(auto &y : path)
		{
			window.draw(y);
		}

		for (auto &g : walls)
		{
			window.draw(g);
		}
		window.draw(selection);

		for (auto &u : gridGroundList)
		{
			window.draw(u);
		}
		window.draw(selection);

		window.draw(player.selection);
  //
		// for (auto& texty : texts)
  //       {
  //          window.draw(texty);
  //       }

		window.display();
	}

	return 0;
}