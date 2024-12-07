#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<tchar.h>
#include<stdio.h>
#include<stack>
#include <stdlib.h> // ���� srand, rand
#include <time.h>   
#define MAX_SIZE 100  // ����Թ��ߴ�
using namespace std;
// �����Թ��ṹ
typedef struct {
    int width;         // �Թ����
    int height;        // �Թ��߶�
    int entranceX;     // ���X����
    int entranceY;     // ���Y����
    int exitX;         // ����X����
    int exitY;         // ����Y����
    int maze[MAX_SIZE][MAX_SIZE];  // �Թ�����
} Maze;

typedef struct {
    int path[MAX_SIZE][MAX_SIZE];
}Path;

Maze maze;
Path path;
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Matrix application");




const int DX[] = { 0, 0, 1, -1 };
const int DY[] = { 1, -1, 0, 0 };

// ����Ƿ������ (nx, ny) ��ͨ·
bool isValid(int nx, int ny, Maze* maze) {
    if (nx <= 0 || nx >= maze->height - 1 || ny <= 0 || ny >= maze->width - 1) return false; // �߽���
    int count = 0;
    for (int d = 0; d < 4; d++) {
        int tx = nx + DX[d], ty = ny + DY[d];
        if (tx >= 0 && tx < maze->height && ty >= 0 && ty < maze->width && maze->maze[tx][ty] == 0) {
            count++;
        }
    }
    return count <= 1; // ��֤�ڳ���·�������ӹ��������·��
}

// ����������������Թ�
void generateMazeDFS(int x, int y, Maze* maze) {
    // ������ҷ���
    int order[] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    // �������������ͨ·
    for (int i = 0; i < 4; i++) {
        int nx = x + DX[order[i]];
        int ny = y + DY[order[i]];
        if (isValid(nx, ny, maze)) {
            maze->maze[nx][ny] = 0; // ��ͨ·
            generateMazeDFS(nx, ny, maze); // �ݹ�����
        }
    }
}

// ��ʼ���Թ����������ͨ·
void generateRandomMaze(Maze* maze) {
    srand((unsigned int)time(NULL)); // �����������

    // ��ʼ���Թ�Ϊȫǽ��
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->maze[i][j] = 1; // ����Ϊǽ��
        }
    }

    // ������ںͳ���
    maze->entranceX = 0;
    maze->entranceY = 0;
    maze->exitX = maze->height - 1;
    maze->exitY = maze->width - 1;
      
    
    

    // ����ڿ�ʼ��������Թ�
    generateMazeDFS(maze->entranceX+1, maze->entranceY+1, maze);
    return;
}





//Ѱ��ͨ·
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
        
            hBrush = CreateSolidBrush(RGB(255, 255, 0));  // ��ɫ��ʾ·��
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

// ���ļ���ȡ�Թ�����
bool loadMazeFromFile(const char* fileName, Maze* maze) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, fileName, "r");
    if (err != 0 || file == NULL) {
        MessageBox(NULL, _T("�޷���ȡ�Թ��ļ�"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // ��ȡ�Թ��ĳߴ����ڳ���
    fscanf_s(file, "%d %d", &maze->width, &maze->height);
    fscanf_s(file, "%d %d", &maze->entranceX, &maze->entranceY);
    fscanf_s(file, "%d %d", &maze->exitX, &maze->exitY);

    // ��ȡ�Թ���ͼ����
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            fscanf_s(file, "%d", &maze->maze[y][x]);
        }
    }

    fclose(file);
    return true;
}

// �����Թ����ݵ��ļ�
void saveMazeToFile(const char* fileName, Maze* maze) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, fileName, "w");
    if (err == 0 && file != NULL) {
        fprintf(file, "%d %d\n", maze->width, maze->height);
        fprintf(file, "%d %d\n", maze->entranceX, maze->entranceY);
        fprintf(file, "%d %d\n", maze->exitX, maze->exitY);

        // �����Թ���ͼ����
        for (int y = 0; y < maze->height; y++) {
            for (int x = 0; x < maze->width; x++) {
                fprintf(file, "%d ", maze->maze[y][x]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
}

// ���ڹ��̺���
LRESULT CALLBACK winproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        /* �����Թ����ݣ�����ļ��������򴴽�Ĭ���Թ�*/
        //if (!loadMazeFromFile("maze.txt", &maze)) {
        //    maze.width = 10;
        //    maze.height = 10;
        //    maze.entranceX = 0;
        //    maze.entranceY = 0;
        //    maze.exitX = 9;
        //    maze.exitY = 9;

        //    // Ĭ���Թ���0Ϊ·����1Ϊǽ�ڣ�
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
        break;

    case WM_PAINT:  
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        initPath(path);
       
        // ���û�ͼ��ɫ
        HBRUSH hBrush;
        for (int y = 0; y < maze.height; y++) {
            for (int x = 0; x < maze.width; x++) {
                if (maze.maze[y][x] == 1) {
                    // ǽ��
                    hBrush = CreateSolidBrush(RGB(0, 0, 0));  // ��ɫ
                }
                else {
                    // ͨ·
                    hBrush = CreateSolidBrush(RGB(255, 255, 255));  // ��ɫ
                }

                // ����ÿ������
                RECT rect = { x * 30, y * 30, (x + 1) * 30, (y + 1) * 30 };
                FillRect(hdc, &rect, hBrush);

                
                 /*�������*/
                if (x-1 == maze.entranceX && y == maze.entranceY) {
                    hBrush = CreateSolidBrush(RGB(0, 255, 0));  // ��ɫ��ʾ���
                    FillRect(hdc, &rect, hBrush);
                }  

                // ���Ƴ���
                if ((x) == maze.exitX && y+1== maze.exitY) {
                    hBrush = CreateSolidBrush(RGB(255, 0, 0));  // ��ɫ��ʾ����
                    FillRect(hdc, &rect, hBrush);
                }
                



                DeleteObject(hBrush);
               
            }
           
        }
        Sleep(1000);
        stack<pair<int, int>> Stack;   
        stack<int> rec;
       
        int x = (maze.entranceX + 1), y = (maze.entranceY + 1);
        Stack.push({ x,y });
        RECT rect = { y * 30, x * 30, (y + 1) * 30, (x + 1) * 30 };

        hBrush = CreateSolidBrush(RGB(255, 255, 0));  // ��ɫ��ʾ·��
        FillRect(hdc, &rect, hBrush);
        Sleep(200);
        DeleteObject(hBrush);
        rec.push(0);
        while ((x + 1) != maze.exitX || (y + 1) != maze.exitY)
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
                    auto p = Stack.top();
                    Stack.pop();
                    
                    int xt = p.first, yt = p.second;
                    hBrush = CreateSolidBrush(RGB(255, 255, 255));
                    RECT rect = { yt * 30, xt * 30, (yt + 1) * 30, (xt + 1) * 30 };
                    FillRect(hdc, &rect, hBrush);
                    DeleteObject(hBrush);
                    Sleep(200);
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

            hBrush = CreateSolidBrush(RGB(255, 255, 0));  // ��ɫ��ʾ·��
            FillRect(hdc, &rect, hBrush);
            Sleep(200);
            DeleteObject(hBrush);
        }

        /*if ((x + 1) == maze.exitX && y + 1 == maze.exitY)
        {
            while (!Stack.empty())
            {
                auto tmp = Stack.top();
                path.path[tmp.first][tmp.second] = 1;

                Stack.pop();

            }
           
        }*/
        
        
        
       
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        saveMazeToFile("maze.txt", &maze);  // �����Թ����ļ�
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// ������ں���
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
