We can perform binary search in other cases than just searching through an array.
For example, if we know that a function is monotonically increasing over a certain
range, and we want to find the x-value where the corresponding y-value is 0
(or any other number we want) we can perform binary search to make that task efficient.
Since we are dealing with integers, we have to handle the case where the 0 of the function
is between two values.   For the purposes of defining this problem, we are going to say
that we want the x-value less than the 0 of the function. 

For this problem you will write the function

  int binarySearchForZero(Function<int, int> * f, int low, int high) 

which peforms binary search to find the largest x such that f(x) <= 0
in the range [low, high) in lg (high-low) time. Note that if f(x) is all
negative, the answer is (high-1).  If f(x) is all positive, you should
return low, even though no "valid" solution exists.


You should write your code for this function in a file called search.cpp.
You should test your implementation with test-bsearch.cpp that you wrote
in the previous assignment.
You may assume that f is strictly increasing on the range [low, high), though it
may not be increasing at a uniform rate. 
You may assume low <= high.

For example,  if you had the following function class:

class SinFunction : public Function<int, int> {
public:
  virtual int invoke(int arg) {
    return 10000000 * (sin(arg/100000.0) - 0.5);
  }
};

and were searching for its 0 in the range [0,150000), your function should return
52359 in fewer than 20 invocations of f. 

