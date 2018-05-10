#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include <float.h>
using namespace std;
int width,height;
struct Point
{
    double x, y, z, h;
    double A, B, C;
    int n;
};

struct Poligon
{
    int p1, p2, p3;
    double A, B, C, D;
};

Point ociste,izvor;
Point pocetno,pocetnoi;
int prikaz=0;
vector< Point > vrhovi;
vector< Poligon > poligon;

double izracunajIntenzitet(Point &N,Point &L);
void normalizeVector(Point &v);
void izracunajNormalu(Poligon &a);
void konstantniPrikaz();
void gouraudovPrikaz();
bool isVisible(Poligon &p);
void izracunajNormaleVrhova();
void zicaniPrikaz();
void normalize(vector< Point > &v);
void myDisplay		();
void myReshape		(int width, int height);
void myMouse		(int button, int state, int x, int y);
void myKeyboard		(unsigned char theKey, int mouseX, int mouseY);
void renderScene();
void redisplay_all	(void);
void updatePerspective ();

//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char** argv)
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
                vrhovi.push_back(T);
            }
        }
    }
    fclose(f);
    normalize(vrhovi);
    izracunajNormaleVrhova();

    for (int i=0; i<vrhovi.size(); i++)
    {
        //printf("%.2f  %.2f  %.2f %.2f %.2f %.2f %d\n",vrhovi[i].x,vrhovi[i].y,vrhovi[i].z,vrhovi[i].A,vrhovi[i].B,vrhovi[i].C,vrhovi[i].n);
    }

    double x, y, z;
    printf("Unesite polozaj ocista: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    ociste.x=x;
    pocetno.x=x;
    ociste.y=y;
    pocetno.y=y;
    ociste.z=z;
    pocetno.z=z;
    printf("Unesite polozaj izvora: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    izvor.x=x;
    pocetnoi.x=x;
    izvor.y=y;
    pocetnoi.y=y;
    izvor.z=z;
    pocetnoi.z=z;

    // postavljanje dvostrukog spremnika za prikaz (zbog titranja)
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    glutCreateWindow ("zadatak 7.");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();
    return 0;
}

//*********************************************************************************
//	Osvjezavanje prikaza.
//*********************************************************************************

void myDisplay(void)
{
    // printf("Pozvan myDisplay()\n");
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);		         // boja pozadine - bijela
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    glutSwapBuffers();      // iscrtavanje iz dvostrukog spemnika (umjesto glFlush)
}

void renderScene()
{
    glPointSize(1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    if (prikaz == 0)
    {
        zicaniPrikaz();
    }
    else if (prikaz == 1)
    {
        konstantniPrikaz();
    }
    else if (prikaz == 2)
    {
        gouraudovPrikaz();
    }
}

void myReshape (int w, int h)
{
    // printf("MR: width=%d, height=%d\n",w,h);
    width=w;
    height=h;
    glViewport (0, 0, width, height);
    updatePerspective();
}

void updatePerspective()
{
    glMatrixMode (GL_PROJECTION);        // aktivirana matrica projekcije
    glLoadIdentity ();
    gluPerspective(45.0, (float)width/height, 0.5, 8.0); // kut pogleda, x/y, prednja i straznja ravnina odsjecanja
    glMatrixMode (GL_MODELVIEW);         // aktivirana matrica modela
    glLoadIdentity ();
    gluLookAt (ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
}

void zicaniPrikaz()
{
    for(int i=0; i<poligon.size(); i++)
    {
        if(!isVisible(poligon[i])) continue;
        glBegin(GL_LINE_LOOP);
        glVertex3f(vrhovi[poligon[i].p1].x, vrhovi[poligon[i].p1].y, vrhovi[poligon[i].p1].z);
        glVertex3f(vrhovi[poligon[i].p2].x, vrhovi[poligon[i].p2].y, vrhovi[poligon[i].p2].z);
        glVertex3f(vrhovi[poligon[i].p3].x, vrhovi[poligon[i].p3].y, vrhovi[poligon[i].p3].z);
        glEnd();
    }
}

void konstantniPrikaz()
{
    for(int i=0; i<poligon.size(); i++)
    {
        if(!isVisible(poligon[i])) continue;
        double x_sr = vrhovi[poligon[i].p1].x + vrhovi[poligon[i].p2].x + vrhovi[poligon[i].p3].x;
        double y_sr = vrhovi[poligon[i].p1].y + vrhovi[poligon[i].p2].y + vrhovi[poligon[i].p3].y;
        double z_sr = vrhovi[poligon[i].p1].z + vrhovi[poligon[i].p2].z + vrhovi[poligon[i].p3].z;
        x_sr /= 3.;
        y_sr /= 3.;
        z_sr /= 3.;
        Point N,L;
        N.x=poligon[i].A;
        N.y=poligon[i].B;
        N.z=poligon[i].C;
        L.x=izvor.x-x_sr;
        L.y=izvor.y-y_sr;
        L.z=izvor.z-z_sr;
        double I=izracunajIntenzitet(N,L);
        glBegin(GL_TRIANGLES);
        glColor3ub(0, I, I);
        glVertex3f(vrhovi[poligon[i].p1].x, vrhovi[poligon[i].p1].y, vrhovi[poligon[i].p1].z);
        glVertex3f(vrhovi[poligon[i].p2].x, vrhovi[poligon[i].p2].y, vrhovi[poligon[i].p2].z);
        glVertex3f(vrhovi[poligon[i].p3].x, vrhovi[poligon[i].p3].y, vrhovi[poligon[i].p3].z);
        glEnd();
    }
}
void gouraudovPrikaz()
{
    for(int i=0; i<poligon.size(); i++)
    {
        if(!isVisible(poligon[i])) continue;
        Point N,L;
        N.x=vrhovi[poligon[i].p1].A;
        N.y=vrhovi[poligon[i].p1].B;
        N.z=vrhovi[poligon[i].p1].C;
        L.x=izvor.x-vrhovi[poligon[i].p1].x;
        L.y=izvor.y-vrhovi[poligon[i].p1].y;
        L.z=izvor.z-vrhovi[poligon[i].p1].z;
        double I1=izracunajIntenzitet(N,L);
        N.x=vrhovi[poligon[i].p2].A;
        N.y=vrhovi[poligon[i].p2].B;
        N.z=vrhovi[poligon[i].p2].C;
        L.x=izvor.x-vrhovi[poligon[i].p2].x;
        L.y=izvor.y-vrhovi[poligon[i].p2].y;
        L.z=izvor.z-vrhovi[poligon[i].p2].z;
        double I2=izracunajIntenzitet(N,L);
        N.x=vrhovi[poligon[i].p3].A;
        N.y=vrhovi[poligon[i].p3].B;
        N.z=vrhovi[poligon[i].p3].C;
        L.x=izvor.x-vrhovi[poligon[i].p3].x;
        L.y=izvor.y-vrhovi[poligon[i].p3].y;
        L.z=izvor.z-vrhovi[poligon[i].p3].z;
        double I3=izracunajIntenzitet(N,L);
        glBegin(GL_TRIANGLES);
        glColor3ub(0, I1, I1);
        glVertex3f(vrhovi[poligon[i].p1].x, vrhovi[poligon[i].p1].y, vrhovi[poligon[i].p1].z);
        glColor3ub(0, I2, I2);
        glVertex3f(vrhovi[poligon[i].p2].x, vrhovi[poligon[i].p2].y, vrhovi[poligon[i].p2].z);
        glColor3ub(0, I3, I3);
        glVertex3f(vrhovi[poligon[i].p3].x, vrhovi[poligon[i].p3].y, vrhovi[poligon[i].p3].z);
        glEnd();
    }
}
double izracunajIntenzitet(Point &N,Point &L)
{
    double Ia = 100;
    double ka = 0.5;
    double Ii = 200;
    double kd = 0.8;
    normalizeVector(N);
    normalizeVector(L);

    double produktLN=N.x*L.x+N.y*L.y+N.z*L.z;
    if(produktLN<0)
    {
        produktLN=0;
    }
    return Ii * kd * produktLN + Ia * ka;
}
void normalizeVector(Point &v)
{
    double absolute = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    v.x/=absolute;
    v.y/=absolute;
    v.z/=absolute;
}
bool isVisible(Poligon &p)
{
    double x_sr = vrhovi[p.p1].x + vrhovi[p.p2].x + vrhovi[p.p3].x;
    double y_sr = vrhovi[p.p1].y + vrhovi[p.p2].y + vrhovi[p.p3].y;
    double z_sr = vrhovi[p.p1].z + vrhovi[p.p2].z + vrhovi[p.p3].z;
    x_sr /= 3.;
    y_sr /= 3.;
    z_sr /= 3.;
    double r=p.A*(ociste.x-x_sr)+p.B*(ociste.y-y_sr)+p.C*(ociste.z-z_sr);
    if(r<0) return false;
    return true;
}

void myMouse(int button, int state, int x, int y)
{
    //	Desna tipka - brise canvas.
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        ociste.x=0;
        updatePerspective();
        glutPostRedisplay();
    }
}

//*********************************************************************************
//	Tastatura tipke - esc - izlazi iz programa.
//*********************************************************************************

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'l':
        ociste.x = ociste.x+0.1;
        break;
    case 'k':
        ociste.x =ociste.x-0.1;
        break;
    case 'h':
        ociste.y+=0.1;
        break;
    case 'j':
        ociste.y-=0.1;
        break;
    case 'w':
        izvor.y+=0.1;
        break;
    case 'a':
        izvor.x-=0.1;
        break;
    case 's':
        izvor.y-=0.1;
        break;
    case 'd':
        izvor.x+=0.1;
        break;
    case 'r':
        ociste.x=pocetno.x;
        ociste.y=pocetno.y;
        izvor.x=pocetnoi.x;
        izvor.y=pocetnoi.y;
        break;
    case 'p':
        prikaz = (prikaz + 1) % 3;
        break;
    case 27:
        exit(0);
        break;
    }
    updatePerspective();
    glutPostRedisplay();
}

void izracunajNormalu(Poligon &a)
{
    double v2x = vrhovi[a.p2].x - vrhovi[a.p1].x;
    double v2y = vrhovi[a.p2].y - vrhovi[a.p1].y;
    double v2z = vrhovi[a.p2].z - vrhovi[a.p1].z;

    double v3x = vrhovi[a.p3].x - vrhovi[a.p1].x;
    double v3y = vrhovi[a.p3].y - vrhovi[a.p1].y;
    double v3z = vrhovi[a.p3].z - vrhovi[a.p1].z;

    a.A=(v2y * v3z - v3y * v2z);
    a.B=(v3x * v2z - v2x * v3z);
    a.C=(v2x * v3y - v3x * v2y);
    a.D=-vrhovi[a.p1].x*a.A-vrhovi[a.p1].y*a.B-vrhovi[a.p1].z*a.C;

    return;
}

void normalize(vector<Point> &v)
{

    double xmax = DBL_MIN, xmin = DBL_MAX;
    double ymax = DBL_MIN, ymin = DBL_MAX;
    double zmax = DBL_MIN, zmin = DBL_MAX;

    for (int i=0; i<v.size(); i++)
    {
        xmax = max(xmax, v[i].x);
        xmin = min(xmin, v[i].x);
        ymax = max(ymax, v[i].y);
        ymin = min(ymin, v[i].y);
        zmax = max(zmax, v[i].z);
        zmin = min(zmin, v[i].z);
    }


    double xsr = (xmin + xmax) / 2.;
    double ysr = (ymin + ymax) / 2.;
    double zsr = (zmin + zmax) / 2.;

    double M = max(max(xmax - xmin, ymax - ymin), zmax - zmin);

    // normalizacija
    // translacija
    for (int i=0; i<v.size(); i++)
    {
        v[i].x-=xsr;
        v[i].y-=ysr;
        v[i].z-=zsr;
        v[i].x=v[i].x*2/M;
        v[i].y=v[i].y*2/M;
        v[i].z=v[i].z*2/M;
    }
    return;
}
void dodaj(int index,Poligon &p)
{
    vrhovi[index].A+=p.A;
    vrhovi[index].B+=p.B;
    vrhovi[index].C+=p.C;
    vrhovi[index].n++;

}

void izracunajNormaleVrhova()
{
    for (int i=0; i<poligon.size(); i++)
    {
        dodaj(poligon[i].p1,poligon[i]);
        dodaj(poligon[i].p2,poligon[i]);
        dodaj(poligon[i].p3,poligon[i]);
    }
    for(int i=0; i<vrhovi.size(); i++)
    {
        vrhovi[i].A/=vrhovi[i].n;
        vrhovi[i].B/=vrhovi[i].n;
        vrhovi[i].C/=vrhovi[i].n;
    }
}

