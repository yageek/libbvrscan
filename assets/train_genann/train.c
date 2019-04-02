
#include "genann.h"
#include <bvrcore.h>
#include <io.h>

#define INPUTS 1024
#define OUTPUTS 13
#define HIDDEN_LAYER_COUNT 1
#define HIDDEN_LAYER_NODES_COUNT 100
#define LEARNING_RATE 0.3

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Name input and output directory at minimum\n");
        exit(-1);
    }

    // We need to train the neural network
    genann *ann = genann_init(INPUTS, HIDDEN_LAYER_COUNT, HIDDEN_LAYER_NODES_COUNT, OUTPUTS);

    const char *input_dir = argv[1];
    const char *output_dir = argv[2];

    const char input_index[OUTPUTS] = "0123456789<+>";
    bvr_mat8_t *asset_data[OUTPUTS] = {0};

    // Load the inputs
    int i;
    double current_input[INPUTS];
    double current_output[OUTPUTS];
    char input_name[1024];

    for (i = 0; i < OUTPUTS; i++)
    {
        // Load the file data
        snprintf(input_name, 1024, "%s/%c.png", input_dir, input_index[i]);
        bvr_io_image_source_t *src = bvr_io_load_png(input_name);
        if (src == NULL) {
            perror("impossible to load image.");
            exit(-1);
        }
        asset_data[i] = bvr_io_grayscale_src(src);
        bvr_io_image_source_free(src);
    }

    // Free memory

    for (i = 0; i < OUTPUTS; i++)
    {
        bvr_mat8_t *mat = asset_data[i];
        if(mat != NULL) {
            bvr_mat_free(mat);
        }
    }

    return 0;
}
