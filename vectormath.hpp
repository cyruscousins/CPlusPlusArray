//Templated vector math
//Provides functions to deal with common operations on vectors, including basic statistics and vector math.

//May be used with the Array type or with raw pointers and lengths.

//Cyrus Cousins

#ifndef VMATH_H
#define VMATH_H

#include <cmath>
#include <assert.h>

#include "array.hpp"

///////
//MAX//
///////

//Returns an index of a maximally valued T in data.  Requires > defined.
template<typename T> unsigned maxIndex(T* data, unsigned len){
  int maxIndex = 0;
  for(unsigned i = 1; i < len; i++){
    if(data[i] > data[maxIndex]){
      maxIndex = i;
    }
  }
  return maxIndex;
}
template<typename T> unsigned maxIndex(Array<T> arr){
  return maxIndex(arr.data, arr.length);
}

//Returns the maximum value in T.  Requires > defined.
template<typename T> T max(T* data, unsigned len){
  return data[maxIndex<T>(data, len)];
}
template<typename T> unsigned max(Array<T> arr){
  return max(arr.data, arr.length);
}

//Complicated function.  Returns the index of the maximum value if the maximum is unique.
//Otherwise, it returns the - of a maximal index.
//Requires > defined.
template<typename T> unsigned maxUniqueIndex(T* data, unsigned len){
  unsigned maxIndex = 0;
  bool tied = false;
  for(unsigned i = 1; i < len; i++){
    if(data[i] > data[maxIndex]){
      maxIndex = i;
      tied = false;
    }
    else if(data[i] == data[maxIndex]){
      tied = true;
    }
  }
  if(tied) return -maxIndex;
  else return maxIndex;
}
template<typename T> unsigned maxUniqueIndex(Array<T> arr){
  return maxUniqueIndex(arr.data, arr.length);
}

///////
//MIN//
///////

//Returns a maximal index in data.  Requires < defined.
template<typename T> unsigned minIndex(T* data, unsigned len){
  int mi = 0;
  for(unsigned i = 1; i < len; i++){
    if(data[i] < data[mi]) mi = i;
  }
  return mi;
}
template<typename T> unsigned minIndex(Array<T> arr){
  return minIndex(arr.data, arr.len);
}

//Returns the minimum value in data.  Requires < defined.
template<typename T> T min(T* data, unsigned len){
  return data[minIndex<T>(data, len)];
}
template<typename T> unsigned min(Array<T> arr){
  return min(arr.data, arr.len);
}

////////////
//DISTANCE//
////////////

//Note, T must be a scalar type for this to be logical.
//Returns euclidean distance squared, requires - and * defined.
template<typename T> T distanceSquared(T* d0, T* d1, unsigned len){
  T ds = 0;
  for(unsigned i = 0; i < len; i++){
    ds += (d0[i] - d1[i]) * (d0[i] - d1[i]);
  }
  return ds;
}
template<typename T> T distanceSquared(Array<T> arr0, Array<T> arr1){
  assert(arr0.length == arr1.length); //Arrays must be identically sized.
  return distanceSquared(arr0.data, arr1.data, arr0.length);
}

//Returns euclidean distance, requires -, * and sqrt(T) defined.
template<typename T> T distance(T* t0, T* t1, unsigned len){
  return sqrt(distanceSquared<T>(t0, t1, len));
}
template<typename T> T distance(Array<T> arr0, Array<T> arr1){
  assert(arr0.length == arr1.length); //Arrays must be identically sized.
  return distance(arr0.data, arr1.data, arr0.length);
}

//Preweights each component of the distance
template<typename T> T distanceWeightedSquared(T* d0, T* d1, T* w, unsigned len){
  T ds = 0;
  for(unsigned i = 0; i < len; i++){
    ds += (d0[i] - d1[i]) * (d0[i]- d1[i]) * w[i];
  }
  return ds;
}
template<typename T> T distanceWeightedSquared(Array<T> arr0, Array<T> arr1, Array<T> weights){
  assert(arr0.length == arr1.length && arr1.length == weights.length); //Arrays must be identically sized.
  return distanceWeightedSquared(arr0.data, arr1.data, weights.data, arr0.length);
}

//Returns preweighted euclidean distance.
template<typename T> T distanceWeighted(T* d0, T* d1, T* w, unsigned len){
  return sqrt(distanceWeightedSquared(d0, d1, w, len));
}
template<typename T> T distanceWeighted(Array<T> arr0, Array<T> arr1, Array<T> weights){
  assert(arr0.length == arr1.length && arr1.length == weights.length); //Arrays must be identically sized.
  return distanceWeighted(arr0.data, arr1.data, weights.data, arr0.length);
}

//Returns distance squared, ignoring elements where w[i] = false.
template<typename T> T distanceSwitchedSquared(T* d0, T* d1, bool* w, unsigned len){
  T ds = 0;
  for(unsigned i = 0; i < len; i++){
    if(w[i]) ds += (d0[i] - d1[i]) * (d0[i]- d1[i]);
  }
  return ds;
}
template<typename T> T distanceSwitchedSquared(Array<T> arr0, Array<T> arr1, Array<bool> switches){
  assert(arr0.length == arr1.length && arr1.length == switches.length); //Arrays must be identically sized.
  return distanceSwitchedSquared(arr0.data, arr1.data, switches.data, arr0.length);
}

//Returns distance, ignoring elements where w[i] = false.
template<typename T> T distanceSwitched(T* d0, T* d1, bool* w, unsigned len){
  return sqrt(distanceSwitchedSquared(d0, d1, w, len));
}
template<typename T> T distanceSwitched(Array<T> arr0, Array<T> arr1, Array<bool> switches){
  assert(arr0.length == arr1.length && arr1.length == switches.length); //Arrays must be identically sized.
  return distanceSwitched(arr0.data, arr1.data, switches.data, arr0.length);
}

////////////////
//BASIC VECTOR//
////////////////

//Sum calculation on arbitrary T.
//Requires addition and identity.
template<typename T> T sumTerms(T* data, unsigned len){ 
  T result = 0;
  for(unsigned i = 0; i < len; i++){
    result += data[i];
  }
  return result;
}
template<typename T> T sumTerms(Array<T> arr){
  return sumTerms(arr.data, arr.length);
}

//Product calculation on arbitrary T.
//Requires product and identity.
template<typename T> T productTerms(T* data, unsigned len){
  T result = 1;
  for(unsigned i = 0; i < len; i++){
    result *= data[i];
  }
  return result;
}
template<typename T> T productTerms(Array<T> arr){
  return productTerms(arr.data, arr.length);
}

//Returns the l1 norm of a vector of arbitrary T
template<typename T> void l1Norm(T* data, unsigned len){
  T val = 0;
  for(unsigned i = 0; i < len; i++){
    val += (data[i] >= 0) ? data[i] : -data[i];
  }
  return val;
}
template<typename T> T l1Norm(Array<T> arr){
  return l1Norm(arr.data, arr.length);
}

//Returns the l2 norm of a vector of arbitrary T.
//Requires that sqrt is defined on T.
//May be succeptible to numerics and overflow issues on narrow types with large values.
template<typename T> void l2Norm(T* data, unsigned len){
  T sumSqrs = 0;
  for(unsigned i = 0; i < len; i++){
    sumSqrs += data[i] * data[i];
  }
  return (T)sqrt(sumSqrs);
}
template<typename T> T l2Norm(Array<T> arr){
  return l2Norm(arr.data, arr.length);
}

//Returns the l infinity norm (or sup norm if you prefer) of a vector of arbitrary T.
template<typename T> void lInfNorm(T* data, unsigned len){
  double norm = 0;
  for(unsigned i = 0; i < len; i++){
    double thisVal = (data[i] >= 0) ? data[i] : -data[i];
    if(thisVal > norm) norm = thisVal;
  }
  return norm;
}
template<typename T> T lInfNorm(Array<T> arr){
  return lInfNorm(arr.data, arr.length);
}

#define LN_2 0.69314718055994528622676398299518041312694549560546875
#define INV_LN_2 (1.0 / 0.69314718055994528622676398299518041312694549560546875)
//Calculates the entropy of a vector.  Has the requirement that log be defined (natural logarithm), and that the input vector sums to 1 and is strictly positive.
template<typename T> T entropyStrictPositive(T* data, unsigned len){
  double entropy = 0;
  for(unsigned i = 0; i < len; i++){
    entropy += -data[i] * log(data[i]);// * INV_LN_2;
  }
  return entropy * INV_LN_2; //Want natural log above, rather than multiplying by a constant for every term just multiply out here.
}
template<typename T> T entropyStrictPositive(Array<T> arr){
  return entropyStrictPositive(arr.data, arr.length);
}
//Calculates the entropy of a vector.  Has the requirement that log be defined (natural logarithm), and that the input vector sums to 1 and is nonnegative.
template<typename T> T entropy(T* data, unsigned len){
  double entropy = 0;
  for(unsigned i = 0; i < len; i++){
    entropy += (data[i] <= 0) ? 0 : (-data[i] * log(data[i])/* * INV_LN_2 */);
  }
  return entropy * INV_LN_2;
}
template<typename T> T entropy(Array<T> arr){
  return entropy(arr.data, arr.length);
}

/////////////////////////
//VECTOR TRANSFORMATION//
/////////////////////////

//Scalar multiplication on a vector.
//Requires * defined
template<typename T> void scalarMultiplyInPlace(T* data, unsigned len, T scalar){
  for(unsigned i = 0; i < len; i++){
    data[i] *= scalar;
  }
}
template<typename T> void scalarMultiplyInPlace(Array<T> arr){
  scalarMultiplyInPlace(arr.data, arr.length);
}

//Scales a nonzero vector of finite values such that the L2 norm of the vector is equal to 0.
//Requires *, +, sqrt defined and additive identity.
template<typename T> void normalizeVectorInPlace(T* data, unsigned len){
  scalarMultiplyInPlace<T>(data, len, l2Norm(data, len));
}
template<typename T> void normalizeVectorInPlace(Array<T> arr){
  normalizeVectorInPlace(arr.data, arr.length);
}

//Scales a nonzero vector of finite values such that the L2 norm of the vector is equal to val.
//Requires +, *, / defined and additive, multiplicative identities.
template<typename T> void normalizeVectorSumToValInPlace(T* data, unsigned len, T val){
  scalarMultiplyInPlace<T>(data, len, val / sumTerms(data, len)); 
}
template<typename T> void normalizeVectorSumToValInPlace(Array<T> arr, T val){
  normalizeVectorSumToValInPlace(arr.data, arr.length, val);
}

//Scales a nonzero vector of finite values such that the L2 norm of the vector is equal to 1.
//Requires +, *, / defined and additive, multiplicative identities.
template<typename T> void normalizeVectorSumToOneInPlace(T* data, unsigned len){
  normalizeVectorSumToValInPlace<T>(data, len, (T)1);
}
template<typename T> void normalizeVectorSumToOneInPlace(Array<T> arr){
  normalizeVectorSumToOneInPlace(arr.data, arr.length);
}

//////////////
//STATISTICS//
//////////////

//Mean calculation on arbitrary T.
//Requires division, conversion from integer, and addition.
template<typename T> T mean(T* data, unsigned len){
  T result = sumTerms<T>(data, len);
  return result / len;
}
template<typename T> T mean(Array<T> arr){
  return mean(arr.data, arr.length);
}

//Variance calculation on arbitrary T.
//Requires division, conversion from integer, subtraction, and multiplication defined on T.
template<typename T> T variance(T* data, T mean, unsigned len){
  T result = 0;
  for(unsigned i = 0; i < len; i++){
    result += (data[i] - mean) * (data[i] - mean);
  }
  return result / (len - 1);
}
template<typename T> T variance(Array<T> arr, T mean){
  return variance(arr.data, mean, arr.length);
}

template<typename T> T variance(T* data, unsigned len){
  return variance<T>(data, mean<T>(data, len), len);
}
template<typename T> T variance(Array<T> arr){
  return variance(arr.data, arr.length);
}

//Stdev calculation on arbitrary T.
//Requires +, -, *, /, conversion from integer, and sqrt(T) all defined.
template<typename T> T stdev(T* data, T mean, unsigned len){
  return (T)sqrt(variance<T>(data, mean, len));
}
template<typename T> T stdev(Array<T> arr, T mean){
  return stdev(arr.data, mean, arr.length);
}

template<typename T> T stdev(T* data, int len){
  return (T)sqrt(variance<T>(data, len));
}
template<typename T> T stdev(Array<T> arr){
  return stdev(arr.data, arr.length);
}


//Biased variance calculation on arbitrary T.
//Requires division, conversion from integer, subtraction, and multiplication defined on T.
template<typename T> T varianceBiased(T* data, T mean, unsigned len){
  T result = 0;
  for(unsigned i = 0; i < len; i++){
    result += (data[i] - mean) * (data[i] - mean);
  }
  return result / len;
}
template<typename T> T varianceBiased(Array<T> arr, T mean){
  return varianceBiased(arr.data, mean, arr.length);
}

template<typename T> T varianceBiased(T* data, unsigned len){
  return varianceBiased<T>(data, mean<T>(data, len), len);
}
template<typename T> T varianceBiased(Array<T> arr){
  return varianceBiased(arr.data, arr.length);
}

//Stdev calculation on arbitrary T.
//Requires +, -, *, /, conversion from integer, and sqrt(T) all defined.
template<typename T> T stdevBiased(T* data, T mean, unsigned len){
  return (T)sqrt(varianceBiased<T>(data, mean, len));
}
template<typename T> T stdevBiased(Array<T> arr, T mean){
  return stdevBiased(arr.data, mean, arr.length);
}

template<typename T> T stdevBiased(T* data, unsigned len){
  return (T)sqrt(varianceBiased<T>(data, len));
}
template<typename T> T stdevBiased(Array<T> arr){
  return stdevBiased(arr.data, arr.length);
}

//PCC calculation
//Code adapted from http://www.cs.tufts.edu/comp/135/pearson.html
template<typename T> T pcc(T* x, T* y, unsigned len){
  T ssx, ssy, cp, mx, my;
  ssx = ssy = cp = mx = my = 0;
  for(unsigned i = 0; i < len; i++){
    ssx += x[i] * x[i];
    ssy += y[i] * y[i];
    cp += x[i] * y[i];
    mx += x[i];
    my += y[i];
  }
  mx /= len;
  my /= len;
  
  T pop_sd_x = sqrt((ssx / len) - (mx * mx));
  T pop_sd_y = sqrt((ssy / len) - (my * my));
  T cov_xy = (cp / len) - (mx * my);
  T pcc = cov_xy / (pop_sd_x * pop_sd_y);
  return pcc;
}
template<typename T> T pcc(Array<T> arr0, Array<T> arr1){
  assert(arr0.length == arr1.length);
  return pcc(arr0.data, arr1.data, arr0.length);
}

////////////////
//VECTOR BASED//
////////////////

template<typename T> void vectorMean(T* out, T** in, unsigned vLen, unsigned vCount){
  for(unsigned i = 0; i < vLen; i++){
    out[i] = 0;
  }
  for(unsigned i = 0; i < vCount; i++){
    for(unsigned j = 0; j < vLen; j++){
      out[j] += in[i][j];
    } 
  }
  for(unsigned i = 0; i < vLen; i++){
    out[i] /= vCount;
  }
}

template<typename T> void vectorMean(Array<T> out, Array<Array<T>> in){
  for(unsigned i = 0; i < out.length; i++){
    out[i] = 0;
  }
  for(unsigned i = 0; i < in.length; i++){
    assert(in[i].length == out.length);
    for(unsigned j = 0; j < out.length; j++){
      out[j] += in[i][j];
    } 
  }
  for(unsigned i = 0; i < out.length; i++){
    out[i] /= in.length;
  }
}

/////////////////////
//DOUBLE COMPARISON//
/////////////////////

#define EPSILON (1.0 / 1024.0)
//Epsilon Comparison requires subtraction, abs(T), and < defined.
template<typename T> bool epsilonCompare(T t0, T t1){
  return std::abs(t0 - t1) < EPSILON;
}

/////////////////////
//ARRAY CONVENIENCE//
/////////////////////

//Sets each element of the given array to a value.
template<typename T> void arraySet(T* array, unsigned length, T val){
  for(unsigned i = 0; i < length; i++){
    array[i] = val;
  }
}
template<typename T> void arraySet(Array<T> arr, T val){
  arraySet(arr.data, arr.length, val);
}

//Sets each element of the given array to 0.
template<typename T> void arrayZero(T* array, unsigned length){
  arraySet<T>(array, length, 0);
}
template<typename T> void arrayZero(Array<T> arr){
  arrayZero(arr.data, arr.length);
}

//Copies an array into another (must have the same length
template<typename T> void arrayCopy(T* d, T* s, unsigned len){
  for(unsigned i = 0; i < len; i++){
    d[i] = s[i];
  }
}
template<typename T> void arrayCopy(Array<T> arr0, Array<T> arr1){
  assert(arr0.length == arr1.length);
  arrayCopy(arr0.data, arr1.data, arr0.length);
}

//Copies an array into new memory
template<typename T> T* arrayCopy(T* s, unsigned len){
  T* d = new T[len];
  arrayCopy(s, d, len);
  return d;
}
template<typename T> Array<T> arrayCopy(Array<T> src){
  Array<T> dest = Array<T>(src.length);
  arrayCopy(dest, src);
  return dest;
}

#endif
