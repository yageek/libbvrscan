#ifndef __NEURAL_H
#define __NEURAL_H

#include "matrix/matrix.h"
#include "genann.h"

typedef struct
{
    genann *inner_net;
} bvr_neural_net_t;

bvr_neural_net_t *bvr_load_neural_net(const char *neural_file);
void bvr_free_net(bvr_neural_net_t *net);

const char *bvr_neural_net_detect_blob(const bvr_neural_net_t *net, const bvr_mat_real_t *input);
#endif
