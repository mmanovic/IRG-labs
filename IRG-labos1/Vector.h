#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include <string>
#include <vector>
#include "AbstractVector.h"
#include "Vector.h"

using namespace std;

class Vector: public AbstractVector{
private:
    vector<double> *elements;
    bool readOnly;
    int dimension;
public:
    Vector();
    ~Vector();
    Vector(vector<double> *el);
    Vector(bool, bool, vector<double> *el);
    double get(int);
    IVector* set(int,double);
    int getDimension();
    IVector* copy();
    IVector* newInstance(int);
    static IVector* parseSimple(string);

};


#endif // VECTOR_H_INCLUDED
