
#include "genann.h"
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

        NSDirectoryEnumerator *en = [[NSFileManager defaultManager] enumeratorAtPath:inputDir];
        NSString *file;

        NSMutableDictionary<NSString *, NSData*> *asset_data = [NSMutableDictionary dictionary];
        NSMutableDictionary<NSString *, NSNumber*> *input_index = [NSMutableDictionary dictionary];


        // Load the inputs
        NSInteger i = 0;
        double current_input[INPUTS];
        double current_output[OUTPUTS];
        while ((file = [en nextObject])) {

            if(![file.pathExtension isEqualToString:@"ocrb"]) {
                continue;
            }

            // Load the file data
            NSData *data = [NSData dataWithContentsOfFile:[inputDir stringByAppendingPathComponent:file]];
            asset_data[[file stringByDeletingPathExtension]] = data;
            input_index[file] = @(i);
            i++;
        }

        // Train the neural network
        for(NSString *key in asset_data) {


            // Load the input buffers
            for(NSInteger i = 0; i < INPUTS; i++) {
                NSData *current = asset_data[key];
                uint8_t *in_ptr = (uint8_t*)(current.bytes);
                current_input[i] = in_ptr[i];
            }

            // Load the output buffers
            memset(current_output, 0, OUTPUTS);
            int numberIndex = [input_index[key] intValue];
            current_output[numberIndex] = 1;
            genann_train(ann, current_input, current_output, LEARNING_RATE);
        }


        // Test
        const double* answer = genann_run(ann, current_input);
        for (NSInteger i=0; i < OUTPUTS; i++){
            printf("%f\t", answer[i]);
        }
        printf("\n");

        // Save neural to output
        FILE*fd = fopen([[outputDir stringByAppendingPathComponent:@"neural1.genann"] cStringUsingEncoding:NSUTF8StringEncoding], "w");
        genann_write(ann, fd);
        genann_free(ann);
    }

    return 0;
}
