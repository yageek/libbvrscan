#include "neural/neural.h"

#define OUTPUT_SIZE 13

// See the train_genann executable.
// The order should be the same as the `files` array of `.ocrb` files.
static const char neural_output_vector_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '>', '<'};

bvr_neural_net_t *bvr_neural_net_load(const char *neural_file)
{
    FILE *fd = fopen(neural_file, "r");
    genann *in = genann_read(fd);
    if (!in)
    {
        perror("invalid neural net file");
        return NULL;
    }

    bvr_neural_net_t *net = (bvr_neural_net_t *)malloc(sizeof(bvr_neural_net_free));
    net->inner_net = in;
    return net;
}

void bvr_neural_net_free(bvr_neural_net_t *net)
{
    genann_free(net->inner_net);
    free(net);
}

const char bvr_neural_net_detect_blob(const bvr_neural_net_t *net, const bvr_mat_real_t *input)
{
    // Query the network
    const double *answer = genann_run(net->inner_net, input->content);

    // Find the maximum value in the array
    int maxIndex = 0;
    int i;
    for (i = 1; i < OUTPUT_SIZE; i++)
    {
        if (answer[i] >= answer[maxIndex])
        {
            maxIndex = i;
        }
    }

    return neural_output_vector_map[maxIndex];
}
