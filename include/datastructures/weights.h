#ifndef __WEIGHTS_H__
#define __WEIGHTS_H__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "tensor.h"


class Weight
{
    public:
        std::string identifier;
        Tensor<double>* values;

        Weight(std::string id, std::vector<unsigned int> v);
        ~Weight();

        Tensor<double>* get_weight_tensor();
};

#endif
