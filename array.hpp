#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

#include <assert.h>
#include <algorithm>
#include <thread>

//This templated array class allows some classic higher order functions, and optionally provides some run time safety with bounds checking.
template <typename T> struct Array {
  //Fields
  unsigned length;
  T* data;
  
  //Constructors
  Array(unsigned length, T* data) : length(length), data(data) { } //This seems backwards.
  Array(T* data, unsigned length) : length(length), data(data) { }
  
  //Empty array (Warning: No initialization)
  Array(unsigned length) : length(length) {
    data = new T[length];
  }
  
  //Copy from a vector
  Array(std::vector<T>& vec){
    length = vec.size();
    data = new T[length];
    arrayCopy(data, vec.data(), length);
  }
  
  void freeData(){
    delete [] data;
  }
  
  //Initializationless constructor
  Array(){}

  //Accessors
  T & operator[](unsigned index) const {
    //#ifdef DEBUG //necessary?
    assert(index < length); //The type system takes care of negative indices.
    //#endif
    return data[index];
  }
  
  /*
  T* operator+(unsigned addand){
      return data + addand;
  }
   */
  
  //Sort
  void sort(){
    std::sort(data, data + length);
  }
  
  //Functional Operators:
  
  ////////////
  //MAP FAMILY
  
  template<class U> Array<U> map( U (*f)(const T)) const{
      Array<U> newArr = Array<U>(length);
      return mapTo(f, newArr);
  }
  
  template<class U> Array<U> mapTo( U (*f)(const T), Array<U> out) const{
      assert(out.length == length);
      for(unsigned i = 0; i < length; i++){
          out[i] = (*f)(data[i]);
      }
      return out;
  }
  
  Array<T> mapInPlace(T (*f)(const T)) const{
      mapTo(f, this);
  }
  
  //TODO: Implement maps in terms of mapCls?
  //Would compiler handle it gracefully?
  
  //TODO: Why are closures always pointers?  Is this the best way to do it?
  template<class U, class V> Array<U> map( U (*f)(const T, const V cl), const V cl) const{
      Array<U> newArr = Array<U>(length);
      return mapTo(f, cl, newArr);
  }
  
  template<class U, class V> Array<U> mapTo( U (*f)(const T, const V cl), const V cl, Array<U> out) const{
      assert(out.length == length);
      for(unsigned i = 0; i < length; i++){
          out[i] = (*f)(data[i], cl);
      }
      return out;
  }
  
  //Parallelized Map
  
  
  template<class U> Array<U> mapParallel(U (*f)(const T)) const {
    //Launch new threads, each doing their own map.
    unsigned threadCount = 8;
    if(length < threadCount){
      return map(f);
    }
    
    Array<U> result = Array<U>(length);
    for(unsigned i = 0; i < threadCount; i++){
      unsigned start = (i * length) / threadCount;
      unsigned finish = ((i + 1) * length) / threadCount;
      unsigned sublen = finish - start;
      
      //TODO: Ship this to a new thread.
      Array(data + start, sublen).mapTo(Array<U>(result.data + start, subLen));
    }
  }
  
  
  ////////////
  //OTHER FUNCTIONAL OPERATORS
  
  Array<T> filter(bool (*f)(const T t)) const{
    Array<T> newArr = Array<T>(length);
    unsigned ni = 0;
    for(unsigned i = 0; i < length; i++){
      if((*f)(data[i])){
        newArr[ni] = data[i];
        ni++;
      }
    }
    newArr.length = ni;
    return newArr;
    //TODO consider a realloc?
  }
  
  template<typename Cl> Array<T> filter(bool (*f)(const T t, const Cl), const Cl cl) const{
    Array<T> newArr = Array<T>(length);
    unsigned ni = 0;
    for(unsigned i = 0; i < length; i++){
      if((*f)(data[i], cl)){
        newArr[ni++] = data[i];
      }
    }
    newArr.length = ni;
    return newArr;
  }
  
  //TODO const higher order functions.
  template<typename ResultTy> ResultTy fold(ResultTy (*f)(const ResultTy zero, const T next), ResultTy zero) const{
    ResultTy acc = zero;
    for(unsigned i = 0; i < length; i++){
      acc = (*f)(acc, data[i]);
    }
    return acc;
  }
 
  template<typename ResultTy, typename ClosureTy> ResultTy fold(ResultTy (*f)(const ResultTy zero, const T next, const ClosureTy closure), const ResultTy zero, const ClosureTy cl) const{
    ResultTy acc = zero;
    for(unsigned i = 0; i < length; i++){
      acc = (*f)(acc, data[i], cl);
    }
    return acc;
  }
};

//Due to a very interesting design decision, vector<bool> is implemented as a bit array.
typedef std::vector<bool> BitArray;

#endif
