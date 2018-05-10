#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<GL/glut.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int sirina=400;
int visina=400;
int trenutnaBoja=0;
int misX = 0, misY = 0;

typedef struct{
    pair<int,int> vrhovi[3];
    int boja;
}Trokut;

vector<Trokut> trokuti;
vector<pair<int,int> > novi;

void reshape(int width,int height);
void display();
void renderScene();
void kvadratic();
void postaviBoju(int boja);
void mousePressed(int, int, int, int);
void mouseMoved(int, int);
void keyPressed(unsigned char, int, int);
void nacrtajTrokute();
void nacrtajNovi();
int main(int argc,char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(sirina,visina);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Trokuti");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mousePressed);
    glutPassiveMotionFunc(mouseMoved);

    glutMainLoop();
}

void display()
{
    glClearColor(1.0f, 1.0f ,1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    renderScene();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glDisable(GL_DEPTH_TEST);
    glViewport(0,0, (GLsizei)width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width-1,height-1,0,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    kvadratic();
    nacrtajTrokute();
    nacrtajNovi();
}

void nacrtajTrokute(){
    for(int i = 0; i < trokuti.size(); i++) {
        postaviBoju(trokuti[i].boja);
        glBegin(GL_TRIANGLES);
        for(int j = 0 ; j < 3; ++j) {
            glVertex2i(trokuti[i].vrhovi[j].first, trokuti[i].vrhovi[j].second);
        }
        glEnd();
    }
}

void nacrtajNovi(){
    postaviBoju(trenutnaBoja);
    if(novi.size() == 1) {
        glBegin(GL_LINES);
        glVertex2i(novi[0].first, novi[0].second);
        glVertex2i(misX, misY);
        glEnd();
    } else if(novi.size() == 2) {
        glBegin(GL_TRIANGLES);
        glVertex2i(novi[0].first, novi[0].second);
        glVertex2i(novi[1].first, novi[1].second);
        glVertex2i(misX, misY);
        glEnd();
    }

}
void mousePressed(int button, int state, int x, int y) {
    if(state == GLUT_DOWN) {
        novi.push_back(make_pair(x, y));
        if(novi.size() == 3) {
            Trokut trokut;
            trokut.boja = trenutnaBoja;
            for(int i = 0; i < 3; ++i) {
                trokut.vrhovi[i] = novi[i];
            }
            trokuti.push_back(trokut);
            novi.clear();
        }
        glutPostRedisplay();
    }
}

void keyPressed(unsigned char key, int x, int y) {
    if(key == 'n') {
        trenutnaBoja=(trenutnaBoja+1)%6;
    } else if(key == 'p') {
        --trenutnaBoja;
        if(trenutnaBoja == -1) trenutnaBoja = 5;
    }
    glutPostRedisplay();
}

void mouseMoved(int x, int y) {
    misX = x;
    misY = y;
    glutPostRedisplay();
}

void kvadratic()
{
    postaviBoju(trenutnaBoja);
    glBegin(GL_QUADS);
    glVertex2i(sirina - 7, 0);
    glVertex2i(sirina - 2, 0);
    glVertex2i(sirina - 2, 5);
    glVertex2i(sirina - 7, 5);
    glEnd();
}
void postaviBoju(int boja)
{
    if(boja==0)
    {
        glColor3f(1.0f, 0.0f, 0.0f); //crvena
    }
    else if(boja==1)
    {
        glColor3f(0.0f, 1.0f, 0.0f); //zelena
    }
    else if(boja==2)
    {
        glColor3f(0.0f, 0.0f, 1.0f); //plava
    }
    else if(boja==3)
    {
        glColor3f(0.0f, 1.0f, 1.0f); //cijan
    }
    else if(boja==4)
    {
        glColor3f(1.0f, 1.0f, 0.0f); //zuta

    }
    else
    {
        glColor3f(1.0f, 0.0f, 1.0f); //magenta
    }
}
