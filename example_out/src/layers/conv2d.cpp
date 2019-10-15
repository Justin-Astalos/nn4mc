

/*************
* conv2d.cpp
*
* Conv2D Layer
* Hardware Plateform: ESP-32
*
* This file defines the functions needed to build a 2D Convolutional layer, and perform a forward pass.
* TODO: This template.
*/

#include "conv2d.h"
#include <stdlib.h>
#include <math.h>

#define max(a, b) (((a)>(b) ? (a) : (b)))
#define min(a, b) (((a)<(b) ? (a) : (b)))

struct Conv2D buildConv2D(const float* W, const float* b, int kernel_shape_0, int kernel_shape_1, int filters, int strides_0, int strides_1, int input_shape_0,int input_shape_1,int input_shape_2, char activation)
{
	struct Conv2D layer;

	layer.weights = W;
	layer.bias = b;

	layer.weight_shape[0] = kernel_shape_0;
	layer.weight_shape[1] = kernel_shape_1;
	layer.weight_shape[2] = input_shape_2;
	layer.weight_shape[3] = filters;

    layer.filters = filters;
    layer.activation= activation;
    layer.kernel_shape[0] = kernel_shape_0;
    layer.kernel_shape[1] = kernel_shape_1;

	layer.strides[0] = strides_0;				// NOTE: NOT YET IMPLEMENTED
	layer.strides[1] = strides_1;

	layer.input_shape[0] = input_shape_0;
	layer.input_shape[1] = input_shape_1;
	layer.input_shape[2] = input_shape_2;

	layer.output_shape[0] = layer.input_shape[0] - layer.kernel_shape[0] + 1;
	layer.output_shape[1] = layer.input_shape[1] - layer.kernel_shape[1] + 1;
	layer.output_shape[2] = layer.filters;

	return layer;
}


float* fwdConv2D(struct Conv2D L, float* input)
{

     float * h = (float*)malloc(L.output_shape[0]*L.output_shape[1]*L.output_shape[2] * sizeof(float));

	for(int i = 0; i < L.output_shape[0]; i++)
	{
		for(int j = 0; j < L.output_shape[1]; j++)
		{
			for(int k = 0; k < L.output_shape[2]; k++)
			{
				int output_idx = i * L.output_shape[1] * L.output_shape[2] + j * L.output_shape[2] + k;

				h[output_idx] = L.bias[k];


				for(int kernel_position_x = 0; kernel_position_x < L.kernel_shape[0]; kernel_position_x++)
				{

                    int mm = L.kernel_shape[0] - 1 - kernel_position_x;

					for(int kernel_position_y = 0; kernel_position_y < L.kernel_shape[1]; kernel_position_y++)
					{				

                        for (int f=0; f<L.filters; f++){
                        int nn = L.kernel_shape[1] - 1 - kernel_position_y;

                        int ii = i + (L.kernel_shape[1]/2 - mm);
                        int jj = j + (L.kernel_shape[0]/2 - nn);

                        if (ii >= 0 && ii < i && jj >= 0 && jj < j){
                            h[output_idx] += *(L.weights + L.weight_shape[3]*L.weight_shape[2]*L.weight_shape[1]*kernel_position_x + L.weight_shape[2]*L.weight_shape[1]* kernel_position_y + L.weight_shape[1]*f) * (input[L.input_shape[1]*L.input_shape[2]*ii + L.input_shape[2] * jj]);
                       }
                   } 

                }
            }

				// Now perform the activation function
				// NOTE: NEED TO IMPLEMENT
        // TODO: Make more elegant:
        // linear not here cause no action

        if (L.activation==0x08){ //sigmoid
            h[output_idx] = exp(h[output_idx])/(exp(h[output_idx]) + 1);
        }

        if (L.activation==0x04){ //softplus
            h[output_idx] = log(exp(h[output_idx]) + 1);
        }

        if (L.activation==0x05){ //softsign
            h[output_idx] = h[output_idx] / (abs(h[output_idx]) + 1);
        }

        if (L.activation==0x09){ //hard_sigmoid
            if (h[output_idx] < -2.5){
                h[output_idx] = 0.0;
            } else if (h[output_idx] > 2.5){
                h[output_idx] = 1.0;
            } else{
                h[output_idx] = 0.2*h[output_idx] + 0.5;
            }
        }

        if (L.activation==0xA){ //exponential
            h[output_idx] = (float)expf((float)h[output_idx]);
        }
        
         if (L.activation==0x06){ //relu
             h[output_idx]= max(h[output_idx], 0.0);
         }

         if (L.activation== 0x07){ //tanh
             h[output_idx]=tanh(h[output_idx]);
         }
         if (L.activation==0x00){ //softmax
             float sum_exp = 0.0;
             for (int ii=0; ii<L.output_shape[0]; i++){
                 sum_exp+= expf(h[ii]);
             }
             for (int ii=0; ii<L.output_shape[0];ii++){
                 float calc = expf(h[ii]) / sum_exp;
                 if (isnan(calc)){
                     h[ii] = 1.0;
                 } else h[ii] = (float)(expf(h[ii]) / sum_exp);
             }
         }

			}
		}
	}
    free(input);
    return h;
}
