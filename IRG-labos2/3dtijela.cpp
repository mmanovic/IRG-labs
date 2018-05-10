#include <iostream>
#include <vector>
#include "Vector.h"
#include "IVector.h"
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

struct Vertex3D
{
    double x, y, z;
};

struct Face3D
{
    int indexes[3];
    double a, b, c, d;
};

class ObjectModel
{
public:
    std::vector<Vertex3D> vertices;
    std::vector<Face3D> faces;
    ObjectModel(vector<Vertex3D> v, vector<Face3D> f) : vertices(v), faces(f)
    {
        izracunajKoeficijente();
    }
    ObjectModel copy()
    {
        return ObjectModel(vertices, faces);
    }

    string dumpToOBJ()
    {
        string s;
        char numstr1[51];
        char numstr2[51];
        char numstr3[51];
        for(int i = 0; i < vertices.size(); i++)
        {
            sprintf(numstr1,"%lf",vertices[i].x);
            sprintf(numstr2,"%lf",vertices[i].y);
            sprintf(numstr3,"%lf",vertices[i].z);
            s+= "v ";
            s+=numstr1;
            s+=" ";
            s+=numstr2;
            s+=" ";
            s+=numstr3;
            s+="\n";
        }
        for(int i = 0; i < faces.size(); i++)
        {
            sprintf(numstr1,"%d",faces[i].indexes[0]+1);
            sprintf(numstr2,"%d",faces[i].indexes[1]+1);
            sprintf(numstr3,"%d",faces[i].indexes[2]+1);
            s+= "f ";
            s+=numstr1;
            s+=" ";
            s+=numstr2;
            s+=" ";
            s+=numstr3;
            s+="\n";
        }
        return s;
    }

    void normalize()
    {
        double xmin = vertices[0].x, xmax = vertices[0].x;
        double ymin = vertices[0].y, ymax = vertices[0].y;
        double zmin = vertices[0].z, zmax = vertices[0].z;
        for(int i = 1; i < (int)vertices.size(); ++i)
        {
            if(vertices[i].x < xmin) xmin = vertices[i].x;
            if(vertices[i].x > xmax) xmax = vertices[i].x;
            if(vertices[i].y < ymin) ymin = vertices[i].y;
            if(vertices[i].y > ymax) ymax = vertices[i].y;
            if(vertices[i].z < zmin) zmin = vertices[i].z;
            if(vertices[i].z > zmax) zmax = vertices[i].z;
        }

        double sredisteX = (xmin + xmax) / 2.0;
        double sredisteY = (ymin + ymax) / 2.0;
        double sredisteZ = (zmin + zmax) / 2.0;

        double M = max(xmax - xmin, max(ymax - ymin, zmax - zmin));

        for(int i = 1; i < (int)vertices.size(); ++i)
        {
            vertices[i].x = (vertices[i].x - sredisteX) * 2/M;
            vertices[i].y = (vertices[i].y - sredisteY) * 2/M;
            vertices[i].z = (vertices[i].z - sredisteZ) * 2/M;
        }
    }

    void izracunajKoeficijente()
    {
        for(int i = 0; i < (int)faces.size(); ++i)
        {
            //printf("uaishdauishd");
            int i0 = faces[i].indexes[0];
            int i1 = faces[i].indexes[1];
            int i2 = faces[i].indexes[2];
            vector<double> v1;
            v1.push_back(vertices[i1].x - vertices[i0].x);
            v1.push_back(vertices[i1].y - vertices[i0].y);
            v1.push_back(vertices[i1].z - vertices[i0].z);
            vector<double> v2;
            v2.push_back(vertices[i2].x - vertices[i0].x);
            v2.push_back(vertices[i2].y - vertices[i0].y);
            v2.push_back(vertices[i2].z - vertices[i0].z);

            faces[i].a = v1[1]*v2[2]-v1[2]*v2[1];
            faces[i].b = -(v1[0]*v2[2]-v1[2]*v2[0]);
            faces[i].c = v1[0]*v2[1]-v1[1]*v2[0];
            faces[i].d = -faces[i].a * vertices[i1].x - faces[i].b * vertices[i1].y - faces[i].c * vertices[i1].z;
            //printf("%lf %lf %lf %lf \n",faces[i].a,faces[i].b,faces[i].c,faces[i].d);
        }

    }

};


int main(int argc, char * argv[])
{
    FILE *f = fopen("C:/Users/Mato/Desktop/IRG/labs-irg/tetrahedron.obj.txt", "r");
    if(f == NULL)
    {
        printf("Unable to read file!");
        return -1;
    }
    char line[1024], tip[20];
    vector<Vertex3D> vertices;
    vector<Face3D> faces;

    while(fgets(line, sizeof line, f))
    {
        if(sscanf(line, "%s", tip) == 1)
        {
            string s = tip;
            if(s == "f")
            {
                int i1, i2, i3;
                sscanf(line, "%*s %d %d %d", &i1, &i2, &i3);
                Face3D f;
                f.indexes[0] = --i1;
                f.indexes[1] = --i2;
                f.indexes[2] = --i3;
                faces.push_back(f);
            }
            else if(s == "v")
            {
                double x, y, z;
                sscanf(line, "%*s %lf %lf %lf", &x, &y, &z);
                Vertex3D v;
                v.x = x;
                v.y = y;
                v.z = z;
                vertices.push_back(v);
            }
        }
    }
    ObjectModel obj(vertices, faces);

    //printf("Normirani model:\n\n%s\n", obj.dumpToOBJ().c_str());

    printf("Upiti:\n");
    while(1)
    {
        scanf("%s",tip);
        string s = tip;;
        if(s == "normiraj")
        {
            ObjectModel kopija=obj.copy();
            kopija.normalize();
            printf("Normirani model:\n\n%s\n", kopija.dumpToOBJ().c_str());
        }
        else if(s == "t")
        {
            double x,y,z;
            scanf("%lf %lf %lf", &x, &y, &z);
            int iznad = 0, ispod = 0, na = 0;
            for(int i = 0; i < obj.faces.size(); i++)
            {
                Face3D f = obj.faces[i];
                double r = x * f.a + y * f.b + z * f.c + f.d;
                if(fabs(r) < 1e-7) ++na;
                if(r > 0) ++iznad;
                if(r < 0) ++ispod;
            }
            if(iznad > 0)
            {
                printf("Tocka T(%lf, %lf, %lf) je izvan tijela\n", x, y, z);
            }
            else if(na > 0)
            {
                printf("Tocka T(%lf, %lf, %lf) je na tijelu\n", x, y, z);
            }
            else
            {
                printf("Tocka T(%lf, %lf, %lf) je unutar tijela\n", x, y, z);
            }
        }
        else if(s == "quit")
        {
            printf("Kraj\n");
            break;
        }
    }

    return 0;
}
