#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include "AbstractMatrix.h"
#include <vector>
#include <string>
#include "AbstractMatrix.h"

using namespace std;

class Matrix: public AbstractMatrix{
private:
    int rows,cols;
    vector<vector<double> > *elements;
public:
    ~Matrix();
    Matrix(int,int);
    Matrix(int,int,vector<vector<double> >*,bool);
    int getRowsCount();
    int getColsCount();
    double get(int,int);
    IMatrix* set(int,int,double);
    IMatrix* copy();
    IMatrix* newInstance(int,int);
    static IMatrix* parseSimple(string);
};
#endif // MATRIX_H_INCLUDED
