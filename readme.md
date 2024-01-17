# Maze Algorithm Visualizer

## Introduction

This project features an interactive setup screen where users can design custom maze layouts on a grid. Users can navigate and modify tiles to create pathways and walls, visualized through a 3D representation of the maze. The project currently allows viewing the maze, with plans to animate the solving process.

Originating from a desire to explore topics beyond the scope of CS 450 Intro to Computer Graphics, this project focuses on modern OpenGL, vertex buffers, and shaders. It aims to bridge theoretical algorithm knowledge with practical application, providing experience in computer graphics through a BFS/A* visualizer.

## Technologies Used

- **Development Environment**: Visual Studio 2022
- **Programming Language**: C++
- **Libraries and Frameworks**:
  - OpenGL
  - GLEW (OpenGL Extension Wrangler)
  - GLUT (OpenGL Utility Toolkit)
  - GLM (OpenGL Mathematics)

## Features

**Current Features**:
- Interactive setup screen for custom maze layouts.
- Grid-based interface with tile navigation and state modification.
- 3D visualization of the user-designed maze.

**Upcoming Features**:
- Implementation of dynamic lighting.
- Development of an 'agent' class for maze solving using BFS and A* algorithms.
- Camera animation following the path determined by the search algorithm.
- Enhanced user instructions and display on the grid layout screen.

## Installation and Setup

**Prerequisites**:
- Visual Studio 2022 or later.
- A clone of this repository.

**Dependencies**:
- The project uses GLUT, GLEW, and GLM. GLM headers are included in the repository. DLLs for GLUT and GLEW are not included and must be downloaded separately.

**Setup Instructions**:
1. **Clone the Repository**:
   - Clone this repository to your local machine using Git or download it as a ZIP file and extract it.
2. **Download the DLLs**:
   - **GLUT**: Download the GLUT DLLs from a trusted source: https://www.opengl.org/resources/libraries/glut/glut_downloads.php
   - **GLEW**: Download the GLEW DLLs from a trusted source: https://glew.sourceforge.net/
3. **Place the DLLs**:
   - Place the downloaded DLL files in the `Debug` folder of the project within your local repository. If this doesn't exist yet, build the project once and Visual Studio will create it for you.
4. **Open the Project**:
   - Open the `.sln` file with Visual Studio 2022.
5. **Build and Run**:
   - Build the project in Visual Studio. If the DLLs are correctly placed, the project should build without any issues.
   - Run the project from within Visual Studio.

## Usage

- Use the arrow keys to select a tile, indicated by the flashing green outline. 
- Press the space bar to change the state of a tile. At this time, the start, end, and included tiles are all the same. A dark tile indicates that it will not be included in the maze.
- Press 'Enter' to submit your layout and proceed to the 3D maze screen.
- In the future, the camera will be animated to fly through the maze.
- For now, you can navigate around the maze using the arrow keys to move forward, backward, turn left, or turn right. Use the 'a' and 'd' keys to strafe left and right. Use 'e' or 'q' to move up or down.

- Press 'ESC' to return to the layout screen.

## Acknowledgements
- This project was adapted from a sample OpenGL project and makes use of glslprogram.h/.cpp and vertexbufferobject.h/.cpp provided courtesy of Mike Bailey during CS 450 Introduction to Computer Graphics at Oregon State University.

