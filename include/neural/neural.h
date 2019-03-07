#ifndef __NEURAL_H
#define __NEURAL_H

#include "matrix/matrix.h"

typedef struct
{
    bvr_matf32_t *input_layer;
    bvr_matf32_t *hidden_layer;
    bvr_matf32_t *output_layer;

    float (*activation_func)(double);
    float (*activation_func_deriv)(double);

} bvr_neural_net_t;

typedef struct
{
    bvr_matf32_t *input_layer;
    bvr_matf32_t *output_layer;

} bvr_neural_training_set_t;

bvr_neural_net_t *bvr_new_neural_net(size_t entry_vector_size, size_t hidden_layer_size, size_t output_layer_size);
void bvr_neural_net_free(bvr_neural_net_t *net);

int bvr_neural_net_train(bvr_neural_net_t *net, bvr_neural_training_set_t *set, size_t length);
#endif