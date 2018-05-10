#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <float.h>
#include <fstream>
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

struct Poligon
{
    int p1, p2, p3;
    double A, B, C, D;
};

/*
 * stanje sustava
 */
double t;
bool ukloni_straznje;
Point o;
Point g;

/*
 * Matrice transformacije
 */
double T1[4][4], T2[4][4], T3[4][4], T4[4][4], T5[4][4];
/*
 * Matrica projekcije
 */
double P1[4][4];
double mT[4][4];

/*
 * Strukture podataka
 */
vector< Point > vrh;
vector< Poligon > poligon;
vector< Point > putanja;

/*
 * Deklaracije funkcija
 */
void multiply(double a[4][4], double b[4][4], double c[4][4]);
Point transform(Point t, double m[4][4]);
void reshape(int width, int height);
void display();
void renderScene();
void izracunajNormalu(Poligon &a);
void normalize(vector< Point > &v);

vector< Point > izracunajProjekciju(const vector< Point > &v);
vector< Point > izracunajBeziera(void);

void keyboardPressed(unsigned char key, int x, int y);

int main(int argc, char **argv)
{
    g.h=1.;
    ukloni_straznje = 1;
    t = 0.5;

    /*
     * unos gledista
     */
    {
        double x, y, z;
        printf("Unesite glediste: ");
        scanf("%lf %lf %lf", &x, &y, &z);
        g.x=x;
        g.y=y;
        g.z=z;
    }

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
                Poligon P;
                P.p1=--i1;
                P.p2=--i2;
                P.p3=--i3;

                izracunajNormalu(P);

                poligon.push_back(P);
            }
            else if(s == "v")
            {
                double x, y, z;
                sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
                Point T;
                T.x=x;
                T.y=y;
                T.z=z;
                T.h=1.;
                vrh.push_back(T);
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

        double x, y, z;
        sscanf(line, "%lf %lf %lf", &x, &y, &z);
        Point T;
        T.x=x;
        T.y=y;
        T.z=z;
        T.h=1.;

        putanja.push_back(T);
    }
    fclose(f);



    /*
     * sanity print
     */
    printf("Ucitano tijelo iz %s\n", argv[1]);
    printf("=====================================\n");
    printf("Vrhova: %d, Poligona: %d\n", (int)vrh.size(), (int)poligon.size());
    printf("=====================================\n");
    printf("Ucitana putanja iz %s\n", argv[2]);
    printf("=====================================\n");
    printf("Tocaka na putanji: %d\n", (int)putanja.size());
    printf("=====================================\n");


    /*
     * Inicijalizacija GLUT-a
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);

    glutInitWindowPosition(50, 50);
    glutCreateWindow("Zadatak 7. drugi dio");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboardPressed);

    glutMainLoop();
    return 0;
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // crtaj scenu
    renderScene();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5,1.5,-1.5,1.5,0,1.5);
    glMatrixMode(GL_MODELVIEW);

}

void renderScene()
{
    glPointSize(1.0f);

    // izracunaj tocku ocista za dani parametar t
    o.h=1.;
    o.x=0;
    o.y=0;
    o.z=0;

    int N = putanja.size() - 1;

    for (int i=0; i<putanja.size(); i++)
    {
        o.x+=putanja[i].x* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
        o.y+=putanja[i].y* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
        o.z+=putanja[i].z* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
    }




    // crno nacrtaj tijelo
    glColor3f(0.0f, 0.0f, 0.0f);

    vector< Point > projekcija = izracunajProjekciju(vrh);
    normalize(projekcija);

    for (int i=0; i<poligon.size(); i++)
    {
        double r= poligon[i].A*o.x+poligon[i].B*o.y+poligon[i].C*o.z+poligon[i].D;
        if (ukloni_straznje == 0 || r > 0)
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(projekcija[poligon[i].p1].x, projekcija[poligon[i].p1].y);
            glVertex2f(projekcija[poligon[i].p2].x, projekcija[poligon[i].p2].y);
            glVertex2f(projekcija[poligon[i].p3].x, projekcija[poligon[i].p3].y);
            glEnd();
        }
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

    for (int i=0; i<vrh.size(); i++)
    {
        xmax = max(xmax, vrh[i].x);
        xmin = min(xmin, vrh[i].x);
        ymax = max(ymax, vrh[i].y);
        ymin = min(ymin, vrh[i].y);
        zmax = max(zmax, vrh[i].z);
        zmin = min(zmin, vrh[i].z);
    }


    double xsr = (xmin + xmax) / 2.;
    double ysr = (ymin + ymax) / 2.;
    double zsr = (zmin + zmax) / 2.;

    double M = max(max(xmax - xmin, ymax - ymin), zmax - zmin);

    // normalizacija
    // translacija
    for (int i=0; i<vrh.size(); i++)
    {
        vrh[i].x-=xsr;
        vrh[i].y-=ysr;
        vrh[i].z-=zsr;
        vrh[i].x=vrh[i].x*2/M;
        vrh[i].y=vrh[i].y*2/M;
        vrh[i].z=vrh[i].z*2/M;
    }
    return;
}

void izracunajNormalu(Poligon &a)
{
    double v2x = vrh[a.p2].x - vrh[a.p1].x;
    double v2y = vrh[a.p2].y - vrh[a.p1].y;
    double v2z = vrh[a.p2].z - vrh[a.p1].z;

    double v3x = vrh[a.p3].x - vrh[a.p1].x;
    double v3y = vrh[a.p3].y - vrh[a.p1].y;
    double v3z = vrh[a.p3].z - vrh[a.p1].z;

    a.A=(v2y * v3z - v3y * v2z);
    a.B=(v3x * v2z - v2x * v3z);
    a.C=(v2x * v3y - v3x * v2y);
    a.D=-vrh[a.p1].x*a.A-vrh[a.p1].y*a.B-vrh[a.p1].z*a.C;

    return;
}

void keyboardPressed(unsigned char key, int x, int y)
{
    bool refresh = false;

    switch(key)
    {
    case 'p':
        ukloni_straznje = 1 - ukloni_straznje;
        refresh = true;
        break;
    case 'j':
        t += 0.01;
        t = min(t, 1.);
        refresh = true;
        break;
    case 'k':
        t -= 0.01;
        t = max(t, 0.);
        refresh = true;
        break;
    default:
        refresh = false;
        break;
    }

    if (refresh)
    {
        glutPostRedisplay();
    }
}

vector< Point > izracunajProjekciju(const vector< Point > &v)
{
    vector< Point > projekcija = v;

    Point g1;
    g1.x=g.x;
    g1.y=g.y;
    g1.z=g.z;
    g1.h=g.h;

    T1[0][0] = 1;
    T1[1][1] = 1;
    T1[2][2] = 1;
    T1[3][0] = -o.x;
    T1[3][1] = -o.y;
    T1[3][2] = -o.z;
    T1[3][3] = 1;
    g1.x-=o.x;
    g1.y-=o.y;
    g1.z-=o.z;

    double cosa, sina;
    if (g1.x == 0 && g1.y == 0)
    {
        cosa = 1;
        sina = 0;
    }
    else
    {
        cosa = g1.x / sqrt(pow(g1.x, 2) + pow(g1.y, 2));
        sina = g1.y / sqrt(pow(g1.x, 2) + pow(g1.y, 2));
        g1.x=sqrt(pow(g1.x, 2) + pow(g1.y, 2));
        g1.y=0;
    }

    T2[0][0] = cosa;
    T2[0][1] = -sina;
    T2[1][0] = sina;
    T2[1][1] = cosa;
    T2[2][2] = 1;
    T2[3][3] = 1;

    double cosb, sinb;
    if (g1.x == 0 && g1.z == 0)
    {
        cosb = 1;
        sinb = 0;
    }
    else
    {
        cosb = g1.z / sqrt(pow(g1.x, 2) + pow(g1.z, 2));
        sinb = g1.x / sqrt(pow(g1.x, 2) + pow(g1.z, 2));
        g1.z=sqrt(pow(g1.x, 2) + pow(g1.z, 2));
        g1.x=0;
        g1.y=0;
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
    P1[2][3] = 1. / g1.z;



    for(int i=0; i<projekcija.size(); i++)
    {
        projekcija[i]=transform(projekcija[i],mT);
        projekcija[i]=transform(projekcija[i],P1);
    }



    printf("=====================================\n");
    printf("Ociste: %.3lf %.3lf %.3lf\n", o.x,o.y,o.z);
    printf("Glediste: %.3lf %.3lf %.3lf\n", g.x,g.y,g.z);
    printf("=====================================\n");


    return projekcija;
}

vector< Point > izracunajBeziera(void)
{
    vector< Point > bezier;

    int N = putanja.size() - 1;

    for (double t = 0; t <= 1; t += 0.01)
    {
        Point T;
        T.x=0.;
        T.y=0.;
        T.z=0.;
        for(int i=0; i<putanja.size(); i++)
        {
            T.x+=putanja[i].x* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
            T.y+=putanja[i].y* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
            T.z+=putanja[i].z* nCr(N, i) * pow(t, i) * pow(1 - t, N - i);
        }
        bezier.push_back(T);
    }
    return bezier;
}
