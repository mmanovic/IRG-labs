#include "Matrix.h"
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

int Matrix::getRowsCount()
{
    return this->rows;
}
int Matrix::getColsCount()
{
    return this->cols;
}
Matrix::~Matrix()
{
    delete elements;
}
Matrix::Matrix(int r,int c)
{
    rows=r;
    cols=c;
    elements = new vector< vector< double > >(r, vector< double >(c));
}
Matrix::Matrix(int r,int c,vector<vector<double > > *el, bool deleteFlag)
{
    rows=r;
    cols=c;
    if(deleteFlag)
    {
        elements=el;
    }
    else
    {
        elements = new vector< vector< double > > (r, vector< double>(c));
        for(int i=0; i<r; i++)
        {
            for(int j=0; j<c; j++)
            {
                elements->at(i).at(j)=el->at(i).at(j);
            }
        }
    }
}

double Matrix::get(int r,int c)
{
    if(r<0||r>=rows||c>=cols||c<0)
    {
        throw "Position of element is out of range.";
    }
    return elements->at(r).at(c);
}

IMatrix* Matrix::set(int r,int c,double value)
{
    if(r<0||r>=rows||c>=cols||c<0)
    {
        throw "Position of element is out of range.";
    }
    elements->at(r).at(c)=value;
    return this;
}

IMatrix* Matrix::copy() {
  IMatrix* m = new Matrix(rows, cols, this->elements, false);
  return m;
}

IMatrix* Matrix::newInstance(int r, int c) {
  return new Matrix(r, c);
}

IMatrix* Matrix::parseSimple(string s){
    vector<vector<double > > *el = new vector<vector<double > >();
    vector<string> tokens;
    string tmp;
    int r,c;
    for(int i=0;i<=s.length();i++){
        if(s[i]=='|'||i==s.length()){
            tokens.push_back(tmp);
            tmp="";
        }else{
            tmp+=s[i];
        }
    }
    for(int i=0;i<tokens.size();i++){
        if(tokens[i][0]==' '){
            tokens[i]=tokens[i].substr(1);
        }
        int limit=tokens[i].length();
        if(tokens[i][limit-1]==' '){
            limit--;
        }
        tmp="";
        vector<double> row;
        for(int j=0;j<=limit;j++){
            if(j==limit||tokens[i][j]==' '){
                double x;
                sscanf(tmp.c_str(),"%lf",&x);
                tmp="";
                row.push_back(x);
            }else{
                tmp+=tokens[i][j];
            }
        }
        c=row.size();
        el->push_back(row);
    }
    r=el->size();
    return new Matrix(r,c,el,true);
}
