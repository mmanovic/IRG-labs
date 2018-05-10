#include "MatrixTransposeView.h"

MatrixTransposeView::MatrixTransposeView(IMatrix* m){
    mat=m;
}

int MatrixTransposeView::getRowsCount() {
  return mat->getColsCount();
}

int MatrixTransposeView::getColsCount() {
  return mat->getRowsCount();
}

double MatrixTransposeView::get(int i, int j) {
  return mat->get(j ,i);
}

IMatrix* MatrixTransposeView::set(int i, int j, double value) {
  return mat->set(j, i, value);
}

IMatrix* MatrixTransposeView::copy() {
  return new MatrixTransposeView(mat->copy());

}

IMatrix* MatrixTransposeView::newInstance(int i, int j) {
  return new MatrixTransposeView(mat->newInstance(j, i));
}
