#include "cMazeMaker.h"

cMazeMaker::cMazeMaker()
{
	this->m_maze_size[2] = {0};

	this->m_start_axis = 0;
	this->m_start_side = 0;

	static const unsigned int UP = 0;
	static const unsigned int DOWN = 1;
	static const unsigned int LEFT = 2;
	static const unsigned int RIGHT = 3;

	return;
}

// Select a random direction based on our options, append it to the current path, and move there
bool cMazeMaker::m_randomMove(bool first_move) 
{
	int random_neighbor;
	std::vector< std::vector< int > > unvisited_neighbors;

	for (int direction = 0; direction < 4; direction++) 
	{
		int possible_pmd[2] = { 0, 0 };

		if (direction == UP) 
		{
			possible_pmd[1] = -1;
		}
		else if (direction == DOWN) 
		{
			possible_pmd[1] = 1;
		}
		else if (direction == LEFT) 
		{
			possible_pmd[0] = -1;
		}
		else 
		{
			possible_pmd[0] = 1;
		}

		if (this->m_dfs_path.back()[0] + possible_pmd[0] * 2 > 0 &&
			this->m_dfs_path.back()[0] + possible_pmd[0] * 2 < this->m_maze_size[0] - 1 &&
			this->m_dfs_path.back()[1] + possible_pmd[1] * 2 > 0 &&
			this->m_dfs_path.back()[1] + possible_pmd[1] * 2 < this->m_maze_size[1] - 1) 
		{
			if ( !maze[this->m_dfs_path.back()[1] + possible_pmd[1] * 2] 
				      [this->m_dfs_path.back()[0] + possible_pmd[0] * 2][1].is_wall ) 
			{
				std::vector< int > possible_move_delta = { possible_pmd[0], possible_pmd[1] };

				unvisited_neighbors.push_back(possible_move_delta);
			}
		}
	}

	if (unvisited_neighbors.size() > 0) 
	{
		random_neighbor = rand() % unvisited_neighbors.size();

		for (int a = 0; a < !first_move + 1; a++) 
		{
			std::vector< int > new_location;

			new_location.push_back(this->m_dfs_path.back()[0] + unvisited_neighbors[random_neighbor][0]);
			new_location.push_back(this->m_dfs_path.back()[1] + unvisited_neighbors[random_neighbor][1]);

			this->m_dfs_path.push_back(new_location);

			this->maze[this->m_dfs_path.back()[1]][this->m_dfs_path.back()[0]][0].is_wall = false;
			this->maze[this->m_dfs_path.back()[1]][this->m_dfs_path.back()[0]][1].is_wall = true;
		}

		return true;
	}
	else 
	{
		return false;
	}
}

bool cMazeMaker::m_validInteger(char* cstr) 
{
	std::string str(cstr);

	for (char& c : str) 
	{
		if (!isdigit(c)) 
		{
			return false;
		}
	}

	return true;
}
// The fun part ;)
bool cMazeMaker::GenerateMaze(unsigned int width, unsigned int height) 
{
	this->m_maze_size[0] = width;
	this->m_maze_size[1] = height;

	if ( (width < 5) || (height < 5) )
	{
		return false;
	}

	// The width and height must be greater than or equal to 5 or it won't work
	// The width and height must be odd or else we will have extra walls
	for (int a = 0; a < 2; a++) 
	{
		if (this->m_maze_size[a] < 5) 
		{
			this->m_maze_size[a] = 5;
		}
		else if (this->m_maze_size[a] % 2 == 0) 
		{
			this->m_maze_size[a]--;
		}
	}

	this->m_initializeMaze();
	this->m_randomPoint(false);
	this->m_randomPoint(true);

	bool first_move = true;
	bool success = true;


	while (( int )this->m_dfs_path.size() > 1 - first_move) 
	{
		if (!success) 
		{
			this->m_dfs_path.pop_back();

			if (!first_move && this->m_dfs_path.size() > 2) 
			{
				this->m_dfs_path.pop_back();
			}
			else 
			{
				break;
			}

			success = true;
		}

		while (success) 
		{
			success = this->m_randomMove(first_move);

			if (first_move) 
			{
				first_move = false;
			}
		}
	}

	return true;
}

// Initialize the maze vector with a completely-filled grid with the size the user specified
void cMazeMaker::m_initializeMaze(void) 
{
	for (int a = 0; a < this->m_maze_size[1]; a++) 
	{
		for (int b = 0; b < this->m_maze_size[0]; b++) 
		{
			bool is_border;

			if (a == 0 || a == this->m_maze_size[1] - 1 ||
				b == 0 || b == this->m_maze_size[0] - 1) 
			{
				is_border = true;
			}
			else 
			{
				is_border = false;
			}

			maze_cube_info info;
			info.is_wall = is_border;

			maze_cube_info info2;
			info2.is_wall = true;

			std::vector< maze_cube_info > new_cell = { info2, info };

			if (( unsigned int )a + 1 > this->maze.size()) 
			{
				std::vector< std::vector< maze_cube_info > > new_row = { new_cell };

				this->maze.push_back(new_row);
			}
			else 
			{
				this->maze[a].push_back(new_cell);
			}
		}
	}
}




void cMazeMaker::PrintMaze(void) 
{
	for (unsigned int a = 0; a < this->maze.size(); a++) 
	{
		for (unsigned int b = 0; b < this->maze[a].size(); b++) 
		{
			if (this->maze[a][b][0].is_wall) 
			{
				std::cout << "||";
//				std::cout << "X";
//				std::cout << '\u2500';
//				std::cout << (char)0xC5;
//				std::cout << (char)0xDA; //   (218)
//				std::cout << (char)0xBF; //   (191)
//				std::cout << (char)0xC0; //   (192)
//				std::cout << (char)0xD9; //   (217)
			}
			else 
			{
				std::cout << "  ";
			}
		}

		std::cout << std::endl;
	}
}

// Set a random point (start or end)
void cMazeMaker::m_randomPoint(bool part) 
{
	int axis;
	int side;

	if (!part) 
	{
		axis = rand() % 2;
		side = rand() % 2;

		this->m_start_axis = axis;
		this->m_start_side = side;
	}
	else 
	{
		bool done = false;

		while (!done) 
		{
			axis = rand() % 2;
			side = rand() % 2;

			if (axis != this->m_start_axis ||
				side != this->m_start_side) 
			{
				done = true;
			}
		}
	}

	std::vector< int > location = { 0, 0 };

	if (!side) 
	{
		location[!axis] = 0;
	}
	else 
	{
		location[!axis] = this->m_maze_size[!axis] - 1;
	}

	location[axis] = 2 * ( rand() % ( ( this->m_maze_size[axis] + 1 ) / 2 - 2 ) ) + 1;

	if (!part) 
	{
		this->m_dfs_path.push_back(location);
	}

	this->maze[location[1]][location[0]][0].is_wall = false;
	this->maze[location[1]][location[0]][1].is_wall = true;

	return;
}
