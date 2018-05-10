#ifndef MATRIXTRANSPOSEVIEW_H_INCLUDED
#define MATRIXTRANSPOSEVIEW_H_INCLUDED
#include "AbstractMatrix.h"
#include <vector>
#include <string>

using namespace std;

class MatrixTransposeView:public AbstractMatrix{
private:
    IMatrix* mat;
public:
    MatrixTransposeView(IMatrix*);
    int getRowsCount();
    int getColsCount();
    double get(int,int);
    IMatrix* set(int,int,double);
    IMatrix* copy();
    IMatrix* newInstance(int,int);

};


#endif // MATRIXTRANSPOSEVIEW_H_INCLUDED
