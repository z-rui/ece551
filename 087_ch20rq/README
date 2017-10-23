As always, read and understand Chapter 20, then answer these questions
in "answers.txt".


For questions 1--5, you will be given two functions,
f and g.  You should determine if f is in O(g) and if
g is in O(f), then choose the proper answer.  Note that
we write x^y to mean "x to the y power" (so x^2 is x squared).

1. f(x) = x^2   + 2*x + 3
   g(x) = 2*x^2 - 3*x + 4 

A. f is in O(g) AND g is in O(f)
B. f is in O(g) 
C. g is in O(f) 
D. Neither f nor g is in O of the other one.

2. f(x) = x^2 + 2 *x + 3
   g(x) = 400 * x + 5000

A. f is in O(g) AND g is in O(f)
B. f is in O(g) 
C. g is in O(f) 
D. Neither f nor g is in O of the other one.

3. f(x) = log(x^2)
   g(x) = log(x)

A. f is in O(g) AND g is in O(f)
B. f is in O(g) 
C. g is in O(f) 
D. Neither f nor g is in O of the other one.

4. f(x) = 2^x + 32 * x^3
   g(x) = 3^x

A. f is in O(g) AND g is in O(f)
B. f is in O(g) 
C. g is in O(f) 
D. Neither f nor g is in O of the other one.

5. f(x) = x!    //x factorial
   g(x) = 2^x 

A. f is in O(g) AND g is in O(f)
B. f is in O(g) 
C. g is in O(f) 
D. Neither f nor g is in O of the other one.


For questiosn 6--9, you will be given a C or C++ function,
which you should figure out the runtime of.  Then you should
choose the function which best describes the runtime of the function.
Specifically, you should choose the answer such that the runtime
is in Big-Oh of that answer, but is not in Big-Oh of any "smaller"
answer.

6. 

 int f(const std::vector<int> & v) {
   int count = 0;
   for(std::vector<int>::const_iterator it = v.begin();
       it != v.end();
       ++it) {
      if (*it < 0) {
        count++;
      }
   }
   return count;
 }

Here N is the length of v (v.size())

A. O(1)
B. O(lg(N))
C. O(N)
D. O(N * lg(n))
E. O(N^2)
F. O(N^k) for some other k> 2 
   [polynomial time, but worse than quadratic]
G. O(2^N)
H. O(N!)

7. 

Matrix multiply(const Matrix & m1, const Matrix & m2) {
   Matrix ans(m1.rows, m2.columns);
   assert(m1.columns == m2.rows);
   for (int i = 0; i < ans.rows; i++) {
      for (int j = 0; i < ans.columns; j++) {
       int sum = 0;
       for (int k = 0; k < m1columns; k++){
         sum += m1[i][k] * m2[k][j];
       }
       ans[i][j] = sum;
      }
   }
   return ans;
}

Here, N is the largest of m1.rows, m1.columns, m2.rows, and m2.columns:

A. O(1)
B. O(lg(N))
C. O(N)
D. O(N * lg(n))
E. O(N^2)
F. O(N^k) for some other k> 2 
   [polynomial time, but worse than quadratic]
G. O(2^N)
H. O(N!)


8. Imagine that there is an AribtraryInt class, which holds
   an arbitrarily large int (any number of bits you want). Assume
   this class overloads the relevant operators (++, <=, %, ==, <<),
   as well as having a constructor which allows it to be initialized
   from an int.

  ArbitraryInt findSmallestDivisor(const ArbitraryInt& x) {
     for(ArbitraryInt i = 2; i <= x; ++i) {
       if (x % i == 0) {
          return i;
       }
     }
     std::cerr << x << " should be divisible by itself!\n";
     abort();
  }

Here, N is the number of bits in x:

A. O(1)
B. O(lg(N))
C. O(N)
D. O(N * lg(n))
E. O(N^2)
F. O(N^k) for some other k> 2 
   [polynomial time, but worse than quadratic]
G. O(2^N)
H. O(N!)
  

9.
  template<typename T>
  std::vector<T> g(std::vector<T>  v) {
     std::vector<T> ans;
     while(!v.empty()) {
      std::vector<T>::iterator it = v.begin()
      std::vector<T>::iterator m = it;
      ++it;
      while(it != v.end()) {
        if (*m < *it){ 
           m = it;
        }
        ++it;
      }
      ans.push_back(*m);
      v.erase(m);
    }
    return ans;
  }

Here, N is the length of v (v.size()).

A. O(1)
B. O(lg(N))
C. O(N)
D. O(N * lg(n))
E. O(N^2)
F. O(N^k) for some other k> 2 
   [polynomial time, but worse than quadratic]
G. O(2^N)
H. O(N!)


For questions 10--15, identify the approrpriate ADT:

10.  Last in, first out

A. Queue
B. Stack
C. Set
D. Map

11. Tracks key/value pairs

A. Queue
B. Stack
C. Set
D. Map


12. First in, first out
A. Queue
B. Stack
C. Set
D. Map


13. Useful if you want to add elements, then check 
    if it has a particular element
A. Queue
B. Stack
C. Set
D. Map

14. Would be useful for parsing, e.g., matching HTML tags
A. Queue
B. Stack
C. Set
D. Map

15. Would be used to hold incoming network data
    until it could be processed
A. Queue
B. Stack
C. Set
D. Map



