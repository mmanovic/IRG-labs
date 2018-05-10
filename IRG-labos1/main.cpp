#include <iostream>
#include "IVector.h"
#include "Vector.h"
#include "Matrix.h"
#include "IMatrix.h"
#include <stdio.h>
#include <string>
using namespace std;
void first()
{
    IVector* a=Vector::parseSimple("1 2 4");
    IVector* b=Vector::parseSimple("2 1 2");
    double x=a->add(Vector::parseSimple("1 0 -2"))->scalarProduct(b);
    cout<<x;
    cout <<"\n";
    IMatrix* m1 = Matrix::parseSimple ("1 2 3 | 4 5 6");
    IMatrix* m2 = m1->nTranspose(true);
    cout << m1->toString();
    cout <<"\n";
    cout << m2->toString();
    cout <<"\n";
    m2->set(2,1,9);
    cout << m1->toString();
    cout <<"\n";
    cout << m2->toString(5);
    cout <<"\n";
}
void second()
{
    IVector* a=Vector::parseSimple("1 0 0");
    IVector* b=Vector::parseSimple("5 0 0");
    IVector* c=Vector::parseSimple("3 8 0");

    IVector* t=Vector::parseSimple("3 4 0");

    double pov=b->nSub(a)->nVectorProduct(c->nSub(a))->norm()/2.0;
    double povA=b->nSub(t)->nVectorProduct(c->nSub(t))->norm()/2.0;
    double povB=a->nSub(t)->nVectorProduct(c->nSub(t))->norm()/2.0;
    double povC=a->nSub(t)->nVectorProduct(b->nSub(t))->norm()/2.0;

    double t1=povA/pov;
    double t2=povB/pov;
    double t3=povC/pov;

    printf("baricentricne koordinate su %f %f %f",t1,t2,t3);
}

void third()
{
    // rjesavanje sustava jednadzbi
    IMatrix* a = Matrix::parseSimple("3 5 | 2 10");
    IMatrix* r = Matrix::parseSimple("2 | 8");
    cout << a->toString() <<endl;
    cout << r->toString() <<endl;
    cout << a->nInvert()->toString() <<endl;
    IMatrix* v = a->nInvert()->nMultiply(r);
    printf("Rjesenje sustava je:\n%s", v->toString().c_str());
    return;
}

void fourth()
{
    IMatrix* x=Matrix::parseSimple("1 5 3 | 0 0 8 | 1 1 1");
    IMatrix* y=Matrix::parseSimple("3 | 4 | 1");

    IMatrix* result=x->nInvert()->nMultiply(y);
    printf("Baricentricne koordinate su:\n%s", result->toString().c_str());
}

void fifth()
{
    string parse;
    printf("Unesite vektor n: \n");
    getline(cin, parse);
    IVector* n = Vector::parseSimple(parse);
    printf("Unesite vektor m: \n");
    getline(cin, parse);
    IVector* m = Vector::parseSimple(parse);
    IVector* tmp=n->nNormalize();

    IVector* result=n->nNormalize()->nScalarMultiply(m->scalarProduct(n->nNormalize()))->nScalarMultiply(2)->nSub(m);
    printf("\nReflektirani vektor je:\n%s", result->toString().c_str());
}
void demo1()
{
    IVector* v1 = Vector::parseSimple("2 3 -4")->add(Vector::parseSimple("-1 4 -3"));
    double s = v1->scalarProduct(Vector::parseSimple("-1 4 -3"));
    IVector* v2 = v1->nVectorProduct(Vector::parseSimple("2 2 4"));
    IVector* v3 = v2->nNormalize();
    IVector* v4 = v2->nScalarMultiply(-1);

    IMatrix* m1 = Matrix::parseSimple("1 2 3 | 2 1 3 | 4 5 1")->add(Matrix::parseSimple("-1 2 -3 | 5 -2 7 | -4 -1 3"));
    IMatrix* m2 = Matrix::parseSimple("1 2 3 | 2 1 3 | 4 5 1")->nMultiply(Matrix::parseSimple("-1 2 -3 | 5 -2 7 | -4 -1 3")->nTranspose(true));
    IMatrix* m3 = Matrix::parseSimple("-24 18 5 | 20 -15 -4 | -5 4 1")->nInvert()->nMultiply(Matrix::parseSimple("1 2 3 | 0 1 4 | 5 6 0")->nInvert());

    cout << v1->toString() << endl;
    cout << s << endl;
    cout << v2->toString() << endl;
    cout << v3->toString() << endl;
    cout << v4->toString() << endl;

    cout << m1->toString() << endl;
    cout << m2->toString() << endl;
    cout << m3->toString() << endl;
}

void demo2()
{
    double x, y, z, t;
    IMatrix* a = new Matrix(3, 3);
    IMatrix* b = new Matrix(3, 1);
    for (int i = 0; i < 3; i++)
    {
        scanf("%lf %lf %lf %lf", &x, &y, &z, &t);
        a->set(i, 0, x);
        a->set(i, 1, y);
        a->set(i, 2, z);
        b->set(i, 0, t);
    }
    cout << a->toString() << endl;
    cout << b->toString() << endl;
    cout << a->nInvert()->nMultiply(b)->toString() << endl;
    return;
}
void demo3()
{
    string parseA, parseB, parseC, parseT;
    printf("Tocka A:\n");
    getline(cin, parseA);
    printf("Tocka B:\n");
    getline(cin, parseB);
    printf("Tocka C:\n");
    getline(cin, parseC);
    printf("Tocka T:\n");
    getline(cin, parseT);

    IVector* a  = Vector::parseSimple(parseA);
    IVector* b  = Vector::parseSimple(parseB);
    IVector* c  = Vector::parseSimple(parseC);
    IVector* t  = Vector::parseSimple(parseT);
    double pov=b->nSub(a)->nVectorProduct(c->nSub(a))->norm()/2.0;
    double povA=b->nSub(t)->nVectorProduct(c->nSub(t))->norm()/2.0;
    double povB=a->nSub(t)->nVectorProduct(c->nSub(t))->norm()/2.0;
    double povC=a->nSub(t)->nVectorProduct(b->nSub(t))->norm()/2.0;

    double t1=povA/pov;
    double t2=povB/pov;
    double t3=povC/pov;

    printf("baricentricne koordinate su %f %f %f",t1,t2,t3);
}
int main()
{
    demo1();
}
