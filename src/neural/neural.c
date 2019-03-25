#include "neural/neural.h"

bvr_neural_net_t *bvr_load_neural_net(const char *neural_file)
{
    FILE *fd = fopen(neural_file, "r");
    genann *in = genann_read(fd);
    if (!in)
    {
        perror("invalid neural net file");
        return NULL;
    }

    bvr_neural_net_t *net = (bvr_neural_net_t *)malloc(sizeof(bvr_free_net));
    net->inner_net = in;
    return net;
}

void bvr_free_net(bvr_neural_net_t *net)
{
    genann_free(net->inner_net);
    free(net);
}

const char *bvr_neural_net_detect_blob(const bvr_neural_net_t *net, const bvr_mat_real_t *input)
{
    const double *answer = genann_run(net->inner_net, input->content);

    for (int i = 0; i < 13; i++)
    {
        printf("%f\t", answer[i]);
    }
    printf("\n");

    return "ddd";
}