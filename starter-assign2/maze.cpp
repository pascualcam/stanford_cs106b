// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function receives a maze (grid) and the current location. It returns
 * a set containing all the valid moves from the current location. A valid
 * move is defined as 1. a -one- step move, 2. in the N, S, E, or W directions,
 * 3. in an open corridor (not possible to move to a "wall" space and
 * 4. is in-bounds.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    // Initialize sets: return set and temporary set
    Set<GridLocation> neighbors;
    Set<GridLocation> potential;

    // Initialize potential locations
    GridLocation north = {cur.row-1, cur.col}; // negative is up/left in grid
    GridLocation south = {cur.row+1, cur.col}; // positive is down/right in grid
    GridLocation east = {cur.row, cur.col+1};
    GridLocation west = {cur.row, cur.col-1};

    // Check if potential locations are in bounds
//    if (maze.inBounds(north) == true) {
//        potential.add(north);
//    } if (maze.inBounds(south) == true) {
//        potential.add(south);
//    } if (maze.inBounds(east) == true) {
//        potential.add(east);
//    } if (maze.inBounds(west) == true) {
//        potential.add(west);
//    }

//    // Check if potential locations open corridor or wall
//    for (GridLocation loc : potential) {
//        // If location is an open corridor, add to 'neighbors'
//        if (maze[loc] == true) {
//            neighbors.add(loc);
//        }
//    }
    return neighbors;
    // Check if potential locations are in bounds
    if (maze.inBounds(north) == true) {
        potential.add(north);
    } if (maze.inBounds(south) == true) {
        potential.add(south);
    } if (maze.inBounds(east) == true) {
        potential.add(east);
    } if (maze.inBounds(west) == true) {
        potential.add(west);
    }

    // Check if potential locations open corridor or wall
    for (GridLocation loc : potential) {
        // If location is an open corridor, add to 'neighbors'
        if (maze[loc] == true) {
            neighbors.add(loc);
        }
    }
    return neighbors;
}

/* This function accepts as input a maze and a path to check if it meets
 * the criteria to be a Solution to the maze. The function completes four checks
 * (criteria) which are as follows:
 *      1. Check if maze ends at exit point (bottom right of maze)
 *      2. Check if maze starts at entry point (top left of maze)
 *      3. Check if every move is a valid move (i to i+1 is valid)
 *      4. Check that move is along a corridor, and not through a wall
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0,0};
    // Initialize vector to keep track of path steps
    Vector<GridLocation> pathsteps;

    // Check if maze ends at exit
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    // Add top GridLocation (end) to vector
    pathsteps.add(path.pop());

    // Loop through all GridLocations in path and add them to vector pathsteps in order
    while (path.isEmpty() != true) {
        pathsteps.add(path.pop());
    }

    // Check if maze starts at entry
    int length = pathsteps.size();
    if (pathsteps[length-1] != mazeEntry) {
        error("Path does not start at maze entry");
    }

    // Loop through vector pathsteps and check that move from i to i+1 is valid.
    for (int i = pathsteps.size()-1; i> 0; i--) {
        GridLocation step = pathsteps[i];
        GridLocation nextstep = pathsteps[i-1];
        if (generateValidMoves(maze,step).contains(nextstep) == false) {
               error("Step from a location x to a location x+1 is not valid");
        }
    }
}

/* This function solves a maze passed as a parameter. It follows the
 * Breadth-First Search algorithm to look at all the possibilities of every
 * step in the maze. It constantly checks for the "exit" from the maze,
 * when it does find, it returns the path (as a Stack), where the path is
 * the solution to the input maze.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    // Initialize Queue and Stack
    Stack<GridLocation> path;
    Queue<Stack<GridLocation>> Q;

    // Set entry and exit points
    GridLocation mazeEntry = {0,0};
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    // Construct Queue
    path.push(mazeEntry); // add entry to path
    Q.enqueue(path); // add path (containing only Entry)

    // Vector to check GridLocations already visited
    Vector<GridLocation> seen;
    seen.add(mazeEntry);

    // Explore all enqueued paths
    while (Q.isEmpty() == false) {
        Stack<GridLocation> path = Q.dequeue();

        // Check if path ends at Exit, if so return that path
        if (path.peek() == mazeExit) {
            MazeGraphics::highlightPath(path, "green");
            return path;
        } else {
            Set<GridLocation> neighbors = generateValidMoves(maze,path.peek()); // check neighbors of path's end

            // For each neighbor of the "path's end", make a copy of the path and add a neighbor
            for (int i=0; neighbors.size();i++) {
                Stack<GridLocation> newpath = path; // make copy
                GridLocation x = neighbors.first();
                // Checks if GridLocation has been visited
                if (seen.contains(x) != true) {
                    newpath.push(x);
                    neighbors.remove(x);
                    seen.add(x);
                    Q.enqueue(newpath);
                } else {
                    neighbors.remove(x);
                }
            }
            neighbors.clear();
        }

    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

// Student Tests
STUDENT_TEST("generateValidMoves on corner of 3x3 grid with walls") {
    Grid<bool> maze = {{true, false, false},
                       {false, false, true},
                       {true, false, true}};
    GridLocation corner = {2, 2};
    Set<GridLocation> neighbors = {{1,2}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

STUDENT_TEST("Does not generateValidMoves on corner of 3x3 grid with all walls") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, true}};
    GridLocation corner = {2, 2};
    Set<GridLocation> neighbors = {};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

STUDENT_TEST("Does not generateValidMoves on center of 3x3 grid surrounded by walls") {
    Grid<bool> maze = {{false, false, false},
                       {false, true, false},
                       {false, false, true}};
    GridLocation corner = {1, 1};
    Set<GridLocation> neighbors = {};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}


STUDENT_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> out_of_bounds = { {0, 0}, {1, 0}, {2,0} };
    Stack<GridLocation> all_out_of_bounds = { {3, 0}, {3, 1}, {3,2} };
    Stack<GridLocation> diagonal = { {0, 0}, {1, 1} };
    Stack<GridLocation> reverse = { {1, 1}, {1,0}, {0,0} };

    EXPECT_ERROR(checkSolution(maze, out_of_bounds));
    EXPECT_ERROR(checkSolution(maze, all_out_of_bounds));
    EXPECT_ERROR(checkSolution(maze, diagonal));
    EXPECT_ERROR(checkSolution(maze, reverse));
}

STUDENT_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, false}};
    Stack<GridLocation> no_path = { {0, 0}, {0, 1}, {0,2}, {1,2}, {2,2}, {3,2} };

    EXPECT_ERROR(checkSolution(maze, no_path));
}

STUDENT_TEST("solveMaze on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}
