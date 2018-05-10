#include <iostream>
#include <vector>
#include "Vector.h"
#include "IVector.h"
#include <algorithm>
#include <string>
#include <stdio.h>
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

    }

    void izracunajKoeficijente()
    {
        for(int i = 0; i < (int)faces.size(); ++i)
        {
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
            IVector *v11 = new Vector();
            IVector *v22 = new Vector();
            IVector *n = v11->nVectorProduct(v22);
            faces[i].a = n->get(0);
            faces[i].b = n->get(1);
            faces[i].c = n->get(2);
            faces[i].d = -faces[i].a * vertices[i1].x - faces[i].b * vertices[i1].y - faces[i].c * vertices[i1].z;
        }

    }

};


int main(int argc, char * argv[])
{
    FILE *f = fopen("C:/Users/Mato/Desktop/IRG/labs-irg/kocka.obj.txt", "r");
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

    printf("Normirani model:\n\n%s\n", obj.dumpToOBJ().c_str());

    return 0;
}
