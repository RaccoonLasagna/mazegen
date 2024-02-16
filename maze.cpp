#include <array>
#include <vector>
#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>

class MazeGen
{
private:
    std::array<std::array<char, 15>, 15> maze = {{
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    }};
    std::vector<std::pair<int, int>> unvisited;
    std::vector<std::pair<int, int>> visited;
    std::pair<int, int> current_coord{-1, -1};
    std::pair<int, int> start_coord{-1, -1};
    char current_direction{'s'};

public:
    MazeGen()
    {

        for (size_t i = 1; i < 14; i += 2)
        {
            for (size_t j = 1; j < 14; j += 2)
            {
                std::pair coord{i, j};
                unvisited.push_back(coord);
            }
        }

        unsigned int start = 2 * (rand() % 7) + 1; // rng for 0 - 6, *2 + 1 = 1,3,5,7,9,11,
        start_coord = std::pair{start, 0};
        maze[0][start] = 'i';
        maze[1][start] = ' ';
        current_coord = {start, 1};
        // record visited coord
        if (std::find(visited.begin(), visited.end(), current_coord) == visited.end())
        {
            visited.push_back(current_coord);
        }
        unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), current_coord), unvisited.end()); // remove the start from the unvisited list
        std::cout << current_coord.first << ',' << current_coord.second << std::endl;
        bool exit_created{false};

        size_t count = 0;

        // when there are still unvisited tiles:
        while (unvisited.size() > 0)
        {
            while (movable())
            {
                if (current_coord.second == 13 && !exit_created)
                {
                    maze[14][current_coord.first] = 'o';
                    exit_created = true;
                }
                move_rand();
                count++;
            }
            relocate();
        }

        current_coord = start_coord;
    }

    void move(char direction, char printchar = ' ')
    {
        switch (direction)
        {
        case 'n':
            // if going up will stay within index and within a path that hasn't gone yet (is in unvisited):
            if (current_coord.second - 2 >= 1 && (std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first, current_coord.second - 2)) != unvisited.end()))
            {
                // walk up
                current_coord.second -= 2;
                // turn the tiles walked on to whitespace
                maze[current_coord.second + 1][current_coord.first] = printchar;
                // remove that tile from unvisited if the tile is in unvisited
            }
            break;
        case 's':
            if (current_coord.second + 2 <= 13 && (std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first, current_coord.second + 2)) != unvisited.end()))
            {
                current_coord.second += 2;
                maze[current_coord.second - 1][current_coord.first] = printchar;
            }
            break;
        case 'e':
            if (current_coord.first + 2 <= 13 && (std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first + 2, current_coord.second)) != unvisited.end()))
            {
                current_coord.first += 2;
                maze[current_coord.second][current_coord.first - 1] = printchar;
            }
            break;
        case 'w':
            if (current_coord.first - 2 >= 1 && (std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first - 2, current_coord.second)) != unvisited.end()))
            {
                current_coord.first -= 2;
                maze[current_coord.second][current_coord.first + 1] = printchar;
            }
            break;
        default:
            break;
        }
        maze[current_coord.second][current_coord.first] = printchar;
        if (std::find(visited.begin(), visited.end(), current_coord) == visited.end())
        {
            visited.push_back(current_coord);
        }
        unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), current_coord), unvisited.end());
    }

    void move_rand()
    {
        int direction;
        direction = rand() % 4; // rng of 0-3
        switch (direction)
        {
        case 0:
            move('n');
            break;

        case 1:
            move('s');
            break;

        case 2:
            move('e');
            break;

        case 3:
            move('w');
            break;

        default:
            std::cout << "how" << std::endl;
        }
    }

    bool movable()
    {
        bool north = current_coord.second - 2 >= 1 && std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first, current_coord.second - 2)) != unvisited.end();
        bool south = current_coord.second + 2 <= 13 && std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first, current_coord.second + 2)) != unvisited.end();
        bool east = current_coord.first + 2 <= 13 && std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first + 2, current_coord.second)) != unvisited.end();
        bool west = current_coord.first - 2 >= 1 && std::find(unvisited.begin(), unvisited.end(), std::make_pair(current_coord.first - 2, current_coord.second)) != unvisited.end();

        return (north || south || east || west);
    }

    void print_maze(bool location = false)
    {
        int y{0};
        for (std::array<char, 15> line : maze)
        {
            int x{0};
            for (char c : line)
            {
                if (location)
                {
                    if (x == current_coord.first && y == current_coord.second)
                    {
                        std::cout << '@' << ' ';
                    }
                    else
                    {
                        std::cout << c << ' ';
                    }
                }
                else
                {
                    std::cout << c << ' ';
                }

                x++;
            }
            std::cout << std::endl;
            y++;
        }
    }

    void relocate()
    {
        if (!visited.empty())
        {
            unsigned int randomIndex = rand() % visited.size();
            current_coord = visited[randomIndex];
        }
    }

    void print_unvisited()
    {
        for (std::pair coord : unvisited)
        {
            std::cout << '(' << coord.first << ',' << coord.second << ')' << std::endl;
        }
    }

    void print_visited()
    {
        for (std::pair coord : visited)
        {
            std::cout << '(' << coord.first << ',' << coord.second << ')' << std::endl;
        }
    }

    void print_current_coord()
    {
        std::cout << '<' << current_coord.first << ',' << current_coord.second << '>' << std::endl;
    }

    void solve()
    {
        maze[current_coord.second][current_coord.first] = '.';
        current_coord.second += 1;
        maze[current_coord.second][current_coord.first] = '.';
        // loop until at the top of the 
        while (maze[current_coord.second+1][current_coord.first] != 'o')
        {
            // if right is a walkable path, turn right and walk
            if (get_right() == ' ' || get_right() == '.')
            {
                turn_right();
                move_solve();
            }
            // otherwise, if the right and front is a wall, turn left
            else if (get_right() == '#' && get_front() == '#')
            {
                turn_left();
            }
            // otherwise, walk forward
            else{
                move_solve();
            }
        }
    }

    char get_front()
    {
        switch (current_direction)
        {
        case 'n':
            return maze[current_coord.second - 1][current_coord.first];
            break;

        case 's':
            return maze[current_coord.second + 1][current_coord.first];
            break;

        case 'e':
            return maze[current_coord.second][current_coord.first + 1];
            break;

        case 'w':
            return maze[current_coord.second][current_coord.first - 1];
            break;

        default:
            std::cout << "how" << std::endl;
            return ' ';
        }
    }

    char get_right()
    {
        switch (current_direction)
        {
        case 'n':
            return maze[current_coord.second][current_coord.first + 1];
            break;

        case 's':
            return maze[current_coord.second][current_coord.first - 1];
            break;

        case 'e':
            return maze[current_coord.second + 1][current_coord.first];
            break;

        case 'w':
            return maze[current_coord.second - 1][current_coord.first];
            break;

        default:
            std::cout << "how" << std::endl;
            return ' ';
        }
    }

    void turn_right()
    {
        switch (current_direction)
        {
        case 'n':
            current_direction = 'e';
            break;

        case 's':
            current_direction = 'w';
            break;

        case 'e':
            current_direction = 's';
            break;

        case 'w':
            current_direction = 'n';
            break;

        default:
            std::cout << "how" << std::endl;
            return;
        }
    }

    void turn_left()
    {
        switch (current_direction)
        {
        case 'n':
            current_direction = 'w';
            break;

        case 's':
            current_direction = 'e';
            break;

        case 'e':
            current_direction = 'n';
            break;

        case 'w':
            current_direction = 's';
            break;

        default:
            std::cout << "how" << std::endl;
            return;
        }
    }

void move_solve(char printchar = '.')
    {
        switch (current_direction)
        {
        case 'n':
            if (current_coord.second - 2 >= 1)
            {
                current_coord.second -= 2;
                maze[current_coord.second + 1][current_coord.first] = printchar;
            }
            break;
        case 's':
            if (current_coord.second + 2 <= 13)
            {
                current_coord.second += 2;
                maze[current_coord.second - 1][current_coord.first] = printchar;
            }
            break;
        case 'e':
            if (current_coord.first + 2 <= 13)
            {
                current_coord.first += 2;
                maze[current_coord.second][current_coord.first - 1] = printchar;
            }
            break;
        case 'w':
            if (current_coord.first - 2 >= 1)
            {
                current_coord.first -= 2;
                maze[current_coord.second][current_coord.first + 1] = printchar;
            }
            break;
        default:
            break;
        }
        maze[current_coord.second][current_coord.first] = printchar;
    }
};

int main()
{
    srand(time(NULL));
    MazeGen maze;
    maze.print_maze();
    maze.solve();
    std::cout << "Solution: " << std::endl;
    maze.print_maze();
    return 0;
}