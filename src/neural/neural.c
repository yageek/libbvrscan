#include "neural/neural.h"

#include <math.h>
#include <time.h>
#include <pthread.h>

static int generated = -1;
void fill_rand_mat(bvr_mat_real_t *mat) {

    
    if(generated < 0) {
        time_t t;
        srand((unsigned) time(&t));
        generated = 0;
    }

    int row, col;
    for(row = 0; row < mat->rows; row++) {

        for(col = 0; col < mat->columns; col++) {
            double val = ((double)(rand() % 100))/100.0;
            if (val == 0) {
                val = 0.1;
            }
            bvr_mat_set(mat, row, col, val);
        }
    }

}
/// Private functions

/// Activations func
double sigmoid(double x) { return 1 / (1 + exp(-x)); };
double sigmoid_deriv(double x) { return sigmoid(x) * (1 - sigmoid(x)); };

/// Diffuse
void bvr_neural_net_diffuse(bvr_neural_net_t *net, bvr_mat_real_t *input)
{
    bvr_mat_real_mul(net->w_input_hidden, input, net->x_hidden); // X_hidden = W.I
    bvr_mat_apply_scalar_func(net->x_hidden, net->activation_func, net->o_hidden); // O = sigma(X)
    bvr_mat_real_mul(net->hidden_output, net->o_hidden, net->x_output);            // x = W.O
    bvr_mat_print(net->x_output);
}

bvr_neural_net_t *bvr_new_neural_net(size_t input_nodes_size, size_t hidden_nodes_size, size_t output_node_size)
{
    // We determine the size of each matrix
    bvr_mat_real_t *w_input_hidden = bvr_mat_real_new(hidden_nodes_size, input_nodes_size);
    fill_rand_mat(w_input_hidden);

    bvr_mat_real_t *x_hidden = bvr_mat_real_new(input_nodes_size, 1);
    bvr_mat_real_t *o_hidden = bvr_mat_real_new(input_nodes_size, 1);

    bvr_mat_real_t *hidden_output = bvr_mat_real_new(hidden_nodes_size, output_node_size);
    fill_rand_mat(hidden_output);
    bvr_mat_real_t *x_output = bvr_mat_real_new(output_node_size, 1);

    bvr_neural_net_t *net = (bvr_neural_net_t *)malloc(sizeof(bvr_neural_net_t));
    net->w_input_hidden = w_input_hidden;
    net->hidden_output = hidden_output;
    net->x_hidden = x_hidden;
    net->o_hidden = o_hidden;
    net->x_output = x_output;

    net->activation_func = sigmoid;
    net->activation_func_deriv = sigmoid_deriv;

    return net;
}

void bvr_neural_net_free(bvr_neural_net_t *net)
{
    bvr_mat_free(net->w_input_hidden);
    bvr_mat_free(net->hidden_output);
    bvr_mat_free(net->x_hidden);
    bvr_mat_free(net->o_hidden);
    bvr_mat_free(net->x_output);
    free(net);
}

int bvr_neural_net_train(bvr_neural_net_t *net, bvr_neural_training_set_t *set, size_t length)
{
}
