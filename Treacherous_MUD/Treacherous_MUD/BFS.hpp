#ifndef BFS_HPP
#define BFS_HPP
#include <queue>
#include <SFML/Graphics/RectangleShape.hpp>

class BFS
{
public:

	BFS();
	~BFS();
	int findPath(int matrix[64][64], int x, int y, int endX, int endY);

	std::vector<std::pair<int, int>> pathBackup;

	std::queue<int> instructions;

private:

	static bool isValid(int x, int y, int N);

	void printPath(std::vector<std::pair<int, int>> path);

	static void updatePath(std::vector<sf::RectangleShape> &path, sf::RectangleShape &pathShape, float gridSize );

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
};

#endif /* BFS_HPP */