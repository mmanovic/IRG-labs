#include "MatrixSubMatrixView.h"
#include <vector>
using namespace std;

MatrixSubMatrixView::MatrixSubMatrixView(IMatrix* m, int x, int y)
{
    mat=m;
    for (int j = 0; j < m->getColsCount(); j++)
    {
        if (j!=y)
        {
            colIndexes.push_back(j);
        }
    }
    for (int i = 0; i < m->getRowsCount(); i++)
    {
        if (i!=x)
        {
            rowIndexes.push_back(i);
        }
    }
}

MatrixSubMatrixView::MatrixSubMatrixView(IMatrix* m, vector<int> rows,vector<int> cols)
{
    mat = m;
    rowIndexes = rows;
    colIndexes = cols;
}

int MatrixSubMatrixView::getRowsCount()
{
    return rowIndexes.size();
}

int MatrixSubMatrixView::getColsCount()
{
    return colIndexes.size();
}
double MatrixSubMatrixView::get(int i,int j)
{
    if(i<0||i>=rowIndexes.size()||j<0||j>=colIndexes.size())
    {
        throw "Position of element at matrix is out of bounds.";
    }
    return mat->get(rowIndexes.at(i),colIndexes.at(j));
}

IMatrix* MatrixSubMatrixView::set(int i,int j,double value)
{
    if(i<0||i>=rowIndexes.size()||j<0||j>=colIndexes.size())
    {
        throw "Position of element at matrix is out of bounds.";
    }
    return mat->set(rowIndexes.at(i),colIndexes.at(j),value);
}
IMatrix* MatrixSubMatrixView::newInstance(int i, int j)
{
    return this->mat->newInstance(i,j);
}

IMatrix* MatrixSubMatrixView::copy()
{
    return new MatrixSubMatrixView(mat->copy(), rowIndexes, colIndexes);
}
IMatrix* MatrixSubMatrixView::subMatrix(int x,int y,bool live)
{
    vector< int > rows;
    vector< int > cols;
    for (int i=0; i<rowIndexes.size(); i++)
    {
        if (i!=x)
        {
            rows.push_back(i);
        }
    }

    for (int j=0; j<colIndexes.size(); j++)
    {
        if (j!=y)
        {
            rows.push_back(j);
        }
    }

    if (live)
    {
        return new MatrixSubMatrixView(mat,rows,cols);
    }
    else
    {
        return new MatrixSubMatrixView(mat->copy(),rows,cols);
    }

}
