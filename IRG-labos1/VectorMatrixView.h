#ifndef VECTORMATRIXVIEW_H_INCLUDED
#define VECTORMATRIXVIEW_H_INCLUDED
#include "IVector.h"
#include "IMatrix.h"
#include "AbstractVector.h"

class VectorMatrixView : public AbstractVector
{
private:
    int dimension;
    bool rowMatrix;
    IMatrix *view;
public:
    VectorMatrixView(IMatrix*);
    double get(int);
    IVector* set(int, double);
    int getDimension();
    IVector* copy();
    IVector* newInstance(int);
};


#endif // VECTORMATRIXVIEW_H_INCLUDED
