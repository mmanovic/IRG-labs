#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include <float.h>

using namespace std;
double nCr(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) return nCr(n, n - k);
    if (k == 0) return 1;

    double res = n;
    for (int i = 2; i <= k; ++i)
    {
        res *= (n - i + 1);
        res /= i;
    }
    return res;
}
struct Point
{
    double x, y, z, h;
};

vector<Point> vrhovi;

void reshape(int width, int height);
void display();
void renderScene();
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
        double x, y, z;
        sscanf(line, "%lf %lf %lf", &x, &y, &z);
        Point T;
        T.x=x;
        T.y=y;
        T.z=z;
        vrhovi.push_back(T);
    }
    fclose(f);


    normalize(vrhovi);

    printf("===============================\n");
    printf("Ucitane tocke iz %s\n", argv[1]);
    printf("===============================\n");
    for(int i=0; i<vrhovi.size(); i++)
    {
        printf("%.2lf %.2lf %.2lf\n", vrhovi[i].x,vrhovi[i].y, vrhovi[i].z);
    }

    /*
     * Inicijalizacija GLUT-a
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 640);

    glutInitWindowPosition(50, 50);
    glutCreateWindow("Zadatak 7.");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // crtaj scenu
    renderScene();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5,1.5,-1.5,1.5,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    glPointSize(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    /*
     * crtanje zadanih tocaka povezanih linijama
     */
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<vrhovi.size(); i++)
    {
        glVertex3f(vrhovi[i].x,vrhovi[i].y,vrhovi[i].z);
    }
    glEnd();

    vector<Point> aprox;

    int N = vrhovi.size() - 1;
    for (double t = 0; t <= 1; t += 0.01)
    {
        Point T;
        T.x=0.;
        T.y=0.;
        T.z=0.;
        for(int i=0; i<vrhovi.size(); i++)
        {
            T.x+=vrhovi[i].x* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
            T.y+=vrhovi[i].y* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
            T.z+=vrhovi[i].z* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
        }
        aprox.push_back(T);
        printf("%.3lf-> %.3lf %.3lf %.3lf\n", t, T.x, T.y, T.z);
    }


    glColor3f(1.0f, 0.0f, 0.0f);
    /*
     * ctanje bezierove krivulje zadanih bernsteinovim polinomima
     */
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<aprox.size(); i++)
    {
        glVertex3f(aprox[i].x,aprox[i].y,aprox[i].z);
    }
    glEnd();

    return;
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

    return;
}
