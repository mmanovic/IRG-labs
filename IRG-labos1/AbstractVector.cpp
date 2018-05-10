#include "AbstractVector.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
using namespace std;
IVector* AbstractVector::copyPart(int n)
{
    IVector *newV=this->newInstance(n);
    int limit=min(this->getDimension(),n);
    for(int i=0; i<limit; i++)
    {
        newV->set(i,this->get(i));
    }
    return newV;
}

IVector* AbstractVector::add(IVector* other)
{
    if(this->getDimension()!=other->getDimension())
    {
        throw "Incompatibile size of vectors.";
    }
    for(int i=0; i<this->getDimension(); i++)
    {
        this->set(i,this->get(i)+other->get(i));
    }
    return this;
}

IVector* AbstractVector::nAdd(IVector* other)
{
    IVector* newV=this->copy();
    return newV->add(other);
}

IVector* AbstractVector::sub(IVector* other)
{
    if(this->getDimension()!=other->getDimension())
    {
        throw "Incompatibile size of vectors.";
    }
    for(int i=0; i<this->getDimension(); i++)
    {
        this->set(i,this->get(i)-other->get(i));
    }
    return this;
}

IVector* AbstractVector::nSub(IVector* other)
{
    IVector* newV=this->copy();
    return newV->sub(other);
}

IVector* AbstractVector::scalarMultiply(double x)
{
    for (int i = 0; i < this->getDimension(); i++)
    {
        this->set(i, this->get(i)*x);
    }
    return this;
}

IVector* AbstractVector::nScalarMultiply(double x)
{
    IVector* newV = this->copy();
    newV->scalarMultiply(x);
    return newV;
}

double AbstractVector::norm()
{
    double result=0;
    for(int i=0; i<this->getDimension(); i++)
    {
        result+=(this->get(i))*(this->get(i));
    }
    return sqrt(result);
}

IVector* AbstractVector::normalize()
{
    double norm=this->norm();
    for(int i=0; i<this->getDimension(); i++)
    {
        this->set(i,this->get(i)/norm);
    }
    return this;
}

IVector* AbstractVector::nNormalize()
{
    double norm=this->norm();
    IVector *newV=this->newInstance(this->getDimension());
    for(int i=0; i<this->getDimension(); i++)
    {
        newV->set(i,this->get(i)/norm);
    }
    return newV;
}

double AbstractVector::scalarProduct(IVector* other)
{
    if(other->getDimension()!=this->getDimension())
    {
        throw "Incompatibile size of vectors.";
    }
    double result=0;
    for(int i=0; i<this->getDimension(); i++)
    {
        result+=this->get(i)*other->get(i);
    }
    return result;
}

double AbstractVector::cosine(IVector* other)
{
    return this->scalarProduct(other)/(this->norm()*other->norm());
}

IVector* AbstractVector::nVectorProduct(IVector* other)
{
    if(other->getDimension()!=this->getDimension())
    {
        throw "Incompatibile size of vectors.";
    }
    if(this->getDimension()!=3)
    {
        throw "Vector product is defined for dimension which equals 3.";
    }
    IVector *newV=this->newInstance(3);
    double i = this->get(1) * other->get(2) - this->get(2) * other->get(1);
    double j = this->get(2) * other->get(0) - this->get(0) * other->get(2);
    double k = this->get(0) * other->get(1) - this->get(1) * other->get(0);
    newV->set(0, i);
    newV->set(1, j);
    newV->set(2, k);
    return newV;
}

string AbstractVector::toString()
{
    return this->toString(3);
}

string AbstractVector::toString(int precision)
{
    string s;
    char buff[50];
    s += "[";
    for(int i = 0; i < this->getDimension(); i++)
    {
        sprintf(buff, "%.*lf ", precision, this->get(i));
        s += buff;
    }
    s += "]";
    return s;
}
