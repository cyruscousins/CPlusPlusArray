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

	return
		testArr.conjunction([](int v){return v % 2 == 1;})
	    && !testArr.conjunction([](int v){return v < 4;})
	    &&  testArr.disjunction([](int v){return v == 1;})
	    && !testArr.disjunction([](int v){return v == 2;});
	
}

bool testSliceLikes(){
	Array<int> test = count(100);
	return (head(test) == 0)
            && (head(tail(test)) == 1)
            && (head(tail(tail(test))) == 2)
            && (head(test + 3) == 3)
            && (head(slice(test, 4, 5)) == 4);
}

bool testTakeDrop(){
	Array<int> test = count(100);
	return sumTerms(test) == sumTerms(test.take(50)) + sumTerms(test.drop(50));
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
}
