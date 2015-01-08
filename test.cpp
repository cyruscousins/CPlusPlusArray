#include <iostream>
#include <cmath>
#include "array.hpp"
#include "vectormath.hpp"

Array<int> count(unsigned count){
  int* data = new int[count];
  for(unsigned i = 0; i < count; i++){
    data[i] = (int)i;
  }
  return Array<int>(data, count);
}

bool testMap(){
	Array<int> testArr = count(5);
	int should[5] = {0,1,4,9,16};
	Array<int> shouldArr(should, 5);
 	Array<int> newArr = testArr.map<int>([](int v){return v * v;});

	return shouldArr == newArr;
}

int expensiveCalculation(int i){
	return (int)(sqrt(i * i + i) / i);
}

#define PCOUNT 10000000
bool testMapParallel(){
	Array<int> testArr = count(PCOUNT);
	int* should = new int[PCOUNT];
	for(unsigned i = 0; i < PCOUNT; i++){
		should[i] = expensiveCalculation(i);
	}
	Array<int> shouldArr(should, PCOUNT);
 	Array<int> newArr = testArr.mapParallel<int>([](int v){return expensiveCalculation(v);});

	return shouldArr == newArr;
}

bool testFilter(){
	int test[5] = {0,1,2,3,4};
	int should[2] = {1,3};
	Array<int> testArr(test, 5);
	Array<int> shouldArr(should, 2);
	Array<int> newArr = testArr.filter([](int v){return v % 2 == 1;});

	return shouldArr == newArr;
}

bool testPredicates(){
	int test[5] = {1, 3, 5, 7};
	Array<int> testArr(test, 4);

	return testArr.conjunction([](int v){return v % 2 == 1;})
	   && !testArr.conjunction([](int v){return v < 4;})
	   &&  testArr.disjunction([](int v){return v == 1;})
	   && !testArr.disjunction([](int v){return v == 2;})
	   
	   &&  testArr.conjunction<int>([](int v, int cl){return v % 2 == cl;}, 1)
	   && !testArr.disjunction<int>([](int v, int cl){return v % 2 == cl;}, 0);
	
}

bool testSliceLikes(){
	Array<int> test = count(100);
	return (head(test) == 0)
     &&  (head(tail(test)) == 1)
     &&  (head(tail(tail(test))) == 2)
     &&  (head(test + 3) == 3)
     &&  (head(slice(test, 4, 5)) == 4);
}

bool testTakeDrop(){
	Array<int> test = count(100);
	return sumTerms(test) == sumTerms(test.take(50)) + sumTerms(test.drop(50));
}

bool testForEach(){
  Array<int> test = Array<int>(100, 0);
  test.forEach([](int& i){i = i + 1;});
  for(unsigned i = 0; i < 100; i++){
    if(test[i] != 1) return false;
  }
  return true;
}

bool testZip(){
  Array<int> test0 = Array<int>(10, 2);
  Array<double> test1 = Array<double>(10, 1);
  double cl = .5;
  
  Array<float> result = test0.zip<double, float, double>(test1, [](int a, double b, double cl){return (float) (a * b * cl);}, cl);
  
  return result.conjunction([](float val){return val == 1;});
}

bool testFold(){
  double data[4] = {2, -2, 2, -2};
  Array<double> arr = Array<double>(data, 4);  

  double length = arr.foldUnordered([](double a, double b){return sqrt(a * a + b * b);});
  
  double length2 = sqrt(arr.fold<double>([](double sum, double next){return sum + next * next;}, 0));
  
  return length == 4
     &&  length2 == 4;
}

int main(){
	if(!testMap()){
		std::cout << "Map error." << std::endl;
	}
	if(!testMapParallel()){
		std::cout << "Map Parallel error." << std::endl;
	}
	if(!testFilter()){
		std::cout << "Filter error." << std::endl;
	}
	if(!testPredicates()){
		std::cout << "Filter error." << std::endl;
	}
	if(!testSliceLikes()){
		std::cout << "Slice likes error." << std::endl;
	}
	if(!testForEach()){
		std::cout << "For each error." << std::endl;
	}
	if(!testZip()){
		std::cout << "Zip error." << std::endl;
	}
	if(!testFold()){
		std::cout << "Fold error." << std::endl;
	}
}
