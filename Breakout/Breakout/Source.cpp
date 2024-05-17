
#pragma region Include
#include <iostream>
#include "freeglut.h"
#include "string"
#include "vector"
#include "Point.h"
#include "Paddle.h"
#include "Plate.h"
#include "Ball.h"
#pragma endregion
#define REDISPLAYTIMERID 1
using namespace std;

float WINDOW_HEIGHT = 1000.0f;
float WINDOW_WIDTH = 800.0f;

vector<Plate> plates;
Paddle paddle;
vector<Ball> balls;

void displayStartPosition() {
    float posX = paddle.getPosition().x;
    float posY = paddle.getPosition().y;
    float width = paddle.getWidth();
    float height = paddle.getHeight();
    glColor3f(paddle.getColor()[0], paddle.getColor()[1], paddle.getColor()[2]);//рисуем синий треугольник
    glBegin(GL_QUADS);
    glVertex2d(posX, posY);
    glVertex2d(posX+width, posY);
    glVertex2d(posX+width, posY + height);
    glVertex2d(posX, posY +height);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    displayStartPosition();
    
    glutSwapBuffers();
}

int g_c = 0;
void timerFunc(int nTimerID)
{
    switch (nTimerID)
    {
    case REDISPLAYTIMERID:
        g_c++;
        glutPostRedisplay();
        glutTimerFunc(100, timerFunc, REDISPLAYTIMERID);
        break;
    }
}
void mouseCB(int  x, int y)
{
    float wid = paddle.getWidth()/2;
    paddle.setPosition(x-wid, 10.0f);
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);                   // начальная инициализация окна
    glutInitDisplayMode(GLUT_DOUBLE);        // установка режима отображения
    glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
    glutInitWindowPosition(0, 0);        // начальная позиция на экране
    glutCreateWindow("BreakOut");            // заголовок окна
    glClearColor(0.1f, 0.1f, 0.1f, 0.05f);               // цвет фона

    glMatrixMode(GL_PROJECTION);             // режим матрицы
    glLoadIdentity();                        // отчищает матрицу
    gluOrtho2D(0, WINDOW_HEIGHT, 0, WINDOW_WIDTH);
    glutDisplayFunc(display);
    glutTimerFunc(100, timerFunc, REDISPLAYTIMERID); 
    glutPassiveMotionFunc(mouseCB);
    paddle = Paddle(10.0f, 150.0f, 25.0f);
    paddle.setPosition(350.0f, 10.0f);
    paddle.setColor(255, 255, 255);
    glutMainLoop();
    return 0;
}
