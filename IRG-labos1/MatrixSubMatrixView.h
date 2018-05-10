#ifndef MATRIXSUBMATRIXVIEW_H_INCLUDED
#define MATRIXSUBMATRIXVIEW_H_INCLUDED
#include "AbstractMatrix.h"
#include <vector>
#include <string>

using namespace std;

class MatrixSubMatrixView: public AbstractMatrix{
private:
    IMatrix* mat;
    vector<int> rowIndexes;
    vector<int> colIndexes;
    MatrixSubMatrixView(IMatrix*,vector<int>,vector<int>);
public:
    MatrixSubMatrixView(IMatrix*,int,int);
    int getRowsCount();
    int getColsCount();
    double get(int,int);
    IMatrix* set(int,int,double);
    IMatrix* copy();
    IMatrix* newInstance(int,int);
    IMatrix* subMatrix(int,int,bool);
};


#endif // MATRIXSUBMATRIXVIEW_H_INCLUDED
