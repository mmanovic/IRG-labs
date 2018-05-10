#include "AbstractMatrix.h"
#include "MatrixTransposeView.h"
#include "MatrixSubMatrixView.h"
#include <string>
#include <stdio.h>
#include <math.h>
using namespace std;

IMatrix* AbstractMatrix::nTranspose(bool live)
{
    if (live)
    {
        return new MatrixTransposeView(this);
    }
    else
    {
        return new MatrixTransposeView(this->copy());
    }
}

IMatrix* AbstractMatrix::add(IMatrix* m)
{
    int rows1=this->getRowsCount();
    int cols1=this->getColsCount();
    int rows2=m->getRowsCount();
    int cols2=m->getColsCount();
    if(rows1!=rows2||cols1!=cols2)
    {
        throw "Incompatibile size of matrices.";
    }
    for(int i=0; i<rows1; i++)
    {
        for(int j=0; j<cols1; j++)
        {
            this->set(i,j,this->get(i,j)+m->get(i,j));
        }
    }
    return this;
}

IMatrix* AbstractMatrix::nAdd(IMatrix* m)
{
    return this->copy()->add(m);
}

IMatrix* AbstractMatrix::sub(IMatrix* m)
{
    int rows1=this->getRowsCount();
    int cols1=this->getColsCount();
    int rows2=m->getRowsCount();
    int cols2=m->getColsCount();
    if(rows1!=rows2||cols1!=cols2)
    {
        throw "Incompatibile size of matrices.";
    }
    for(int i=0; i<rows1; i++)
    {
        for(int j=0; j<cols1; j++)
        {
            this->set(i,j,this->get(i,j)-m->get(i,j));
        }
    }
    return this;
}

IMatrix* AbstractMatrix::nSub(IMatrix* m)
{
    return this->copy()->sub(m);
}

string AbstractMatrix::toString()
{
    return this->toString(3);
}

std::string AbstractMatrix::toString(int precision)
{
    string res;
    char buff[50];
    for(int i = 0; i < this->getRowsCount(); i++)
    {
        res += "[";
        for(int j = 0; j < this->getColsCount(); j++)
        {
            sprintf(buff, "%.*lf ", precision, this->get(i, j));
            res += buff;
        }
        res += "]\n";
    }
    return res;
}

IMatrix* AbstractMatrix::nMultiply(IMatrix* m)
{
    if(this->getColsCount() != m->getRowsCount())
        throw "Matrices are incompatibile for product.";
    IMatrix *mat = this->newInstance(this->getRowsCount(), m->getColsCount());
    for(int i = 0; i < this->getRowsCount(); i++)
    {
        for(int j = 0; j < m->getColsCount(); j++)
        {
            double value = 0;
            for(int k = 0; k < this->getColsCount(); k++)
            {
                value += this->get(i,k)*m->get(k,j);
            }
            mat->set(i, j, value);
        }
    }
    return mat;
}

IMatrix* AbstractMatrix::subMatrix(int i,int j,bool live)
{
    if (live)
    {
        return new MatrixSubMatrixView(this, i, j);
    }
    else
    {
        return new MatrixSubMatrixView(this->copy(), i, j);
    }
}

double AbstractMatrix::determinant()
{
    if(this->getColsCount() != this->getRowsCount())
    {
        throw "Matrix must be square.";
    }
    int n=this->getColsCount();
    double result=0;
    if(n==1)
    {
        result = this->get(0,0);
    }
    else if(n==2)
    {
        result = this->get(1,1)*this->get(0,0)-this->get(0,1)*this->get(1,0);
    }
    else
    {
        for(int j=0; j<this->getColsCount(); j++)
        {
            result+=pow(-1,j)*this->get(0,j)*this->subMatrix(0,j,false)->determinant();
        }
    }
    return result;
}

IMatrix* AbstractMatrix::nInvert()
{
    if(this->getColsCount() != this->getRowsCount())
    {
        throw "Inverse exist only for square matrix";
    }
    int n=this->getColsCount();
    double det = this->determinant();
    if(det < 1e-6)
    {
        throw "Determinant is 0.";
    }
    IMatrix *mat = this->newInstance(n,n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            double cofactor = pow(-1,(i+j)%2) * this->subMatrix(j, i, false)->determinant();//true ili false
            mat->set(i, j, cofactor / det);
        }
    }
    return mat;
}
