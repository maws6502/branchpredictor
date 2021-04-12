## EE318 Assignment 1

Aditya Goturu <aditya18203@mechyd.ac.in>
18XJ1A0203

### Compiling and running

To compile, run:

```
$ make
```

You will need a version of make, as well as a C compiler. The compiler is assumed to be GCC, specify otherwise in the CC argument:

```
$ make CC=clang
```

To run the program, specify the trace file as the first argument:

```
$ ./predictor /path/to/trace
```

The program will run all the predictors and display the results in a tabulated format. Value shown is (correct predictions)/(total branches).

#### Example

```
$ ./predictor traces/trace_01
ST                                              0.878724
SNT                                             0.121276
Dynamic BM                      n=2             0.877444
Dynamic BM                      n=4             0.876443
Dynamic BM                      n=8             0.879254
Dynamic BM+GSHARE               n=2             0.876624
Dynamic BM+GSHARE               n=4             0.878490
Dynamic BM+GSHARE               n=8             0.877773
```
### About

##### The following predictors are implemented

* Static Taken (ST)
* Static Not Taken (SNT)
* Dynamic Bimodal (Dynamic BM)
* Dynamic Bimodal + Global Branch History (Dynamic BM+GSHARE)


##### General program design
* Each predict-and-compare function lies in it's own file. The main function is in the predictor.c file.
* Central header file (predictor.h).
* Trace file is read into a linked list, and this list is passed onto each predict-and-compare function.
* These functions are to ignore the root of the passed-in linked list, as only it's ->next is meaningful.
* These functions will return a double containing the fraction of correct predictions.

##### Code style
* C89 for the most part.
* Assume system integer or system long, unless specific size is needed.