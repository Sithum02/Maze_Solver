#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "Motors.h"
#include "Encoders.h"
#include "Sensors.h"

// Constants for the maze
#define MAZE_ROWS 5 // Adjust based on your maze dimensions
#define MAZE_COLS 5
#define MAX_DISTANCE 10000 // Default large value for unvisited cells

// Global variables for flood-fill algorithm
int maze[MAZE_ROWS][MAZE_COLS];  // Flood-fill values for the maze
bool visited[MAZE_ROWS][MAZE_COLS]; // Tracks visited cells
int currentRow = 0, currentCol = 0; // Robot's current position
char direction = 'N'; // Robot's current facing direction ('N', 'E', 'S', 'W')

// Directions for moving in the maze
const int dRow[] = {-1, 0, 1, 0}; // N, E, S, W
const int dCol[] = {0, 1, 0, -1};

// Initialize the maze with large values
void initializeMaze() {
    for (int i = 0; i < MAZE_ROWS; i++) {
        for (int j = 0; j < MAZE_COLS; j++) {
            maze[i][j] = MAX_DISTANCE;
            visited[i][j] = false;
        }
    }
    maze[MAZE_ROWS / 2][MAZE_COLS / 2] = 0; // Set the goal cell at the center
}

// Update flood-fill values based on neighbors
void updateFloodFill(int row, int col) {
    int minVal = MAX_DISTANCE;
    for (int i = 0; i < 4; i++) {
        int newRow = row + dRow[i];
        int newCol = col + dCol[i];
        if (newRow >= 0 && newRow < MAZE_ROWS && newCol >= 0 && newCol < MAZE_COLS) {
            minVal = min(minVal, maze[newRow][newCol]);
        }
    }
    maze[row][col] = minVal + 1;
}

// Function to determine the next cell to move
void getNextMove(int &nextRow, int &nextCol) {
    int minVal = MAX_DISTANCE;
    for (int i = 0; i < 4; i++) {
        int newRow = currentRow + dRow[i];
        int newCol = currentCol + dCol[i];
        if (newRow >= 0 && newRow < MAZE_ROWS && newCol >= 0 && newCol < MAZE_COLS &&
            !visited[newRow][newCol] && maze[newRow][newCol] < minVal) {
            minVal = maze[newRow][newCol];
            nextRow = newRow;
            nextCol = newCol;
        }
    }
}

// Turn the robot to face a specific direction
void turnTo(char targetDirection) {
    while (direction != targetDirection) {
        if ((direction == 'N' && targetDirection == 'E') ||
            (direction == 'E' && targetDirection == 'S') ||
            (direction == 'S' && targetDirection == 'W') ||
            (direction == 'W' && targetDirection == 'N')) {
            turnRight(128);
        } else {
            turnLeft(128);
        }
        // Update the direction based on the turn
        if (direction == 'N') direction = 'E';
        else if (direction == 'E') direction = 'S';
        else if (direction == 'S') direction = 'W';
        else if (direction == 'W') direction = 'N';
    }
}

// Move to a specific cell
void moveToCell(int targetRow, int targetCol) {
    if (targetRow == currentRow - 1) { // North
        turnTo('N');
    } else if (targetRow == currentRow + 1) { // South
        turnTo('S');
    } else if (targetCol == currentCol - 1) { // West
        turnTo('W');
    } else if (targetCol == currentCol + 1) { // East
        turnTo('E');
    }
    goForward(100, 128); // Move forward one cell (adjust distance as needed)
    currentRow = targetRow;
    currentCol = targetCol;
}

// Flood-fill search phase
void floodFillPhase() {
    initializeMaze();

    while (currentRow != MAZE_ROWS / 2 || currentCol != MAZE_COLS / 2) { // Until the goal is reached
        visited[currentRow][currentCol] = true;

        // Read distances from sensors
        int distanceL = getSensorDistance(XSHUT1_PIN, 0x30);
        int distanceR = getSensorDistance(XSHUT2_PIN, 0x31);
        int distanceF = getSensorDistance(XSHUT3_PIN, 0x32);

        // Mark walls in the maze
        if (distanceF < 200) {
            maze[currentRow + dRow[0]][currentCol + dCol[0]] = MAX_DISTANCE; // North wall
        }
        if (distanceR < 200) {
            maze[currentRow + dRow[1]][currentCol + dCol[1]] = MAX_DISTANCE; // East wall
        }
        if (distanceL < 200) {
            maze[currentRow + dRow[3]][currentCol + dCol[3]] = MAX_DISTANCE; // West wall
        }

        // Update flood-fill values
        updateFloodFill(currentRow, currentCol);

        // Determine the next cell to move
        int nextRow = currentRow, nextCol = currentCol;
        getNextMove(nextRow, nextCol);

        // Move to the next cell
        moveToCell(nextRow, nextCol);
    }
    Serial.println("Flood-fill search complete.");
}

// Fast-run phase
void fastRunPhase() {
    Serial.println("Starting fast run...");
    // Placeholder for optimal path execution
    // Use the maze values to trace the shortest path back to the start or follow the solved path
}

#endif
