#ifndef MATRIXVECTORVIEW_H_INCLUDED
#define MATRIXVECTORVIEW_H_INCLUDED
#include "IVector.h"
#include "IMatrix.h"
#include "AbstractMatrix.h"

class MatrixVectorView : public AbstractMatrix
{
private:
    bool asRowMatrix;
    IVector *view;
public:
    MatrixVectorView(IVector*, bool);
    int getRowsCount();
    int getColsCount();
    double get(int, int);
    IMatrix* set(int, int, double);
    IMatrix* copy();
    IMatrix* newInstance(int, int);
};

#endif // MATRIXVECTORVIEW_H_INCLUDED
