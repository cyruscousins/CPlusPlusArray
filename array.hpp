#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <ostream>

#include <assert.h>
#include <algorithm>
#include <thread>
#include <random>

//This templated array class allows some classic higher order functions, and optionally provides some run time safety with bounds checking.
template <typename T> struct Array {
  //Fields
  unsigned length;
  T* data;
  
  //Constructors
  Array(unsigned length, T* data) : length(length), data(data) { }
  Array(T* data, unsigned length) : length(length), data(data) { }
  
  //Empty array (Warning: No initialization)
  Array(unsigned length) : length(length) {
    data = new T[length];
  }
  
  //Initialize array to value
  Array(unsigned length, T val) : Array(length) {
    for(unsigned i = 0; i < length; i++){
      data[i] = val;
    }
  }
  
  //Copy from a vector
  Array(std::vector<T>& vec){
    length = vec.size();
    data = new T[length];
    arrayCopy(data, vec.data(), length);
  }
  
  void freeMemory(){
    delete [] data;
  }
  
  //Initializationless constructor
  Array(){}

  //Accessors
  T & operator[](unsigned index) const {
    assert(index < length); //The type system takes care of negative indices.
    return data[index];
  }

  //Equality

  bool operator==(const Array<T> other) const {
    if(length != other.length) return false;
    for(unsigned i = 0; i < length; i++){
      if(data[i] != other[i]) return false;
    }
    return true;
  }

  bool operator!=(const Array<T> other) const {
    return !operator==(other);
  }

  //IO
  
  void writeToStream(std::ostream& out) const {
    if(length == 0){
      out << "{}" << std::endl;
      return;
    }
    out << "{" << data[0];
    for(unsigned i = 1; i < length; i++){
      out << ", " << data[i];
    }
    out << "}";
  }

  //Functional Creators
  Array<T> slice(unsigned first) { //Technically this could be const, but such would seem to violate the spirit of the thing.
    assert(first <= length);
    return Array<T>(data + first, length - first);
  }

  //Basically alternative syntax to slice, more C like.
  Array<T> operator+(unsigned addand) {
    return slice(addand);
  }

  //Gives a new array from [first, last)
  Array<T> slice(unsigned first, unsigned last){
    assert(first <= length);
    assert(last <= length);
    return Array<T>(data + first, last - first);
  }

  //Head and tail, to operate like a functional list.
  T head() const {
    assert(length > 0);
    return data[0];
  }

  Array<T> tail() const {
    assert(length > 0);
    return Array(data + 1, length - 1);
  }

  Array<T> take(unsigned count) {
    assert(count <= length);
    return Array<T>(data, count);
  }

  Array<T> drop(unsigned count) {
    assert(count <= length);
    return Array<T>(data + count, length - count);
  }

  //Mutators  

  //Sort the contents of the array using its < operator.
  void sort(){
    std::sort(data, data + length);
  }

  //Shuffle the contents of the array with a random number generator.
  template <class URNG> void shuffle(URNG&& r){
    std::shuffle(data, data + length, r);
  }
  
  //Functional Operators:

  //Predicates

  bool conjunction(bool (*f)(const T)) const{
    for(unsigned i = 0; i < length; i++){
      if(!f(data[i])) return false;
    }
    return true;
  }
  
  bool disjunction(bool (*f)(const T)) const{
    for(unsigned i = 0; i < length; i++){
      if(f(data[i])) return true;
    }
    return false;
  }

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
  
  Array<T> mapInPlace(T (*f)(const T)) {
      mapTo(f, this);
  }
  
  //TODO: Implement maps in terms of mapCls?
  //Would compiler handle it gracefully?
  
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
  
  template<class U> Array<U> mapParallel(U (*f)(const T), unsigned threadCount, unsigned minToMultithread) const {
    //Launch new threads, each doing their own map.
    if(length < minToMultithread){
      return map(f);
    }
    
    Array<U> result = Array<U>(length);

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for(unsigned i = 0; i < threadCount; i++){
      unsigned start = (i * length) / threadCount;
      unsigned finish = ((i + 1) * length) / threadCount;
      unsigned subLen = finish - start;
      
      threads.push_back(std::thread([this, f, start, subLen, result](){Array(data + start, subLen).mapTo(f, Array<U>(result.data + start, subLen));}));
    }

    //TODO have the main thread work too?
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) 
    {
      t.join();
    });

    return result; 
  }

  template<class U> Array<U> mapParallel(U (*f)(const T)) const {
    return mapParallel(f, 8, 16); //Defaults
  }
 
  //For Each
  void forEach(void (*f)(T&)) {
    for(unsigned i = 0; i < length; i++){
      f(data[i]);
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
    //Need a C++ equivalent of this:
    //newArr.data = (T*)realloc(newArr.data, ni);
    return newArr;
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

  //Given a commutative function f, fold a list of A into a single A.
  T foldUnordered(T (*f)(const T t0, const T t1)) const{
    assert(length > 0);
    if(length == 1) return data[0];
    else{
      assert(f(data[0], data[1]) == f(data[1], data[0])); //Assert commutativity (this check is necessary but not sufficient).
      return f(data[0], tail().foldUnordered(f));
    }
  }
  template<typename Closure> T foldUnordered(T (*f)(const T t0, const T t1, const Closure cl), const Closure cl) const{
    assert(length > 0);
    if(length == 1) return data[0];
    else{
      assert(f(data[0], data[1], cl) == f(data[1], data[0], cl)); //Assert commutativity (this check is necessary but not sufficient).
      return f(data[0], tail().foldUnordered(f));
    }
  }
  
  //Zip
  
  template<typename OtherTy, typename ResTy> Array<ResTy> zip(const Array<OtherTy> other, ResTy (*f)(const T, const OtherTy)) const {

    assert(length == other.length);
    
    Array<ResTy> result = Array<ResTy>(length);
    for(unsigned i = 0; i < length; i++){
      result[i] = f(data[i], other[i]);
    }
    
    return result;
  }
  
  template<typename OtherTy, typename ResTy, typename ClosureTy> Array<ResTy> zip(const Array<OtherTy> other, ResTy (*f)(const T, const OtherTy, const ClosureTy), const ClosureTy cl) const {

    assert(length == other.length);
    
    Array<ResTy> result = Array<ResTy>(length);
    for(unsigned i = 0; i < length; i++){
      result[i] = f(data[i], other[i], cl);
    }
    
    return result;
  }

};

template <typename T> std::ostream& operator<<(std::ostream& o, const Array<T>& arr){
  arr.writeToStream(o);
  return o;
}

//Some people want to use these operators and don't like OOP.
template <typename T> T head(const Array<T> arr){
	return arr.head();
}

template <typename T> Array<T> tail(const Array<T> arr){
	return arr.tail();
}

template <typename T> Array<T> slice(Array<T> arr, unsigned s, unsigned e){
	return arr.slice(s, e);
}


//Due to a very interesting design decision, vector<bool> is implemented as a bit array.
typedef std::vector<bool> BitArray;

#endif
