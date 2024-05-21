
#pragma region Include
#include <iostream>
#include <fstream>
#include <ctime>
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

float WINDOW_WIDTH = 1000.0f;
float WINDOW_HEIGHT = 800.0f;
bool white = false;

vector<vector<Plate>> plates;
int brokenA = 0;
int platesA = 0;
Paddle paddle;
vector<Ball> balls;

void RenderString(float x, float y, void* font, const char* string)
{
    auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(string));
    char* c;
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    glutBitmapString(font, uchrs);
}

void showUpdatedObjects() {
    // show paddle
    float posX = paddle.getPosition().x;
    float posY = paddle.getPosition().y;
    float width = paddle.getWidth();
    float height = paddle.getHeight();
    glColor3ub(paddle.getColor()[0], paddle.getColor()[1], paddle.getColor()[2]);
    glBegin(GL_QUADS);
    glVertex2d(posX, posY);
    glVertex2d(posX+width, posY);
    glVertex2d(posX+width, posY + height);
    glVertex2d(posX, posY +height);
    glEnd();

    //show plates
    for (int i = 0; i < plates.size(); ++i) {
        for (int j = 0; j < plates[0].size(); ++j) {
            if (plates[i][j].getLifes() > 0) {
                float posX = plates[i][j].getPosition().x;
                float posY = plates[i][j].getPosition().y;
                float width = plates[i][j].getWidth();
                float height = plates[i][j].getHeight();
                glColor3ub(plates[i][j].getColor()[0], plates[i][j].getColor()[1], plates[i][j].getColor()[2]);
                glBegin(GL_QUADS);
                glVertex2d(posX, posY);
                glVertex2d(posX + width, posY);
                glVertex2d(posX + width, posY + height);
                glVertex2d(posX, posY + height);
                glEnd();
                /*char buffer[33];
                _itoa_s(i, buffer, plates[i][j].getLifes());
                RenderString(posX,posY,GLUT_BITMAP_HELVETICA_18, buffer);*/
            }
        }
    }
    // show ball
    for (int i = 0; i < balls.size(); ++i) {
        glColor3ub(balls[i].getColor()[0], balls[i].getColor()[1], balls[i].getColor()[2]);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < 80; ii++)
        {
            float theta = 2.0f * 3.1415926f * float(ii) / float(80);

            float x = balls[i].getRadius() * cosf(theta);
            float y = balls[i].getRadius() * sinf(theta);
            float cx = balls[i].getPosition().x;
            float cy = balls[i].getPosition().y;
            glVertex2f(x + cx, y + cy);

        }
        glEnd();

        glPointSize(15.0);
        glBegin(GL_POINTS);
        glColor3ub(255, 0, 0);
        glVertex2f(balls[i].getPosition().x - balls[i].getSpeed().x * 5, balls[i].getPosition().y - balls[i].getSpeed().y * 5);
        glEnd();
        
        glPointSize(10.0); 
        glBegin(GL_POINTS);
        glColor3ub(255, 0, 0);
        glVertex2f(balls[i].getPosition().x - balls[i].getSpeed().x * 7, balls[i].getPosition().y - balls[i].getSpeed().y * 7);
        glEnd();

        glPointSize(5.0); 
        glBegin(GL_POINTS);
        glColor3ub(255, 0, 0);
        glVertex2f(balls[i].getPosition().x - balls[i].getSpeed().x * 9, balls[i].getPosition().y - balls[i].getSpeed().y * 9);
        glEnd();
    }

}

void AddBall() {
    if (balls[0].getPosition().y > 200) {
        Ball ball = Ball(balls[0].getPosition().x, balls[0].getPosition().y, 10.0f, 6.0f);
        ball.setColor(200, 255, 255);
        ball.setSpeed(Point(-balls[0].getSpeed().x, balls[0].getSpeed().y));
        balls.push_back(ball);
    }
}
bool checkBallIntersection(Ball &ball) {
    float paddlePosY = paddle.getPosition().y;
    float paddlePosX = paddle.getPosition().x;
    float paddleWidth = paddle.getWidth();
    float paddleHeight = paddle.getHeight();
    float TOP_LINE_Y = WINDOW_HEIGHT;
    float BOTTOM_LINE_X = 0.0f;
    float RIGHT_LINE_X = WINDOW_WIDTH;
    float LEFT_LINE_X = 0.0f;
    float ballPosX = ball.getPosition().x;
    float ballPosY = ball.getPosition().y;
    float ballRadius = ball.getRadius();
    Point paddleRightCorner = Point(paddlePosX + paddleWidth, paddlePosY + paddleHeight);
    Point paddleLeftCorner = Point(paddlePosX, paddlePosY + paddleHeight);
    // ПРОВЕРКА КРАЕВ
    if (ballPosY + ballRadius > TOP_LINE_Y) {
        ball.setSpeed(Point(ball.getSpeed().x, -ball.getSpeed().y));
    }
    else if (ballPosX + ballRadius > RIGHT_LINE_X) {
        ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
    }
    else if (ballPosX - ballRadius < LEFT_LINE_X) {
        ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
    }
    else if (ballPosY - ballRadius < BOTTOM_LINE_X) {
        for (int h = 0; h < balls.size(); ++h) {
            if (balls[h].getPosition().x == ball.getPosition().x && balls[h].getPosition().y == ball.getPosition().y) {
                balls.erase(std::next(balls.begin(), h));
            }
        }
        return true;
    }
    // ПРОВЕРКА ПЛАШКИ ИГРОКА

    // проверка столкновения шара с верхом плашки
    else if (ballPosY - ballRadius < paddlePosY + paddleHeight && ballPosX>paddlePosX && ballPosX < paddlePosX + paddleWidth) {
        float ballTouchX = ballPosX - paddlePosX- paddleWidth / 2;
        float speedX = ballTouchX / paddleWidth;
        ball.setSpeed(Point(speedX * ball.getVelocity(), -ball.getSpeed().y));
    }
    // проверка столкновения шара с правой стенкой плашки
    else if (ballPosX - ballRadius < paddleRightCorner.x && ballPosY < paddleRightCorner.y && ballPosX - ballRadius > paddleLeftCorner.x) {
        if(ball.getSpeed().x<0 && ball.getSpeed().y < 0) ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
    }
    // проверка столкновения шара с левой стенкой плашки
    else if (ballPosX + ballRadius > paddleLeftCorner.x && ballPosY < paddleLeftCorner.y && ballPosX + ballRadius < paddleRightCorner.x) {
        if (ball.getSpeed().x > 0 && ball.getSpeed().y < 0) ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
    }
    // проверка столкновения шара с правым углом плашки
    else if (abs(paddleRightCorner.x-ballPosX)<ballRadius && abs(paddleRightCorner.y - ballPosY) < ballRadius) {
        ball.setSpeed(Point(abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
        ball.setPosition(ball.getPosition().x + abs(paddleRightCorner.x - ballPosX)+5.0f, ball.getPosition().y + abs(paddleRightCorner.y - ballPosY) + 5.0f);
    }
    // проверка столкновения шара с левым углом плашки
    else if (abs(paddleLeftCorner.x - ballPosX) < ballRadius && abs(paddleLeftCorner.y - ballPosY) < ballRadius) {
        ball.setSpeed(Point(-abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
        ball.setPosition(ball.getPosition().x - (abs(paddleLeftCorner.x - ballPosX) + 5.0f), ball.getPosition().y + abs(paddleLeftCorner.y - ballPosY) + 5.0f);
    }
    // ПРОВЕРКА ПЛИТКИ
    for (int i = plates.size()-1; i >-1;--i) {
        for (int j = plates[0].size() - 1; j > -1; --j) {
            if (plates[i][j].getLifes() < 1) continue;
            Point plateTopRightCorner = Point(plates[i][j].getPosition().x + plates[i][j].getWidth(), plates[i][j].getPosition().y + plates[i][j].getHeight());
            Point plateTopLeftCorner = Point(plates[i][j].getPosition().x, plates[i][j].getPosition().y + plates[i][j].getHeight());
            Point plateBottomRightCorner = Point(plates[i][j].getPosition().x, plates[i][j].getPosition().y + plates[i][j].getHeight());
            Point plateBottomLeftCorner = Point(plates[i][j].getPosition().x, plates[i][j].getPosition().y);
            // c низом
            if (ballPosY + ballRadius < plates[i][j].getPosition().y + plates[i][j].getHeight() && ballPosY + ballRadius>plates[i][j].getPosition().y && ballPosX > plates[i][j].getPosition().x && ballPosX < plates[i][j].getPosition().x + plates[i][j].getWidth()) {
                ball.setSpeed(Point(ball.getSpeed().x, -ball.getSpeed().y));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // с верхом
            else if (ballPosY - ballRadius > plates[i][j].getPosition().y && ballPosY - ballRadius < plates[i][j].getPosition().y + plates[i][j].getHeight() && ballPosX > plates[i][j].getPosition().x && ballPosX < plates[i][j].getPosition().x + plates[i][j].getWidth()) {
                ball.setSpeed(Point(ball.getSpeed().x, -ball.getSpeed().y));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // справа
            else if (ballPosX - ballRadius > plates[i][j].getPosition().x && ballPosX - ballRadius < plates[i][j].getPosition().x + plates[i][j].getWidth() && ballPosY<plates[i][j].getPosition().y + plates[i][j].getHeight() && ballPosY>plates[i][j].getPosition().y) {
                ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // слева
            else if (ballPosX + ballRadius < plates[i][j].getPosition().x + plates[i][j].getWidth() && ballPosX + ballRadius > plates[i][j].getPosition().x && ballPosY<plates[i][j].getPosition().y + plates[i][j].getHeight() && ballPosY>plates[i][j].getPosition().y) {
                ball.setSpeed(Point(-ball.getSpeed().x, ball.getSpeed().y));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // BottomRightCorner
            else if (abs(plateBottomRightCorner.x - ballPosX) < ballRadius && abs(plateBottomRightCorner.y - ballPosY) < ballRadius) {
                ball.setSpeed(Point(-abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
                ball.setPosition(ball.getPosition().x + (abs(plateBottomRightCorner.x - ballPosX) + 5.0f), ball.getPosition().y - (abs(plateBottomRightCorner.y - ballPosY) + 5.0f));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // BottomLeftCorner
            else if (abs(plateBottomLeftCorner.x - ballPosX) < ballRadius && abs(plateBottomLeftCorner.y - ballPosY) < ballRadius) {
                ball.setSpeed(Point(-abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
                ball.setPosition(ball.getPosition().x - (abs(plateBottomLeftCorner.x - ballPosX) + 5.0f), ball.getPosition().y - (abs(plateBottomLeftCorner.y - ballPosY) + 5.0f));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // TopLeftCorner
            else if (abs(plateTopLeftCorner.x - ballPosX) < ballRadius && abs(plateTopLeftCorner.y - ballPosY) < ballRadius) {
                ball.setSpeed(Point(-abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
                ball.setPosition(ball.getPosition().x - (abs(plateTopLeftCorner.x - ballPosX) + 5.0f), ball.getPosition().y + abs(plateTopLeftCorner.y - ballPosY) + 5.0f);
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
            // TopRightCorner
            else if (abs(plateTopRightCorner.x - ballPosX) < ballRadius && abs(plateTopRightCorner.y - ballPosY) < ballRadius) {
                ball.setSpeed(Point(-abs(ball.getSpeed().x), abs(ball.getSpeed().y)));
                ball.setPosition(ball.getPosition().x + (abs(plateTopRightCorner.x - ballPosX) + 5.0f), ball.getPosition().y - (abs(plateTopRightCorner.y - ballPosY) + 5.0f));
                plates[i][j].hit(ball.getPower());
                brokenA++;
            }
        }
    }
    return false;
}

void victory() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderString(WINDOW_WIDTH/2, WINDOW_HEIGHT / 2, GLUT_BITMAP_TIMES_ROMAN_24, "PRETTY BOY!");
}
void gameOver() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width = rand() % (1000 - 0 + 1) + 0;
    int height = rand() % (800 - 0 + 1) + 0;
    RenderString(width, height, GLUT_BITMAP_HELVETICA_18, "LOSER!");
}

void frame() {
    vector<int> TrashArray;
    for (int i = 0; i < balls.size(); ++i) {
        balls[i].setPosition(balls[i].getPosition().x + balls[i].getSpeed().x, balls[i].getPosition().y + balls[i].getSpeed().y);
        if (checkBallIntersection(balls[i])) {
            TrashArray.push_back(i);
        }
    }
    /*for(auto x: TrashArray) {
        balls.erase(std::next(balls.begin(), x));
    }*/
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    frame();
    showUpdatedObjects();
    if (balls.size() == 0) {
        if (white) {
            glClearColor(1, 1, 1, 0.05f);
            white = false;
        }
        else if (!white) {
            glClearColor(1, 0.1f, 0.1f, 0.05f);
            white = true;
        }
            gameOver();
    }
    if (brokenA == platesA) {
        glClearColor(0, 0.5f, 0, 0.05f);
        victory();
    }
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
        glutTimerFunc(10, timerFunc, REDISPLAYTIMERID);
        break;
    }
}
void mouseClick(int button, int state, int  x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && balls[balls.size() - 1].getSpeed().x == 0 && balls[balls.size() - 1].getSpeed().y==0) {
        for (int i = 0; i < balls.size(); ++i) {
            int start = 0;
            int end = 12;
            int r = rand() % (end - start + 1) + start;
            balls[i].setSpeed(Point(r-6, 6.0f));
        }
    }
}
void mouseCB(int  x, int y)
{
    float wid = paddle.getWidth() / 2;
    if ( x<WINDOW_WIDTH - wid && x>wid) {
        paddle.setPosition(x - wid, paddle.getPosition().y);
    }
    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i].getSpeed().x == 0.0f && balls[i].getSpeed().y == 0.0f) {
            balls[i].setPosition(paddle.getPosition().x + paddle.getWidth() / 2, paddle.getPosition().y + paddle.getHeight() + balls[i].getRadius());
        }
    }
}
void KeyboardDown(unsigned char key, int x, int y) {
    if (key == 'n') {
        AddBall();
    }
}
void initBaseField() {
    
    paddle = Paddle(10.0f, 150.0f, 25.0f);
    paddle.setPosition(350.0f, 20.0f);
    paddle.setColor(255, 255, 255);
    Ball ball = Ball(paddle.getPosition().x+paddle.getWidth()/2, paddle.getPosition().y + paddle.getHeight() +10.0f,10.0f,6.0f);
    ball.setColor(200, 255, 255);
    balls.push_back(ball);
    std::ifstream in("fieldVlad.txt");
    int rows, columns;
    in >> rows >> columns;
    plates.resize(columns);
    for (int i = 0; i < columns;++i) {
        plates[i].resize(rows);
    }
    for (int j = rows-1; j > -1; --j) {
        for (int i = 0; i < columns; ++i) {
            int is;
            in >> is;
            if (is > 0) {
                platesA++;
            }
            float d =  (250 / columns);
            plates[i][j] = Plate((WINDOW_WIDTH/columns)*i, (WINDOW_HEIGHT-250) + d * j,  d, WINDOW_WIDTH / columns ,is); 
            int start = 15;
            int end = 255;
            int r = rand() % (end - start + 1) + start;
            int g = rand() % (end - start + 1) + start;
            int b = rand() % (end - start + 1) + start;
            plates[i][j].setColor(r, g, b);
        }
    }
}

int main(int argc, char** argv)
{
    srand(time(0));
    glutInit(&argc, argv);                   // начальная инициализация окна
    glutInitDisplayMode(GLUT_DOUBLE);        // установка режима отображения
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);        // начальная позиция на экране
    glutCreateWindow("BreakOut");            // заголовок окна
    glClearColor(0.1f, 0.1f, 0.1f, 0.05f);               // цвет фона
    glMatrixMode(GL_PROJECTION);             // режим матрицы
    glLoadIdentity();                        // отчищает матрицу
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    //gluOrtho2D(WINDOW_WIDTH,0.0, WINDOW_HEIGHT,0.0);
    glutDisplayFunc(display);
    glutTimerFunc(100, timerFunc, REDISPLAYTIMERID); 
    glutPassiveMotionFunc(mouseCB);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(KeyboardDown);
    initBaseField();
    glutMainLoop();
    return 0;
}
