#ifndef __NEURAL_H
#define __NEURAL_H

#include "matrix/matrix.h"

typedef struct
{
    bvr_mat_real_t *w_input_hidden;
    bvr_mat_real_t *w_input_hidden_T;

    bvr_mat_real_t *hidden_output;
    bvr_mat_real_t *hidden_output_T;

    bvr_mat_real_t *output;

    bvr_mat_real_t *x_hidden;
    bvr_mat_real_t *o_hidden;
    bvr_mat_real_t *x_output;

    bvr_mat_real_t *output_errors;
    bvr_mat_real_t *hidden_errors;

    double (*activation_func)(double);
    double (*activation_func_deriv)(double);
    double (*error_func)(double input, double output);

} bvr_neural_net_t;

typedef struct
{
    bvr_mat_real_t *input_layer;
    bvr_mat_real_t *output_layer;

} bvr_neural_training_set_t;

bvr_neural_net_t *bvr_new_neural_net(size_t input_nodes_size, size_t hidden_nodes_size, size_t output_node_size);
void bvr_neural_net_diffuse(bvr_neural_net_t *net, bvr_mat_real_t *input);
void bvr_net_output_error_mat(bvr_neural_net_t *net, bvr_mat_real_t *input, bvr_mat_real_t *result);
void bvr_neural_net_free(bvr_neural_net_t *net);
void bvr_mat_weight_back(double alpha, const bvr_mat_real_t *output, const bvr_mat_real_t *error);
int bvr_neural_net_train(bvr_neural_net_t *net, bvr_neural_training_set_t *set, size_t length);
#endif
