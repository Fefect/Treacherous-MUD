#include "BFS.hpp"
#include <map>
#include <iostream>


int row[] = { -1, 0, 0, 1 };
int col[] = { 0, -1, 1, 0 };

bool focus;

int BFS::findPath(int matrix[64][64], int x, int y, int endX, int endY)
{
	std::vector<std::pair<int, int>> path;
    path.emplace_back(x, y);


 	this->pathBackup.clear();
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
             if (isValid(x, y, 64) && matrix[x][y] != 10)
             {
                 // include next vertex in the path
                 path.emplace_back(x, y);
  
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

void BFS::updatePath(std::vector<sf::RectangleShape>& path, sf::RectangleShape& pathShape, float gridSize)
{
	return; //Empty for now
}

bool BFS::isValid(int x, int y, int N)
{
	return (x >= 0 && x < N) && (y >= 0 && y < N);
}

void BFS::printPath(std::vector<std::pair<int, int>> path)
 {
 	this->pathBackup = path;
 	std::cout << "Destination Found:\t";
   //  for (std::pair<int, int> p: pathBackup)
 	//   std::cout << "(" << p.first << ", " << p.second << ") ";
 	//std::cout << '\n';
 	// while(!instructions.empty())
 	// {
 	// 	instructions.pop();
 	// }
  //
 	// for(int i = 0; i < pathBackup.size() -1; i++)
 	// {
 	// 	int x1 = pathBackup[i].first;
 	// 	int y1 = pathBackup[i].second;
 	// 	int x2 = pathBackup[i+1].first;
 	// 	int y2 = pathBackup[i+1].second;
 	// 	int newCoors = x1 - x2;
 	// 	int newCoors2 = y1 - y2;
 	// 	if(newCoors > 0 && newCoors2 == 0)
 	// 		{
 	// 			instructions.push(3);
 	// 		}
 	// 	else if(newCoors2 > 0 && newCoors == 0)
 	// 		{
 	// 			instructions.push(0);
 	// 		}
 	// 	else if(newCoors < 0 && newCoors2 == 0)
 	// 		{
 	// 			instructions.push(1);
 	// 		}
 	// 	else
 	// 		{
 	// 			instructions.push(2);
 	// 		}
 	// }

 }


