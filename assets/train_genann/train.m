
#include "genann.h"
#include <bvrcore.h>
#include <io.h>
#include <Foundation/Foundation.h>

#define INPUTS 1024
#define OUTPUTS 13
#define HIDDEN_LAYER_COUNT 1
#define HIDDEN_LAYER_NODES_COUNT 100
#define LEARNING_RATE 0.3

int main(int argc, char **argv)
{
    if( argc < 3 ){
        NSLog(@"Name input and output directory at minimum");
        exit(-1);
    }

    @autoreleasepool {
        // We need to train the neural network
        genann *ann = genann_init(INPUTS, HIDDEN_LAYER_COUNT, HIDDEN_LAYER_NODES_COUNT, OUTPUTS);

        NSString *inputDir = [NSString stringWithCString:argv[1] encoding:NSUTF8StringEncoding];
        NSString *outputDir = [NSString stringWithCString:argv[2] encoding:NSUTF8StringEncoding];
        NSMutableDictionary<NSString *, NSData*> *asset_data = [NSMutableDictionary dictionary];
        NSMutableDictionary<NSString *, NSNumber*> *input_index = [NSMutableDictionary dictionary];

        // This is the order of the files.
        // The output vector will be in the same order
        NSArray<NSString *> *files = @[
            @"0.ocrb",
            @"1.ocrb",
            @"2.ocrb",
            @"3.ocrb",
            @"4.ocrb",
            @"5.ocrb",
            @"6.ocrb",
            @"7.ocrb",
            @"8.ocrb",
            @"9.ocrb",
            @"+.ocrb",
            @">.ocrb",
            @"<.ocrb",
        ];

        // Load the inputs
        NSInteger i = 0;
        double current_input[INPUTS];
        double current_output[OUTPUTS];
        for(NSString *file in files) {
            // Load the file data
            NSData *data = [NSData dataWithContentsOfFile:[inputDir stringByAppendingPathComponent:file]];
            NSString *key = [file stringByDeletingPathExtension];
            asset_data[key] = data;
            input_index[key] = @(i);
            i++;
        }


        NSInteger epoch = 50;
        for(NSInteger e = 0; e < epoch; e++) {
//            NSLog(@"Pass %li", e);
            // Train the neural network
            for(NSString *key in asset_data) {
//                NSLog(@"\tTraining for %@...", key);

                // Load the input buffers
                NSData *current = asset_data[key];
                uint8_t *in_ptr = (uint8_t*)(current.bytes);
                for(NSInteger i = 0; i < INPUTS; i++) {
                    current_input[i] = (double)in_ptr[i];
                }

                // Load the output buffers
                memset(current_output, 0, OUTPUTS*sizeof(double));
                int numberIndex = [input_index[key] intValue];
                current_output[numberIndex] = 1;
                genann_train(ann, current_input, current_output, LEARNING_RATE);
            }

        }
        /// Test
        // 0 Char
        bvr_io_image_source_t src;
        int res = bvr_io_load_png("neural_tests_imgs/0_sized.png", &src);
        if(res < 0) {
            printf("Error during loading test image\n");
            return -1;
        }
        for(NSInteger i = 0; i < INPUTS; i++) {
            current_input[i] = src.data[i] == 255 ? 1.0 : 0.0;
        }
        
        const double *answer = genann_run(ann, current_input);
        for (NSInteger i=0; i < OUTPUTS; i++){
            printf("%f\t", answer[i]);
        }
        printf("\n");

//        const double* answer = genann_run(ann, current_input);
//        for (NSInteger i=0; i < OUTPUTS; i++){
//            printf("%f\t", answer[i]);
//        }
//        printf("\n");

        // Save neural to output
//        FILE*fd = fopen([[outputDir stringByAppendingPathComponent:@"neural1.genann"] cStringUsingEncoding:NSUTF8StringEncoding], "w");
//        genann_write(ann, fd);
//        genann_free(ann);
    }

    return 0;
}
