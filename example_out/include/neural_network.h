
/********************

    NEURAL_NETWORK_H

Code Generated using nn4mc

*/

#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "neural_network_params.h"


volatile int num_layers = 0;

struct conv1d *conv1d_1;
struct dense *dense_1;
struct dense *dense_2;
struct dense *dense_3;



float fwdNN(float*);

#endif
