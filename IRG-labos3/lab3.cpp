#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <float.h>
#include <fstream>
#include <algorithm>
using namespace std;

struct Point
{
    double x, y, z, h;
};

struct Poligon
{
    int v1, v2, v3;
};

vector<Point> vrhovi;
vector<Poligon> poligoni;
vector<pair<int, int> > tocke;

Point o;
Point g;
void multiply(double a[4][4], double b[4][4], double c[4][4]);
Point transform(Point t, double m[4][4]);
double T1[4][4], T2[4][4], T3[4][4], T4[4][4], T5[4][4];
double P1[4][4];
double mT[4][4];


void display();
void renderScene();
void reshape(int width, int height);
void normalize(vector<Point> &v);


int main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "r");
    if(f == NULL)
    {
        printf("Unable to read file!");
        return -1;
    }
    char line[1024], tip[20];

    while(fgets(line, sizeof line, f))
    {
        if(sscanf(line, "%s", tip) == 1)
        {
            string s = tip;
            if(s == "f")
            {
                int i1, i2, i3;
                sscanf(line, "%*s %d %d %d", &i1, &i2, &i3);
                Poligon p;
                p.v1 = --i1;
                p.v2 = --i2;
                p.v3 = --i3;
                poligoni.push_back(p);
            }
            else if(s == "v")
            {
                double x, y, z;
                sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
                Point v;
                v.x = x;
                v.y = y;
                v.z = z;
                v.h = 1.;
                vrhovi.push_back(v);
            }
        }
    }
    fclose(f);
    f = fopen(argv[2], "r");
    if(f == NULL)
    {
        printf("Unable to read file!");
        return -1;
    }
    while(fgets(line, sizeof line, f))
    {
        if(sscanf(line, "%s", tip) == 1)
        {
            string s = tip;
            if(s == "G")
            {
                double x, y, z;
                sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
                g.x = x;
                g.y = y;
                g.z = z;
                g.h = 1.;
            }
            else if(s == "O")
            {
                double x, y, z;
                sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
                o.x = x;
                o.y = y;
                o.z = z;
                o.h = 1.;
            }
        }
    }
    fclose(f);




    // inicijlizacija matrica ===========

    T1[0][0] = 1;
    T1[1][1] = 1;
    T1[2][2] = 1;
    T1[3][0] = -o.x;
    T1[3][1] = -o.y;
    T1[3][2] = -o.z;
    T1[3][3] = 1;
    g.x-=o.x;
    g.y-=o.y;
    g.z-=o.z;

    double cosa, sina;
    if (g.x == 0 && g.y == 0)
    {
        cosa = 1;
        sina = 0;
    }
    else
    {
        cosa = g.x / sqrt(pow(g.x, 2) + pow(g.y, 2));
        sina = g.y / sqrt(pow(g.x, 2) + pow(g.y, 2));
        g.x=sqrt(pow(g.x, 2) + pow(g.y, 2));
        g.y=0;
    }

    T2[0][0] = cosa;
    T2[0][1] = -sina;
    T2[1][0] = sina;
    T2[1][1] = cosa;
    T2[2][2] = 1;
    T2[3][3] = 1;

    double cosb, sinb;
    if (g.x == 0 && g.z == 0)
    {
        cosb = 1;
        sinb = 0;
    }
    else
    {
        cosb = g.z / sqrt(pow(g.x, 2) + pow(g.z, 2));
        sinb = g.x / sqrt(pow(g.x, 2) + pow(g.z, 2));
        g.z=sqrt(pow(g.x, 2) + pow(g.z, 2));
        g.x=0;
        g.y=0;
    }
    T3[0][0] = cosb;
    T3[0][2] = sinb;
    T3[1][1] = 1;
    T3[2][0] = -sinb;
    T3[2][2] = cosb;
    T3[3][3] = 1;

    T4[0][1] = -1;
    T4[1][0] = 1;
    T4[2][2] = 1;
    T4[3][3] = 1;

    T5[0][0] = -1;
    T5[1][1] = 1;
    T5[2][2] = 1;
    T5[3][3] = 1;

    double tempM1[4][4];
    multiply(T1, T2, tempM1);
    multiply(tempM1, T3, mT);
    multiply(mT, T4, tempM1);
    multiply(tempM1, T5, mT);

    P1[0][0] = 1;
    P1[1][1] = 1;
    P1[2][3] = 1. / g.z;

    for(int i=0; i<vrhovi.size(); i++)
    {
        vrhovi[i]=transform(vrhovi[i],mT);
        vrhovi[i]=transform(vrhovi[i],P1);
    }



    normalize(vrhovi);


    /*
     * Inicijalizacija GLUT-a
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);

    glutInitWindowPosition(50, 50);
    glutCreateWindow("Zadatak 6.");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
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
    glOrtho(0, width-1,0,height-1,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    glPointSize(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    for(int i=0; i<poligoni.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2i(tocke[poligoni[i].v1].first, tocke[poligoni[i].v1].second);
        glVertex2i(tocke[poligoni[i].v2].first, tocke[poligoni[i].v2].second);
        glVertex2i(tocke[poligoni[i].v3].first, tocke[poligoni[i].v3].second);
        glEnd();
    }
    return;
}

void multiply(double a[4][4], double b[4][4], double c[4][4])
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            double temp = 0;
            for (int k = 0; k < 4; ++k)
            {
                temp += a[i][k] * b[k][j];
            }
            c[i][j] = temp;
        }
    }
    return;
}

Point transform(Point t, double M[4][4])
{
    Point result;
    result.x=t.x * M[0][0] + t.y * M[1][0] + t.z * M[2][0] + t.h * M[3][0];
    result.y=t.x * M[0][1] + t.y * M[1][1] + t.z * M[2][1] + t.h * M[3][1];
    result.z=t.x * M[0][2] + t.y * M[1][2] + t.z * M[2][2] + t.h * M[3][2];
    result.h=t.x * M[0][3] + t.y * M[1][3] + t.z * M[2][3] + t.h * M[3][3];
    if (result.h != 1.0 && result.h != 0)
    {
        result.x/=result.h;
        result.y/=result.h;
        result.z/=result.h;
        result.h=1.;
    }
    return result;
}

void normalize(vector<Point> &v)
{

    double xmax = DBL_MIN, xmin = DBL_MAX;
    double ymax = DBL_MIN, ymin = DBL_MAX;
    double zmax = DBL_MIN, zmin = DBL_MAX;

    for (int i=0; i<vrhovi.size(); i++)
    {
        xmax = max(xmax, vrhovi[i].x);
        xmin = min(xmin, vrhovi[i].x);
        ymax = max(ymax, vrhovi[i].y);
        ymin = min(ymin, vrhovi[i].y);
        zmax = max(zmax, vrhovi[i].z);
        zmin = min(zmin, vrhovi[i].z);
    }


    double xsr = (xmin + xmax) / 2.;
    double ysr = (ymin + ymax) / 2.;
    double zsr = (zmin + zmax) / 2.;

    double M = max(max(xmax - xmin, ymax - ymin), zmax - zmin);

    // normalizacija
    // translacija
    for (int i=0; i<vrhovi.size(); i++)
    {
        vrhovi[i].x-=xsr;
        vrhovi[i].y-=ysr;
        vrhovi[i].z-=zsr;
        vrhovi[i].x=vrhovi[i].x*2/M;
        vrhovi[i].y=vrhovi[i].y*2/M;
        vrhovi[i].z=vrhovi[i].z*2/M;
    }
    for (int i=0; i<vrhovi.size(); i++)
    {
        tocke.push_back(make_pair((int)(vrhovi[i].x*300+320),(int)(vrhovi[i].y*300+320)));
        vrhovi[i].x=vrhovi[i].x*300+320;
        vrhovi[i].y=vrhovi[i].y*300+320;
    }


    return;
}





