#ifndef IVECTOR_H_INCLUDED
#define IVECTOR_H_INCLUDED
#include <string>
using namespace std;
class IVector
{
public:
    virtual double get(int)=0;
    virtual IVector* set(int,double)=0;
    virtual int getDimension()=0;
    virtual IVector* copy()=0;
    virtual IVector* copyPart(int)=0;
    virtual IVector* newInstance(int)=0;
    virtual IVector* add(IVector*)=0;
    virtual IVector* nAdd(IVector*)=0;
    virtual IVector* sub(IVector*)=0;
    virtual IVector* nSub(IVector*)=0;
    virtual IVector* scalarMultiply(double)=0;
    virtual IVector* nScalarMultiply(double)=0;
    virtual double norm()=0;
    virtual IVector* normalize()=0;
    virtual IVector* nNormalize()=0;
    virtual double cosine(IVector*)=0;
    virtual double scalarProduct(IVector*)=0;
    virtual IVector* nVectorProduct(IVector*)=0;
    //virtual *IVector nFromHomogeneus()=0;
    //virtual *IMatrix toRowMatrix(bool)=0;
    //virtual *IMatrix toColumnMatrix(bool)=0;
    //virtual *double toArray()=0;
    virtual string toString()=0;
    virtual string toString(int)=0;

};

#endif // IVECTOR_H_INCLUDED
