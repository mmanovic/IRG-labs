#ifndef ABSTRACTMATRIX_H_INCLUDED
#define ABSTRACTMATRIX_H_INCLUDED
#include "IMatrix.h"
#include <string>

using namespace std;

class AbstractMatrix: virtual public IMatrix{
public:
    IMatrix* nTranspose(bool);
    IMatrix* add(IMatrix*);
    IMatrix* nAdd(IMatrix*);
    IMatrix* sub(IMatrix*);
    IMatrix* nSub(IMatrix*);
    IMatrix* nMultiply(IMatrix*);
    double determinant();
    IMatrix* subMatrix(int,int,bool);
    IMatrix* nInvert();
    string toString();
    string toString(int);

};

#endif // ABSTRACTMATRIX_H_INCLUDED
