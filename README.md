# Maze-Solving Robot with Flood-Fill Algorithm

This project implements a maze-solving robot using an **ESP32 board**, equipped with **VL53L0X ToF sensors**, **TB6612 motor controller**, and **N20 encoder motors**. The robot navigates through a maze using the **flood-fill algorithm**, identifying obstacles, mapping the maze, and performing a fast run to reach the destination optimally.

---

## **Features**

- **Hardware Components**:
  - Esp32 microcontroller
  - VL53L0X ToF distance sensors
  - TB6612 motor driver
  - N20 encoder motors
  - Custom-designed chassis (optional)

- **Algorithms**:
  - Flood-fill algorithm for mapping and navigating the maze.
  - Encoder-based motor control for precise movement and turning.
  - Obstacle detection and wall mapping using ToF sensors.

- **Phases**:
  1. **Search Phase**: The robot explores the maze, identifying walls and computing the shortest path.
  2. **Analysis Phase**: The flood-fill algorithm analyzes the maze to determine the optimal route.
  3. **Fast Run Phase**: The robot navigates the maze using the shortest computed path.

---

## **Folder Structure**

```plaintext
.
├── src/
│   ├── main.ino           # Main Arduino sketch
│   ├── Motors.h           # Motor control logic
│   ├── Sensors.h          # ToF sensor management
│   ├── Encoders.h         # Encoder-based odometry
│   ├── MazeSolver.h       # Flood-fill algorithm and maze-solving logic
├── README.md              # Project documentation
├── LICENSE                # License information (optional)
└── .gitignore             # Ignore unnecessary files
