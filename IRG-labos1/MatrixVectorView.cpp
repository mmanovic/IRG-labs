#include "MatrixVectorView.h"
#include "Matrix.h"
using namespace std;

MatrixVectorView::MatrixVectorView(IVector *v, bool isRow)
{
    asRowMatrix = isRow;
    view = v;
}

int MatrixVectorView::getRowsCount()
{
    if (asRowMatrix)
    {
        return 1;
    }
    else
    {
        return view->getDimension();
    }
}

int MatrixVectorView::getColsCount()
{
    if (!asRowMatrix)
    {
        return 1;
    }
    else
    {
        return view->getDimension();
    }
}

double MatrixVectorView::get(int i, int j)
{
    if(asRowMatrix)
    {
        if(i != 0||j < 0 || j >= this->getColsCount())
            throw "Index out of bounds.";
        return this->view->get(j);
    }
    else
    {
        if(j != 0||i < 0 || i >= this->getRowsCount())
            throw "Index out of bounds.";
        return this->view->get(i);
    }
}

IMatrix* MatrixVectorView::set(int i, int j, double val)
{
    if(i < 0 || i >= this->getRowsCount()||j < 0 || j >= this->getColsCount())
        throw "Index out of bounds";
    if(asRowMatrix)
    {
        this->view->set(j, val);
    }
    else
    {
        this->view->set(i, val);
    }
    return this;
}

IMatrix* MatrixVectorView::copy()
{
    return new MatrixVectorView(view->copy(), asRowMatrix);
}

IMatrix* MatrixVectorView::newInstance(int rows, int cols)
{
    return new Matrix(rows, cols);
}
