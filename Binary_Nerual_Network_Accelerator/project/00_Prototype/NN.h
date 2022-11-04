#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

//#define NN_LOOP
#define NN_PIPE_PARALLEL
#define WORDSIZE 16 // bits per word
#define uint16 unsigned int
typedef bool bit;

#ifdef NN_PIPE_PARALLEL
void conv2d(uint16 size_w, uint16 dim1, uint16 dim2, bit*W, bit* In, bit* Out);
void process_element_pipe(uint16 b, bit A0, bit A1, bit A2, bit w0, bit w1, bit w2, uint16* Z);
#endif

#ifdef NN_LOOP
void conv2d(uint16 size_w, uint16 dim1, uint16 dim2, bit*W, bit* In, bit* Out);
bit activation(uint16 size_w, uint16 conv);
#endif

void printmatrix(bit** M, uint16 dim1, uint16 dim2);
