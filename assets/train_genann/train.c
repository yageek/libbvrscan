
#include "genann.h"
#include <bvrcore.h>
#include <io.h>
#include <string.h>

#define INPUTS 1024
#define OUTPUTS 13
#define HIDDEN_LAYER_COUNT 1
#define HIDDEN_LAYER_NODES_COUNT 100
#define LEARNING_RATE 0.3



void free_in_data(bvr_mat8_t **asset_data, size_t len) {
    // Free memory
    int i;
    for (i = 0; i < len; i++)
    {
        bvr_mat8_t *mat = asset_data[i];
        if (mat != NULL)
        {
            bvr_mat_free(mat);
        }
    }
}

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

    // Load the assets dats
    for (i = 0; i < OUTPUTS; i++)
    {

        snprintf(input_name, 1024, "%s/%c.png", input_dir, input_index[i]);
        bvr_io_image_source_t *src = bvr_io_load_png(input_name);
        if (src == NULL)
        {
            perror("impossible to load image.");
            free_in_data(asset_data, OUTPUTS);
            exit(-1);
        }

        bvr_mat8_t *rgb = bvr_io_rgb_src(src);
        if(rgb == NULL) {
            perror("can not load rgb image");
            bvr_io_image_source_free(src);
            free_in_data(asset_data, OUTPUTS);
            exit(-1);
        }
        asset_data[i] = bvr_filter_create_grayscale(rgb);
        bvr_io_image_source_free(src);
        bvr_mat_free(rgb);
    }

    // Load and train
    int epoch = 100;
    int e;
    for(e = 0; e < epoch; e++) {

        // For each characters
        int i;
        for(i = 0; i < OUTPUTS; i++) {

            // Load the inputs
            bvr_mat8_t *data = asset_data[i];
            uint8_t * in_ptr = data->content;

            int j;
            for(j = 0; j < INPUTS; j++) {
                double val =  ((double)in_ptr[j]);
                
                current_input[j] = val;
            }

            // Load the outputs
            memset(current_output, 0, OUTPUTS*sizeof(double));
            current_input[i] = 1.0;

            // Train the network
            genann_train(ann, current_input, current_output, LEARNING_RATE);

        }
    }


    // Test
    const double *answer = genann_run(ann, current_input);
    for (i=0; i < OUTPUTS; i++){
        printf("%f\t", answer[i]);
    }
    printf("\n");


    // Free memory
    free_in_data(asset_data, OUTPUTS);
    genann_free(ann);


    return 0;
}
