#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <GL/glut.h>
using namespace std;

void reshape(int width,int height);
void display();
void renderScene();
void keyboardPressed(unsigned char key, int x, int y);
void izracunajBoju(int n);
int mandelbrot(double x, double y);
int julije(double x, double y);
int width, height;
int maxIter;
int vrstaFraktala=0;
double eps;
double umin, umax, vmin, vmax;
double Cx, Cy;

int main(int argc,char **argv)
{
    printf("Epsilon: ");
    scanf("%lf", &eps);
    printf("Broj iteracija: ");
    scanf("%d", &maxIter);
    printf("Podrucje kompleksne ravnine umin, umax, vmin, vmax: ");
    scanf("%lf%lf%lf%lf", &umin, &umax, &vmin, &vmax);
    printf("Kompleksna konstanta c: ");
    scanf("%lf%lf", &Cx, &Cy);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fraktali");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardPressed);
    glutMainLoop();
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // crtanje scene
    renderScene();
    glutSwapBuffers();
}

void reshape(int w,int h)
{
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width-1, height-1, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            double u0 = i * (umax - umin) / double(width) + umin;
            double v0 = j * (vmax - vmin) / double(height) + vmin;
            int k;
            if(vrstaFraktala)
            {
                k=julije(u0,v0);
            }
            else
            {
                k=mandelbrot(u0,v0);
            }

            izracunajBoju(k);
            glVertex2i(i, j);
        }
    }
    glEnd();
}

int mandelbrot(double x, double y)
{
    int k = -1;
    double cx = x, cy = y;
    double z1x, z1y;
    double z0x = (double)0, z0y = (double)0;

    while (1)
    {
        double r = sqrt(z0x * z0x + z0y * z0y);
        if (r >= eps || k >= maxIter)
        {
            break;
        }
        k++;
        z1x = z0x * z0x - z0y * z0y + cx;
        z1y = (double)2 * z0x * z0y + cy;
        z0x = z1x;
        z0y = z1y;
    }
    return k;
}

int julije(double x, double y)
{
    int k = -1;
    double z1x, z1y;
    double z0x = x, z0y = y;

    while (1)
    {
        double r = sqrt(z0x * z0x + z0y * z0y);
        if (r >= eps || k >= maxIter)
        {
            break;
        };
        k++;
        z1x = z0x * z0x - z0y * z0y + Cx;
        z1y = 2 * z0x * z0y + Cy;
        z0x = z1x;
        z0y = z1y;
    }

    return k;
}

void izracunajBoju(int n)
{
    if (n>=maxIter)
    {
        glColor3f(0.0f,0.0f ,0.0f);
    }
    else
    {
        glColor3f((double)n/maxIter, 1.0-(double)n/maxIter/2.0, 0.8-(double)n/maxIter/3.0);
    }
}

void keyboardPressed(unsigned char key, int x, int y)
{
    if(key=='n')
    {
        vrstaFraktala = 1 - vrstaFraktala;
    }
    glutPostRedisplay();
}
