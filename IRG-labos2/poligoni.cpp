#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <stdio.h>
using namespace std;

int sirina=500,visina=500;
int misX = 0, misY = 0;
int stanje = 1;
bool konveksnost = false, popunjavanje = false;

typedef struct
{
    int x;
    int y;
} iTocka2D;

typedef struct
{
    int a;
    int b;
    int c;
} iBrid2D;

typedef struct
{
    iTocka2D Vrh;
    iBrid2D Brid;
    bool lijevi;
} iPolyElem;
vector<iPolyElem> poligon;

void RacunajKoefPoligonKonv(std::vector<iPolyElem>&);
void PopuniPoligonKonv(std::vector<iPolyElem>&, int);
void ProvjeriPoligonKonv(std::vector<iPolyElem>&, int*, int*);
int ProvjeriTockaPozicija(std::vector<iPolyElem>&, int, int, int);
void reshape(int, int);
void display();
void renderScene();
void keyPressed(unsigned char, int, int);
void mouseMoved(int, int);
void mousePressedOrReleased(int, int, int, int);

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(sirina, visina);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Poligoni");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressedOrReleased);
    glutPassiveMotionFunc(mouseMoved);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}

void display()
{
    if(konveksnost)
    {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // crtanje scene:
    renderScene();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    sirina = width;
    visina = height;
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width-1, height-1, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
    glPointSize(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    if(stanje==1)
    {
        if(!popunjavanje && (int)poligon.size() > 0)
        {
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < (int)poligon.size(); ++i)
            {
                glVertex2i(poligon[i].Vrh.x, poligon[i].Vrh.y);
            }
            glVertex2i(misX, misY);
            glEnd();
        }
        if(popunjavanje && (int)poligon.size() >1)
        {
            std::vector<iPolyElem> zaPopunjavanje (poligon);
            iPolyElem elem;
            elem.Vrh.x = misX;
            elem.Vrh.y = misY;
            zaPopunjavanje.push_back(elem);
            RacunajKoefPoligonKonv(zaPopunjavanje);
            int konv, orij;
            ProvjeriPoligonKonv(zaPopunjavanje, &konv, &orij);
            PopuniPoligonKonv(zaPopunjavanje, orij);
        }
    }
    else
    {
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < (int)poligon.size(); ++i)
        {
            glVertex2i(poligon[i].Vrh.x, poligon[i].Vrh.y);
        }
        glEnd();
    }

}
void mousePressedOrReleased(int button,int state,int x,int y)
{
    if(state== GLUT_DOWN && button==GLUT_LEFT_BUTTON)
    {
        if(stanje==1)
        {
            if(konveksnost)
            {
                int konv, orij;
                vector<iPolyElem> zaPopunjavanje(poligon);
                iPolyElem elem;
                elem.Vrh.x = x;
                elem.Vrh.y = y;
                zaPopunjavanje.push_back(elem);
                RacunajKoefPoligonKonv(zaPopunjavanje);
                ProvjeriPoligonKonv(zaPopunjavanje, &konv, &orij);
                if(konv == 0)
                {
                    printf("Ne moze se dodati tocka (%d, %d) jer bi poligon postao konkavan!\n", x, y);
                }
                else
                {
                    printf("Tocka: (%d, %d) OK\n",x,y);
                    iPolyElem elem;
                    elem.Vrh.x = x;
                    elem.Vrh.y = y;
                    poligon.push_back(elem);
                }

            }
            else
            {
                printf("Tocka: (%d, %d) OK\n",x,y);
                iPolyElem elem;
                elem.Vrh.x = x;
                elem.Vrh.y = y;
                poligon.push_back(elem);
            }
        }
        else
        {
            int konv, orij;
            RacunajKoefPoligonKonv(poligon);
            ProvjeriPoligonKonv(poligon, &konv, &orij);
            int tockaPozicija = ProvjeriTockaPozicija(poligon, orij, x, y);
            if(tockaPozicija == 0)
            {
                printf("Tocka (%d, %d) je unutar poligona\n", x, y);
            }
            else if(tockaPozicija == -1)
            {
                printf("Tocka (%d, %d) je izvan poligona\n", x, y);
            }
            else
            {
                printf("Tocka (%d, %d) je na poligonu\n", x, y);
            }

        }
    }
}
void keyPressed(unsigned char key, int x, int y)
{
    if(stanje == 1)
    {
        if(key == 'k')
        {
            int konv, orij;
            vector<iPolyElem> zaPopunjavanje(poligon);
            RacunajKoefPoligonKonv(zaPopunjavanje);
            ProvjeriPoligonKonv(zaPopunjavanje, &konv, &orij);
            if(konv==1 || poligon.size()<4)
            {
                konveksnost = !konveksnost;
            }
            else
            {
                printf("Nije moguce promijeniti zastavicu konkveksnost jer je trenutni poligon konkavan.\n");
            }
        }
        if(key == 'p')
        {
            popunjavanje = !popunjavanje;
        }
        if(key == 'n')
        {
            stanje = 2;
        }
    }
    else if(stanje == 2)
    {
        if(key == 'n')
        {
            stanje = 1;
            poligon.clear();
            konveksnost = false;
            popunjavanje = false;
        }
    }
    glutPostRedisplay();
}

void mouseMoved(int x, int y)
{
    misX = x;
    misY = y;
    glutPostRedisplay();
}

void RacunajKoefPoligonKonv(std::vector<iPolyElem> &poligon)
{
    int i0 = (int)poligon.size() - 1;
    for(int i = 0; i < (int)poligon.size(); ++i)
    {
        poligon[i0].Brid.a = poligon[i0].Vrh.y - poligon[i].Vrh.y;
        poligon[i0].Brid.b = -(poligon[i0].Vrh.x - poligon[i].Vrh.x);
        poligon[i0].Brid.c = poligon[i0].Vrh.x * poligon[i].Vrh.y - poligon[i0].Vrh.y * poligon[i].Vrh.x;
        poligon[i0].lijevi = poligon[i0].Vrh.y > poligon[i].Vrh.y;
        i0 = i;
    }
}

void PopuniPoligonKonv(std::vector<iPolyElem> &poligon, int orij)
{
    int i, i0, y;
    int xmin, xmax, ymin, ymax;
    double L,D,x;

    xmin = xmax = poligon[0].Vrh.x;
    ymin = ymax = poligon[0].Vrh.y;

    for(i = 1; i < (int)poligon.size(); ++i)
    {
        if(xmin > poligon[i].Vrh.x) xmin = poligon[i].Vrh.x;
        if(xmax < poligon[i].Vrh.x) xmax = poligon[i].Vrh.x;
        if(ymin > poligon[i].Vrh.y) ymin = poligon[i].Vrh.y;
        if(ymax < poligon[i].Vrh.y) ymax = poligon[i].Vrh.y;
    }
    for(y = ymin; y <= ymax; ++y)
    {
        L = xmin;
        D = xmax;
        //printf("L: %lf, D: %lf\n", L, D);
        i0 = (int)poligon.size() - 1;
        for(i = 0; i < (int)poligon.size(); i0=i++)
        {
            if(poligon[i0].Brid.a == 0.)
            {
                if(poligon[i0].Vrh.y == y)
                {
                    if(poligon[i0].Vrh.x < poligon[i].Vrh.x)
                    {
                        L = poligon[i0].Vrh.x;
                        D = poligon[i].Vrh.x;
                    }
                    else
                    {
                        L = poligon[i].Vrh.x;
                        D = poligon[i0].Vrh.x;
                    }
                    break;
                }
            }
            else
            {
                x = (-poligon[i0].Brid.b * y - poligon[i0].Brid.c)/(double)poligon[i0].Brid.a;
                if(poligon[i0].lijevi)
                {
                    if(L < x) L = x;
                }
                else
                {
                    if(D > x) D = x;
                }
            }
        }

        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2i((int)L, y);
        glVertex2i((int)D, y);
        glEnd();

    }
}
void ProvjeriPoligonKonv(std::vector<iPolyElem> &poligon, int *konv, int *orij)
{
    int i, i0, r;
    int iznad, ispod, na;

    ispod = iznad = na = 0;
    i0 = (int)poligon.size()-2;

    for(i = 0; i < (int)poligon.size(); i++,i0++)
    {
        if(i0 >= (int)poligon.size()) i0 = 0;
        r = poligon[i0].Brid.a * poligon[i].Vrh.x + poligon[i0].Brid.b * poligon[i].Vrh.y + poligon[i0].Brid.c;
        if(r == 0) ++na;
        else if(r > 0) iznad++;
        else ispod++;
    }

    *konv = 0;
    *orij = 0;
    if(ispod == 0)
    {
        *konv = 1;
    }
    else if (iznad == 0)
    {
        *konv = 1;
        *orij = 1;
    }
}

int ProvjeriTockaPozicija(std::vector<iPolyElem> &poligon, int orij, int x, int y)
{
    int i, i0, r;
    int iznad, ispod, na;

    ispod = iznad = na = 0;
    i0 = (int)poligon.size()-2;

    for(i = 0; i < (int)poligon.size(); i++,i0++)
    {
        if(i0 >= (int)poligon.size()) i0 = 0;
        r = poligon[i0].Brid.a * x + poligon[i0].Brid.b * y + poligon[i0].Brid.c;
        if(r == 0) ++na;
        else if(r > 0) iznad++;
        else ispod++;
    }

    if(na > 0)
    {
        return 1;
    }
    else if(orij)
    {
        if(iznad == 0) return 0;
    }
    else
    {
        if(ispod == 0) return 0;
    }
    return -1;
}
