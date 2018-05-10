#include "Vector.h"
#include <vector>
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
Vector::Vector() {
  readOnly = false;
  dimension = 0;
}

Vector::Vector(vector<double> *el)
{
    dimension=el->size();
    readOnly=false;
    elements=new vector<double>();
    for(int i=0; i<dimension; i++)
    {
        elements->push_back(el->at(i));
    }
}

Vector::Vector(bool readOnly,bool freeDelete,vector<double> *el)
{
    this->readOnly=readOnly;
    dimension=el->size();
    if(freeDelete)
    {
        elements=el;
    }
    else
    {
        elements= new vector<double>();
        for(int i=0; i<dimension; i++)
        {
            elements->push_back(el->at(i));
        }
    }
}
Vector::~Vector()
{
    delete elements;
}

double Vector::get(int n){
    if(n>=dimension||n<0){
        throw "Index is out of range.";
    }
    return elements->at(n);
}

IVector* Vector::set(int n,double value){
    if(readOnly){
        throw "Vector is immutable.";
    }
    if(n>=dimension||n<0){
        throw "Index is out of range.";
    }
    elements->at(n)=value;
    return this;
}
int Vector::getDimension() {
  return dimension;
}

IVector* Vector::copy(){
    //cout << elements->size() <<endl;
    return new Vector(readOnly,false,elements);
}

IVector* Vector::newInstance(int d){
    vector<double> *newVector=new vector<double>(d);
    for(int i=0;i<d;i++){
        newVector->at(i)=0;
    }
    return new Vector(newVector);
}

IVector* Vector::parseSimple(string entry){
    vector<double> *el= new vector<double>;
    string tmp;
    double x;
    int length=entry.size();
    for(int i=0;i<=length;i++){
        if(i==length||entry[i]==' '){
            sscanf(tmp.c_str(),"%lf",&x);
            el->push_back(x);
            tmp="";
        }else{
            tmp+=entry[i];
        }
    }
    return new Vector(false,true,el);
}
