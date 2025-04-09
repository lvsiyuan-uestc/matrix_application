# Matrix Application

## Description
This project is a matrix application that generates random mazes and highlights available paths using the Win32 API. It also includes features such as treasure collection and score tracking, creating an interactive visualization of maze traversal.

这个项目是一个矩阵应用程序，它使用 Win32 API 随机生成迷宫并高亮显示可用路径。此外，它还包含宝藏收集和分数记录的功能，提供了一个关于迷宫遍历的交互式可视化。

---

## Features / 功能

- **Random Maze Generation**: Uses Depth-First Search (DFS) to create random mazes.
  - **随机迷宫生成**：使用深度优先搜索 (DFS) 算法生成随机迷宫。
  
- **Treasure Placement**: Randomly places treasures within the maze.
  - **宝藏放置**：在迷宫中随机放置宝藏。
  
- **Pathfinding**: Highlights the path from the entrance to the exit.
  - **路径查找**：高亮显示从入口到出口的路径。
  
- **Score System**: Tracks the player's score as treasures are collected.
  - **计分系统**：记录玩家收集宝藏的得分。
  
- **Win32 Graphics**: Implements graphical rendering of the maze and pathfinding process.
  - **Win32 图形**：实现迷宫和路径查找过程的图形渲染。

---

## How to Run / 如何运行

1. Clone the repository:
   ```bash
   git clone https://github.com/lvsiyuan-uestc/matrix_application.git
   cd matrix_application
   ```

2. Open the project in a suitable C++ IDE (e.g., Visual Studio).

3. Compile and run the project. Ensure you have the necessary Windows development environment to support Win32 applications.

   1. 克隆代码仓库：
      ```bash
      git clone https://github.com/lvsiyuan-uestc/matrix_application.git
      cd matrix_application
      ```

   2. 在合适的 C++ IDE（如 Visual Studio）中打开项目。

   3. 编译并运行项目。确保您具备支持 Win32 应用程序的必要 Windows 开发环境。

---

## Controls / 控制

- **Arrow Keys**: Navigate the path within the maze.
- **Exit**: Escape the maze by reaching the exit.

- **方向键**：在迷宫中导航路径。
- **出口**：通过到达出口逃离迷宫。

---

## File Structure / 文件结构

- `main.cpp`: Contains the application logic for maze generation, pathfinding, and rendering.
  - `main.cpp`：包含迷宫生成、路径查找和渲染的应用逻辑。
  
- `maze.txt`: Used to save and load maze configurations.
  - `maze.txt`：用于保存和加载迷宫配置。

---

## License / 版权

This project is licensed under the MIT License. See `LICENSE` for more information.

此项目使用 MIT 许可证。有关详细信息，请参阅 `LICENSE`。


