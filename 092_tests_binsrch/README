For your next assignment, you will be writing a function to perform
binary search.  You can read that assignment's README in the file next-README.

For this assignment, you will be writing test cases (in a file called
test-bsearch.cpp) for the binary search
function before you implement it.  By now, you should be quite familiar
with the idea of writing test cases first.

For this particular assignment, we have provided object files in
/usr/local/ece551/bin_search/

with different compiled implementations of binary search.  bsearch.o
contains a correct implementation, and the others are broken in some way.

Your testing should include checking if the binary search implementation
invokes the f an appropriate number of times (specifically,
at most log2(high-low)+1 times if high > low, and at most once otherwise).

To help you check the right number of invocations, we suggest the following

class which can wrap another Function<int,int> (see function.h for
the definition of Function) and count its invocations:


class CountedIntFn : public Function<int,int>{
protected:
  unsigned remaining;
  Function<int,int> * f;
  const char * mesg;
public:
  CountedIntFn(unsigned n, Function<int,int> * fn, const char * m): remaining(n),
                                                                    f(fn),
								    mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr,"Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }

};



When you create an instance of CountedIntFn, you should pass in the
maximum number of invocations allowed, the actual function to comptue with,
and a message which is useful to you to know what happened if the test fails.

I will also note that I found it useful to abstract out the implementation
of running the tests into this function:

void check(Function<int,int> * f,
           int low,
	   int high,
	   int expected_ans,
	   const char * mesg);


in which I computed the maximum number of invocations of f, wrap f in
a CountedIntFn, perform the binary search, and check the answer.


We have provided a run_all.sh script which will compile your test-bsearch.cpp
with each object file, run your tests, and report if your tests failed
each broken implementation (and passed the correct one).

Submit your test-bsearch.cpp file when you are done.
