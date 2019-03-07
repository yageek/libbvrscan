#include "neural/neural.h"

#include <math.h>
/// Private functions

/// Activations func
float sigmoid(float x) { return 1 / (1 + exp(-x)); };
float sigmoid_deriv(float x) { return sigmoid(x) * (1 - sigmoid(x)); };

/// Diffuse
void bvr_neural_net_diffuse(bvr_neural_net_t *net, float *vector, size_t length)
{
}

bvr_neural_net_t *bvr_new_neural_net(size_t entry_vector_size, size_t hidden_layer_size, size_t output_layer_size)
{
    bvr_matf64_t *entry = bvr_matf64_new(entry_vector_size, hidden_layer_size);
    bvr_matf64_t *hidden = bvr_matf64_new(entry_vector_size, hidden_layer_size);
    bvr_matf64_t *output_layer = bvr_matf64_new(entry_vector_size, hidden_layer_size);

    bvr_neural_net_t *net = (bvr_neural_net_t *)malloc(sizeof(bvr_neural_net_t));
    net->input_layer = entry;
    net->hidden_layer = hidden;
    net->output_layer = output_layer;

    net->activation_func = sigmoid;
    net->activation_func_deriv = sigmoid_deriv;

    return net;
}

void bvr_neural_net_free(bvr_neural_net_t *net)
{
    bvr_mat_free(net->input_layer);
    bvr_mat_free(net->hidden_layer);
    bvr_mat_free(net->output_layer);
    free(net);
}

int bvr_neural_net_train(bvr_neural_net_t *net, bvr_neural_training_set_t *set, size_t length)
{
}