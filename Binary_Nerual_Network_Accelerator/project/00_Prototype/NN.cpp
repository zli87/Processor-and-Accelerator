#include "NN.h"

bit activation(uint16 size_w, uint16 conv){
	uint16 threshold = (size_w * size_w) /2;
	if(0==size_w%2) threshold--;
	return (conv > threshold)? (bit)1 : (bit)0;
}

#ifdef NN_PIPE_PARALLEL
void conv2d(uint16 size_w, uint16 dim1, uint16 dim2, bit* sram_weight, bit* sram_input, bit* sram_output){
    uint16 dim1o    = dim1-size_w+1;
    uint16 dim2o    = dim2-size_w+1;
    bit* w0 = sram_weight + 0; // weight row 0
    bit* w1 = sram_weight + 3; // weight row 1
    bit* w2 = sram_weight + 6; // weight row 2
    uint16 b_pipe1[14] = {0}; // maximun regiter # 16-3+1 = 14 parallel PE
    uint16 b_pipe2[14] = {0};
    uint16 Z_pipe1[14] = {0};
    uint16 Z_pipe2[14] = {0};
    uint16 Z_pipe3[14] = {0};

    loop_pipe :for(int i=0;i<dim1;++i){
        // each clock cycle read in one row from input sram
        bit* A = sram_input  + i*WORDSIZE;
        loop_parallel: for(int sl=0; sl<14; sl++){ // shift left
            // In the microarchitecture, this for loop is unrolled. they computes in the same clock sycle.
            // In the microarchitecture, the PE's input port b is connect to the regiter bits, which store Z's value in previous clock cycle.
	    // therefore, before we start execute process element, we should store Z's value in b.
	    b_pipe1[sl] = Z_pipe1[sl];
	    b_pipe2[sl] = Z_pipe2[sl];
	    // execute pipeline PE
	    process_element_pipe(           0, A[0+sl], A[1+sl], A[2+sl], w0[0], w0[1], w0[2], &(Z_pipe1[sl]) );
	    process_element_pipe( b_pipe1[sl], A[0+sl], A[1+sl], A[2+sl], w1[0], w1[1], w1[2], &(Z_pipe2[sl]) );
	    process_element_pipe( b_pipe2[sl], A[0+sl], A[1+sl], A[2+sl], w2[0], w2[1], w2[2], &(Z_pipe3[sl]) );
	    // after the pipeline is fulled, write to output sram.
	    if(2 <= i) sram_output[(i-2)*WORDSIZE+sl] = (sl < dim2o)? activation(size_w, Z_pipe3[sl]) : (bit) 0;
        } //end parallel loop
    }// end pipe loop
}

void process_element_pipe(uint16 b, bit A0, bit A1, bit A2, bit w0, bit w1, bit w2, uint16* Z){
    bit A = !(A0^w0);
    bit B = !(A1^w1);
    bit C = !(A2^w2);
    *Z = A + B + C + b;
}
#endif

#ifdef NN_LOOP
void conv2d(uint16 size_w, uint16 dim1, uint16 dim2, bit* sram_weight, bit* sram_input, bit* sram_output){

	uint16 dim1o    = dim1-size_w+1;
	uint16 dim2o    = dim2-size_w+1;
	uint16 size_max = size_w * size_w;

	loop_row :for(int i=0;i<dim1o;++i){	// down shift row index
		loop_col :for(int j=0;j<dim2o;++j){ // right shift col index
			uint16 conv;
			loop_kernel: for(int k=0;k<size_max;++k){
				if(0==k) // reset value for new 2-D convolution
					conv = 0;

				uint16 offset_dim1 = k/size_w;
				uint16 offset_dim2 = k%size_w;
				bit in_b = sram_input[(i+offset_dim1)*WORDSIZE+j+offset_dim2];
				conv += !(sram_weight[k]^in_b); //conv += weight[k]*input[i+k][j+k];

				if(size_max-1 == k){ // finish 2-D convolution, write to output sram
					sram_output[i*WORDSIZE+j] = activation(size_w, conv);
				}
			}// end kernel loop
		}// end col loop
	}// end row loop
}
#endif
