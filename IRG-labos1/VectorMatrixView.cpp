#include "VectorMatrixView.h"
#include <algorithm>

using namespace std;

VectorMatrixView::VectorMatrixView(IMatrix *v)
{
    view = v;
    dimension = max(v->getRowsCount(),v->getColsCount());
    if (v->getColsCount() == 1)
    {
        rowMatrix = false;
    }
    else
    {
        rowMatrix = true;
    }
}

double VectorMatrixView::get(int n)
{
    if (!rowMatrix)
    {
        return view->get(n,0);
    }
    else
    {
        return view->get(0,n);
    }
}

IVector* VectorMatrixView::set(int n, double value)
{
    if (!rowMatrix)
    {
        view->set(n,0,value);
    }
    else
    {
        view->set(0,n,value);
    }
    return this;
}

int VectorMatrixView::getDimension()
{
    return dimension;
}
IVector* VectorMatrixView::copy()
{
    return new VectorMatrixView(view->copy());
}

IVector* VectorMatrixView::newInstance(int d)
{
    if (!rowMatrix)
    {
        return new VectorMatrixView(view->newInstance(d,1));
    }
    else
    {
        return new VectorMatrixView(view->newInstance(1,d));
    }
}
