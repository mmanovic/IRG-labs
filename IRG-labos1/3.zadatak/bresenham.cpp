#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<GL/glut.h>
#include<algorithm>
#include<vector>
#include<math.h>
using namespace std;

int sirina=400,visina=400;
int xmin=100,xmax=300,ymin=100,ymax=300;
int misX=0, misY=0;
bool kontrola=false,odsijecanje=false;

typedef struct
{
    pair<int,int> poc;
    pair<int,int> kraj;
} Linija;
vector<Linija> linije;
bool unosPrve=true;
pair<int, int> prva,druga;
void bresenham_nacrtaj_cjelobrojni(int,int,int,int);
void keyPressed(unsigned char, int, int);
void mouseMoved(int, int);
void mouseClick(int,int,int,int);
void nacrtajLinije();
void reshape(int width,int height);
void display();
void renderScene();
void crtajPravokutnik();
void cothen_sutherland(int,int,int,int);

int main(int argc,char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(sirina,visina);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Zadatak 3.");
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMoved);
    glutKeyboardFunc(keyPressed);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
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
    sirina=width;
    visina=height;
    ymin=1*height/4;
    ymax=3*height/4;
    xmin=1*width/4;
    xmax=3*width/4;
    glDisable(GL_DEPTH_TEST);
    glViewport(0,0, (GLsizei)width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width-1,height-1,0,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    if(odsijecanje)
    {
        crtajPravokutnik();
    }
    nacrtajLinije();
}
void crtajPravokutnik()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glEnd();
}

void nacrtajLinije()
{
    int limit=linije.size();
    glColor3f(0.0f,0.0f,0.0f);
    for(int i=0; i<limit; i++)
    {
        if(odsijecanje)
        {
            cothen_sutherland(linije[i].poc.first,linije[i].poc.second, linije[i].kraj.first, linije[i].kraj.second);
        }
        else
        {
            glBegin(GL_POINTS);
            bresenham_nacrtaj_cjelobrojni(linije[i].poc.first,linije[i].poc.second, linije[i].kraj.first, linije[i].kraj.second);
            glEnd();
        }

        if(kontrola)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
            int x1 = linije[i].poc.first;
            int x2 = linije[i].kraj.first;
            int y1 = linije[i].poc.second;
            int y2 = linije[i].kraj.second;

            int a = -(y2-y1);
            int b = x2-x1;

            double dist =sqrt(a*a+b*b);
            double d_x =(a/dist)*4;
            double d_y =(b/dist)*4;

            glVertex2i(x1+(int)d_x, y1+(int)d_y);
            glVertex2i(x2+(int)d_x, y2+(int)d_y);

            glEnd();
            glColor3f(0.0f,0.0f,0.0f);
        }
    }
}


void mouseClick(int button, int status, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN)
    {
        if(unosPrve)
        {
            prva.first=x;
            prva.second=y;
            unosPrve=false;
        }
        else
        {
            druga.first = x;
            druga.second = y;

            Linija linija;
            linija.poc=prva;
            linija.kraj=druga;
            linije.push_back(linija);
            unosPrve=true;
        }
        glutPostRedisplay();
    }
}

void mouseMoved(int x, int y)
{
    misX = x;
    misY = y;
    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
    if(key == 'o')
    {
        odsijecanje = !odsijecanje;
    }
    else if(key == 'k')
    {
        kontrola = !kontrola;
    }
    glutPostRedisplay();
}

// kutevi od 0 do 90
void bresenham_nacrtaj_cjelobrojni2(int xs, int ys, int xe, int ye)
{
    int x, yc, korekcija;
    int a, yf;

    if (ye - ys <= xe - xs)
    {
        a = 2*(ye - ys);
        yc = ys;
        yf = -(xe - xs);
        korekcija = -2*(xe - xs);
        for (x = xs; x <= xe; ++x)
        {
            glVertex2i(x, yc);
            yf = yf + a;
            if (yf >= 0)
            {
                yf = yf + korekcija;
                yc = yc + 1;
            }
        }
    }
    else
    {
        x = xe;
        xe = ye;
        ye = x;
        x = xs;
        xs = ys;
        ys = x;
        a = 2*(ye - ys);
        yc = ys;
        yf = -(xe - xs);
        korekcija = -2*(xe - xs);
        for (x = xs; x <= xe; ++x)
        {
            glVertex2i(yc, x);
            yf = yf + a;
            if (yf >= 0)
            {
                yf = yf + korekcija;
                yc = yc + 1;
            }
        }
    }
}


// kutevi od 0 do -90
void bresenham_nacrtaj_cjelobrojni3(int xs, int ys, int xe, int ye)
{
    int x, yc, korekcija;
    int a, yf;
    if (-(ye-ys) <= xe - xs)
    {
        a = 2*(ye - ys);
        yc = ys;
        yf = (xe - xs);
        korekcija = 2*(xe - xs);
        for (x = xs; x <= xe; ++x)
        {
            glVertex2i(x, yc);
            yf = yf + a;
            if (yf <= 0)
            {
                yf = yf + korekcija;
                yc = yc - 1;
            }
        }
    }
    else
    {
        x = xe;
        xe = ys;
        ys = x;
        x = xs;
        xs = ye;
        ye = x;
        a = 2*(ye - ys);
        yc = ys;
        yf = (xe - xs);
        korekcija = 2*(xe - xs);
        for (x = xs; x <= xe; ++x)
        {
            glVertex2i(yc, x);
            yf = yf + a;
            if (yf <= 0)
            {
                yf = yf + korekcija;
                yc = yc - 1;
            }
        }
    }
}

void bresenham_nacrtaj_cjelobrojni(int xs, int ys, int xe, int ye)
{
    if (xs <= xe)
    {
        if (ys <= ye)
        {
            bresenham_nacrtaj_cjelobrojni2(xs, ys, xe, ye);
        }
        else
        {
            bresenham_nacrtaj_cjelobrojni3(xs, ys, xe, ye);
        }
    }
    else
    {
        if (ys >= ye)
        {
            bresenham_nacrtaj_cjelobrojni2(xe, ye, xs, ys);
        }
        else
        {
            bresenham_nacrtaj_cjelobrojni3(xe, ye, xs, ys);
        }
    }
}

int maska(double x,double y)
{
    if(y>ymax)
        return 8;
    if(y<ymin)
        return 4;
    if(x>xmax)
        return 2;
    if(x<xmin)
        return 1;
    return 0;
}

void cothen_sutherland(int xpoc,int ypoc,int xkraj,int ykraj)
{
    double x1=xpoc;
    double y1=ypoc;
    double x2=xkraj;
    double y2=ykraj;
    while(true)
    {
        int maskPoc=maska(x1,y1);
        int maskKraj=maska(x2,y2);

        if(maskPoc==0&&maskKraj==0)
        {
            glBegin(GL_POINTS);
            bresenham_nacrtaj_cjelobrojni((int)x1,(int)y1,(int)x2,(int)y2);
            glEnd();
            break;
        }
        else if((maskKraj & maskPoc)!=0)
        {
            break;
        }
        else
        {
            int mask = maskPoc ? maskPoc : maskKraj;
            double x, y;
            if (mask &8)
            {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (mask &4)
            {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (mask &2)
            {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else if (mask &1)
            {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (mask == maskPoc)
            {
                x1 = x;
                y1 = y;
            }
            else
            {
                x2 = x;
                y2 = y;
            }
        }

    }

}
