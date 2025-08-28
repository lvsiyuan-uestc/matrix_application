#include<windows.h>
#include<string.h>
#include<tchar.h>
#include<stdio.h>
#include<stack>
#include <stdlib.h> // 用于 srand, rand
#include <time.h>   
#define MAX_SIZE 100  // 最大迷宫尺寸
// 定义宝藏的数量
#define TREASURE_COUNT 5  // 可以修改成希望的宝藏数目
using namespace std;
// 定义迷宫结构
typedef struct {
    int width;         // 迷宫宽度
    int height;        // 迷宫高度
    int entranceX;     // 入口X坐标
    int entranceY;     // 入口Y坐标
    int exitX;         // 出口X坐标
    int exitY;         // 出口Y坐标
    int maze[MAX_SIZE][MAX_SIZE];  // 迷宫数据
} Maze;

typedef struct {
    int path[MAX_SIZE][MAX_SIZE];
}Path;



// 定义宝藏类型
typedef struct {
    int x;
    int y;
    bool collected;  // 是否已被玩家收集
} Treasure;

Maze maze;
Path path;
Treasure treasures[TREASURE_COUNT];
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Matrix application");

int score = 0; // 玩家分数
int directionPriority = 0;

const int DX[] = { 0, 0, 1, -1 };
const int DY[] = { 1, -1, 0, 0 };

// 检查是否可以在 (nx, ny) 挖通路
bool isValid(int nx, int ny, Maze* maze) {
    if (nx <= 0 || nx >= maze->height - 1 || ny <= 0 || ny >= maze->width - 1) return false; // 边界外
    int count = 0;
    for (int d = 0; d < 4; d++) {
        int tx = nx + DX[d], ty = ny + DY[d];
        if (tx >= 0 && tx < maze->height && ty >= 0 && ty < maze->width && maze->maze[tx][ty] == 0) {
            count++;
        }
    }
    return count <= 1; // 保证挖出的路不会连接过多的已挖路径
}

// 深度优先搜索生成迷宫
void generateMazeDFS(int x, int y, Maze* maze) {
    // 随机打乱方向
    int order[] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    // 按随机方向尝试挖通路
    for (int i = 0; i < 4; i++) {
        int nx = x + DX[order[i]];
        int ny = y + DY[order[i]];
        if (isValid(nx, ny, maze)) {
            maze->maze[nx][ny] = 0; // 挖通路
            generateMazeDFS(nx, ny, maze); // 递归生成
        }
    }
}

// 初始化迷宫并随机生成通路
void generateRandomMaze(Maze* maze) {
    srand((unsigned int)time(NULL)); // 设置随机种子

    // 初始化迷宫为全墙壁
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->maze[i][j] = 1; // 设置为墙壁
        }
    }

    // 设置入口和出口
    maze->entranceX = 0;
    maze->entranceY = 0;
    maze->exitX = maze->height - 1;
    maze->exitY = maze->width - 1;
      
    
    

    // 从入口开始随机生成迷宫
    generateMazeDFS(maze->entranceX+1, maze->entranceY+1, maze);
    return;
}

// 随机生成宝藏
void generateTreasures(Maze* maze) {
    srand((unsigned int)time(NULL)); // 设置随机种子
    for (int i = 0; i < TREASURE_COUNT; i++) {
        int tx, ty;
        do {
            tx = rand() % maze->height;
            ty = rand() % maze->width;
        } while (maze->maze[tx][ty] == 1 || (tx == maze->entranceX && ty == maze->entranceY)); // 避免放置在墙壁或入口
        treasures[i] = { tx, ty, false }; // 初始化宝藏
    }
}

// 绘制宝藏
void drawTreasures(HDC hdc) {
    HBRUSH hBrush;
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (!treasures[i].collected) {
            RECT rect = { treasures[i].y * 30, treasures[i].x * 30, (treasures[i].y + 1) * 30, (treasures[i].x + 1) * 30 };
            hBrush = CreateSolidBrush(RGB(255, 215, 0)); // 金色表示宝藏
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
        }
    }
}


// 更新玩家分数
void updateScore(int x, int y) {
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (treasures[i].x == x && treasures[i].y == y && !treasures[i].collected) {
            treasures[i].collected = true; // 收集宝藏
            score++;  // 得分增加
            break;
        }
    }
}

// 绘制计分板
void drawScore(HDC hdc) {
    TCHAR scoreText[50];
    wsprintf(scoreText, _T("Score: %d"), score);
    TextOut(hdc, 10, 10, scoreText, lstrlen(scoreText));
}


bool changedirection(int& x, int& y,HDC hdc,stack<int>&rec,int dir)
{
    switch (dir)
    {
    case(3):
    {
        if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
            return true;
        }
        else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
            return true;
        }
        else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
            return true;
        }
        else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
            return true;
        }
        else
            return false;
    }
    case(0):
    {
        if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
            return true;
        }
        else if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
            return true;
        }
        else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
            return true;
        }
        else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
            return true;
        }
        else
            return false;
    }
    case(1):
    {
        if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
            return true;
        }
        else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
            return true;
        }
        else if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
            return true;
        }
        else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
            return true;
        }
        else
            return false;
    }
    case(2):
    {
        if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
            return true;
        }
        else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
            return true;
        }
        else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
            return true;
        }
        else if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
            return true;
        }
        else
            return false;
    }
    }
    
}


//寻找通路
void findpath(HWND hwnd, Maze maze, Path& path)
{

    stack<pair<int, int>> Stack;
    stack<int> rec;
    HBRUSH hBrush;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    


    int x = (maze.entranceX+1 ), y = (maze.entranceY+1);
    Stack.push({ x,y });
    rec.push(0);
    while ((x+1) != maze.exitX || (y +1) != maze.exitY)  
    {

        if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
        }
        else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
        }
        else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
        }
        else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
        }
        else
        {

            while (true)
            {
                Stack.pop();
                auto p = Stack.top();
                int xt = p.first, yt = p.second;
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                RECT rect = { xt * 30, yt * 30, (xt + 1) * 30, (yt + 1) * 30 };
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
                Sleep(1000);
                auto re = rec.top();
                maze.maze[xt][yt] = 1;
                rec.pop();
                if (maze.maze[xt + 1][yt] == 0 && re != 1 && rec.top() != 3)
                {
                    x = xt + 1;
                    y = yt;
                    rec.push(1);
                    break;
                }
                else if (maze.maze[xt][yt + 1] == 0 && re != 2 && rec.top() != 4)
                {
                    x = xt;
                    y = yt + 1;
                    rec.push(2);
                    break;
                }
                else if (maze.maze[xt - 1][yt] == 0 && re != 3 && rec.top() != 1)
                {
                    x = xt - 1;
                    y = yt;
                    rec.push(3);
                    break;
                }
                else if (maze.maze[xt][yt - 1] == 0 && re != 4 && rec.top() != 2)
                {
                    x = xt;
                    y = yt - 1;
                    rec.push(4);
                    break;
                }
                
            }
        }
        Stack.push({ x, y });
        RECT rect = { x * 30, y * 30, (x + 1) * 30, (y + 1) * 30 };
        
            hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 黄色表示路径
            FillRect(hdc, &rect, hBrush);
            Sleep(1000);
            DeleteObject(hBrush);
    }

    if ((x+1) == maze.exitX && y+1== maze.exitY)
    {
        while (!Stack.empty())
        {   
            auto tmp = Stack.top();
            path.path[tmp.first][tmp.second] = 1;

            Stack.pop();

        }
        return;  
    }
}

void drawpath(HDC hdc)
{
    stack<pair<int, int>> Stack;
    stack<int> rec;
    HBRUSH hBrush;
    int x = (maze.entranceX + 1), y = (maze.entranceY + 1);
    Stack.push({ x,y });
    RECT rect = { y * 30, x * 30, (y + 1) * 30, (x + 1) * 30 };

    hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 黄色表示路径
    FillRect(hdc, &rect, hBrush);
    Sleep(500);
    DeleteObject(hBrush);
    rec.push(0);
    while ((x + 1) != maze.exitX || (y + 1) != maze.exitY)
    {



        updateScore(x, y);
        drawScore(hdc);
        

        /*if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        {
            x = x + 1;
            rec.push(1);
        }
        else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        {
            y = y + 1;
            rec.push(2);
        }
        else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        {
            x = x - 1;
            rec.push(3);
        }
        else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        {
            y = y - 1;
            rec.push(4);
        }
        */
        if (!changedirection(x, y, hdc, rec, directionPriority))
        {
            while (true)
            {
                auto p = Stack.top();
                Stack.pop();

                int xt = p.first, yt = p.second;
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                RECT rect = { yt * 30, xt * 30, (yt + 1) * 30, (xt + 1) * 30 };
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
                Sleep(500);
                p = Stack.top();
                xt = p.first, yt = p.second;
                auto re = rec.top();
                maze.maze[xt][yt] = 1;
                rec.pop();
                if (maze.maze[xt + 1][yt] == 0 && re != 1 && rec.top() != 3)
                {
                    x = xt + 1;
                    y = yt;
                    rec.push(1);
                    break;
                }
                else if (maze.maze[xt][yt + 1] == 0 && re != 2 && rec.top() != 4)
                {
                    x = xt;
                    y = yt + 1;
                    rec.push(2);
                    break;
                }
                else if (maze.maze[xt - 1][yt] == 0 && re != 3 && rec.top() != 1)
                {
                    x = xt - 1;
                    y = yt;
                    rec.push(3);
                    break;
                }
                else if (maze.maze[xt][yt - 1] == 0 && re != 4 && rec.top() != 2)
                {
                    x = xt;
                    y = yt - 1;
                    rec.push(4);
                    break;
                }

            }
        }
        
        Stack.push({ x, y });
        RECT rect = { y * 30, x * 30, (y + 1) * 30, (x + 1) * 30 };

        hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 黄色表示路径
        FillRect(hdc, &rect, hBrush);
        Sleep(500);
        DeleteObject(hBrush);
    }
}
void initPath(Path& path)
{
    for (int i = 0; i < maze.height; i++)
    {
        for (int j = 0; j < maze.width; j++)
        {
            path.path[i][j] = 0;
        }
    }
    return;
}

// 从文件读取迷宫数据
bool loadMazeFromFile(const char* fileName, Maze* maze) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, fileName, "r");
    if (err != 0 || file == NULL) {
        MessageBox(NULL, _T("无法读取迷宫文件"), _T("错误"), MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // 读取迷宫的尺寸和入口出口
    fscanf_s(file, "%d %d", &maze->width, &maze->height);
    fscanf_s(file, "%d %d", &maze->entranceX, &maze->entranceY);
    fscanf_s(file, "%d %d", &maze->exitX, &maze->exitY);

    // 读取迷宫地图数据
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            fscanf_s(file, "%d", &maze->maze[y][x]);
        }
    }

    fclose(file);
    return true;
}

// 保存迷宫数据到文件
void saveMazeToFile(const char* fileName, Maze* maze) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, fileName, "w");
    if (err == 0 && file != NULL) {
        fprintf(file, "%d %d\n", maze->width, maze->height);
        fprintf(file, "%d %d\n", maze->entranceX, maze->entranceY);
        fprintf(file, "%d %d\n", maze->exitX, maze->exitY);

        // 保存迷宫地图数据
        for (int y = 0; y < maze->height; y++) {
            for (int x = 0; x < maze->width; x++) {
                fprintf(file, "%d ", maze->maze[y][x]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
}

// 窗口过程函数
LRESULT CALLBACK winproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        /* 加载迷宫数据，如果文件不存在则创建默认迷宫*/
        //if (!loadMazeFromFile("maze.txt", &maze)) {
        //    maze.width = 10;
        //    maze.height = 10;
        //    maze.entranceX = 0;
        //    maze.entranceY = 0;
        //    maze.exitX = 9;
        //    maze.exitY = 9;

        //    // 默认迷宫（0为路径，1为墙壁）
        //    for (int i = 0; i < maze.height; i++) {
        //        for (int j = 0; j < maze.width; j++) {
        //            maze.maze[i][j] = (i == 0 || i == maze.height - 1 || j == 0 || j == maze.width - 1) ? 1 : 0;
        //        }
        //    }
        //}
        maze.height = 10;
        maze.width = 10;   
        generateRandomMaze(&maze);
        maze.maze[1][1] = 0;
        maze.maze[maze.height - 2][maze.width - 2] = 0;
        generateTreasures(&maze);
        break;

    case WM_KEYDOWN:
    {
        if (wParam == VK_UP)
            directionPriority = 0;
        if (wParam == VK_LEFT)
            directionPriority = 1;
        if (wParam == VK_RIGHT)
            directionPriority = 2;
        if (wParam == VK_DOWN)
            directionPriority = 3;

            ;
    }
    case WM_PAINT:  
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        initPath(path);
        
       

        // 设置绘图颜色
        HBRUSH hBrush;
        for (int y = 0; y < maze.height; y++) {
            for (int x = 0; x < maze.width; x++) {
                if (maze.maze[y][x] == 1) {
                    // 墙壁
                    hBrush = CreateSolidBrush(RGB(0, 0, 0));  // 黑色
                }
                else {
                    // 通路
                    hBrush = CreateSolidBrush(RGB(255, 255, 255));  // 白色
                }

                // 绘制每个格子
                RECT rect = { x * 30, y * 30, (x + 1) * 30, (y + 1) * 30 };
                FillRect(hdc, &rect, hBrush);

                
                 /*绘制入口*/
                if (x-1 == maze.entranceX && y == maze.entranceY) {
                    hBrush = CreateSolidBrush(RGB(0, 255, 0));  // 绿色表示入口
                    FillRect(hdc, &rect, hBrush);
                }  

                // 绘制出口
                if ((x) == maze.exitX && y+1== maze.exitY) {
                    hBrush = CreateSolidBrush(RGB(255, 0, 0));  // 红色表示出口
                    FillRect(hdc, &rect, hBrush);
                }
                
                drawTreasures(hdc);  // 新增的绘制宝藏方法


                DeleteObject(hBrush);
               
            }
           
        }
        
        Sleep(1000);
        drawpath(hdc);
        //stack<pair<int, int>> Stack;   
        //stack<int> rec;
       
        //int x = (maze.entranceX + 1), y = (maze.entranceY + 1);
        //Stack.push({ x,y });
        //RECT rect = { y * 30, x * 30, (y + 1) * 30, (x + 1) * 30 };

        //hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 黄色表示路径
        //FillRect(hdc, &rect, hBrush);
        //Sleep(200);
        //DeleteObject(hBrush);
        //rec.push(0);
        //while ((x + 1) != maze.exitX || (y + 1) != maze.exitY)
        //{

        //    

        //    updateScore(x, y);
        //    drawScore(hdc);
        //    if (maze.maze[x + 1][y] == 0 && rec.top() != 3)
        //    {
        //        x = x + 1;
        //        rec.push(1);
        //    }
        //    else if (maze.maze[x][y + 1] == 0 && rec.top() != 4)
        //    {
        //        y = y + 1;
        //        rec.push(2);
        //    }
        //    else if (maze.maze[x - 1][y] == 0 && rec.top() != 1)
        //    {
        //        x = x - 1;
        //        rec.push(3);
        //    }
        //    else if (maze.maze[x][y - 1] == 0 && rec.top() != 2)
        //    {
        //        y = y - 1;
        //        rec.push(4);
        //    }
        //    else
        //    {

        //        while (true)
        //        {
        //            auto p = Stack.top();
        //            Stack.pop();
        //            
        //            int xt = p.first, yt = p.second;
        //            hBrush = CreateSolidBrush(RGB(255, 255, 255));
        //            RECT rect = { yt * 30, xt * 30, (yt + 1) * 30, (xt + 1) * 30 };
        //            FillRect(hdc, &rect, hBrush);
        //            DeleteObject(hBrush);
        //            Sleep(200);
        //            p = Stack.top();
        //            xt = p.first, yt = p.second;
        //            auto re = rec.top();
        //            maze.maze[xt][yt] = 1;
        //            rec.pop();
        //            if (maze.maze[xt + 1][yt] == 0 && re != 1 && rec.top() != 3)
        //            {
        //                x = xt + 1;
        //                y = yt;
        //                rec.push(1);
        //                break;
        //            }
        //            else if (maze.maze[xt][yt + 1] == 0 && re != 2 && rec.top() != 4)
        //            {
        //                x = xt;
        //                y = yt + 1;
        //                rec.push(2);
        //                break;
        //            }
        //            else if (maze.maze[xt - 1][yt] == 0 && re != 3 && rec.top() != 1)
        //            {
        //                x = xt - 1;
        //                y = yt;
        //                rec.push(3);
        //                break;
        //            }
        //            else if (maze.maze[xt][yt - 1] == 0 && re != 4 && rec.top() != 2)
        //            {
        //                x = xt;
        //                y = yt - 1;
        //                rec.push(4);
        //                break;
        //            }

        //        }
        //    }
        //    Stack.push({ x, y });
        //    RECT rect = { y * 30, x * 30, (y + 1) * 30, (x + 1) * 30 };

        //    hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 黄色表示路径
        //    FillRect(hdc, &rect, hBrush);
        //    Sleep(200);
        //    DeleteObject(hBrush);
        //}

        
        
        
        
       
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        saveMazeToFile("maze.txt", &maze);  // 保存迷宫到文件
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// 程序入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = winproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, _T("Window Class Registration Failed!"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;   
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
