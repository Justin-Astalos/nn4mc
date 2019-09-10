
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
#include "layers/conv1d.h"
#include "layers/dense.h"


volatile int num_layers = 0;

/*
conv1d layer1;
dense layer2;

*/

float fwdNN(float*);

#endif

