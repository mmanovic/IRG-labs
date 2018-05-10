#ifndef ABSTRACTVECTOR_H_INCLUDED
#define ABSTRACTVECTOR_H_INCLUDED
#include "AbstractVector.h"
#include "IVector.h"
class AbstractVector : virtual public IVector {
  public:
    IVector* copyPart(int);
    IVector* add(IVector*);
    IVector* nAdd(IVector*);
    IVector* sub(IVector*);
    IVector* nSub(IVector*);
    IVector* scalarMultiply(double);
    IVector* nScalarMultiply(double);
    double norm();
    IVector* normalize();
    IVector* nNormalize();
    double cosine(IVector*);
    double scalarProduct(IVector*);
    IVector* nVectorProduct(IVector*);
    //IVector* nFromHomogenus();
    //IMatrix* toRowMatrix(bool);
    //IMatrix* toColumnMatrix(bool);
    //double* toArray();
    std::string toString();
    std::string toString(int);
};

#endif // ABSTRACTVECTOR_H_INCLUDED
