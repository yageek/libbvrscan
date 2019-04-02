import numpy
import scipy.special
import os
from os import listdir
from os.path import isfile, join
import struct
from skimage import io
from skimage import color
import matplotlib.pyplot as plt

# neural network class definition
class neuralNetwork:
    
    
    # initialise the neural network
    def __init__(self, inputnodes, hiddennodes, outputnodes, learningrate):
        # set number of nodes in each input, hidden, output layer
        self.inodes = inputnodes
        self.hnodes = hiddennodes
        self.onodes = outputnodes
        
        # link weight matrices, wih and who
        # weights inside the arrays are w_i_j, where link is from node i to node j in the next layer
        # w11 w21
        # w12 w22 etc 
        self.wih = numpy.random.normal(0.0, pow(self.inodes, -0.5), (self.hnodes, self.inodes))
        self.who = numpy.random.normal(0.0, pow(self.hnodes, -0.5), (self.onodes, self.hnodes))

        # learning rate
        self.lr = learningrate
        
        # activation function is the sigmoid function
        self.activation_function = lambda x: scipy.special.expit(x)
        
        pass

    
    # train the neural network
    def train(self, inputs_list, targets_list):
        # convert inputs list to 2d array
        inputs = numpy.array(inputs_list, ndmin=2).T
        targets = numpy.array(targets_list, ndmin=2).T
        
        # calculate signals into hidden layer
        hidden_inputs = numpy.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        hidden_outputs = self.activation_function(hidden_inputs)
        
        # calculate signals into final output layer
        final_inputs = numpy.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        final_outputs = self.activation_function(final_inputs)
        
        # output layer error is the (target - actual)
        output_errors = targets - final_outputs
        # hidden layer error is the output_errors, split by weights, recombined at hidden nodes
        hidden_errors = numpy.dot(self.who.T, output_errors) 
        
        # update the weights for the links between the hidden and output layers
        self.who += self.lr * numpy.dot((output_errors * final_outputs * (1.0 - final_outputs)), numpy.transpose(hidden_outputs))
        
        # update the weights for the links between the input and hidden layers
        self.wih += self.lr * numpy.dot((hidden_errors * hidden_outputs * (1.0 - hidden_outputs)), numpy.transpose(inputs))
        
        pass

    
    # query the neural network
    def query(self, inputs_list):
        # convert inputs list to 2d array
        inputs = numpy.array(inputs_list, ndmin=2).T
        
        # calculate signals into hidden layer
        hidden_inputs = numpy.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        hidden_outputs = self.activation_function(hidden_inputs)
        
        # calculate signals into final output layer
        final_inputs = numpy.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        final_outputs = self.activation_function(final_inputs)
        
        return final_outputs
    
    # Save the network
    def save(self, file_path):
        numpy.savetxt(file_path+"_wih.txt", self.wih, delimiter=",", header=f'{self.wih.shape}')
        numpy.savetxt(file_path+"_who.txt", self.who, delimiter=",", header=f'{self.who.shape}')
    def save_as_c(self, file_name):
        # Write header first
        out = open(f"{file_name}.h", "w")
        out.write("#ifndef __HEADER_NET_CONFIG\n")
        out.write("#define __HEADER_NET_CONFIG\n\n\n")
        out.write("#include \"matrix/matrix.h\"\n\n\n")
        
        out.write("extern double bvr_neural_wih_content[];\n")
        out.write("extern double bvr_neural_woh_content[];\n")


        out.write("#endif\n")
        out.close() 

        # Write content
        c = open(f"{file_name}.c", "w")
        # wih
        wih_out = ",".join([ f"{x}" for x in self.wih.flatten().tolist()])
        c.write(f"double bvr_neural_wih_content[] = {{ {wih_out} }};\n")

        # woh
        who_out = ",".join([ f"{x}" for x in self.who.flatten().tolist()])
        c.write(f"double bvr_neural_woh_content[] = {{ {who_out} }};\n")


def load_values(file_path):
    img_array = imageio.imread(file_path, as_gray=True)
    # img_data = (img_array/255.0*0.99)+0.01
    return img_array
    

# # Start loading the assets
# assets_dir = os.path.dirname(os.path.realpath(__file__)) + "/../generated"
# assets_files = {}
# original_assets = {}
# input_index = {}

# i = 0
# for file in listdir(assets_dir):
#     name, ext = os.path.splitext(file)
#     abs_path = join(assets_dir, file)
#     if isfile(abs_path) and ext == ".png":
#         assets_files[name] = load_values(abs_path)
#         input_index[name] = i
#         i += 1

# # number of input, hidden and output nodes
# input_nodes = 1024
# hidden_nodes = 100
# output_nodes = 13

# # # learning rate is 0.3
# learning_rate = 0.3

# # create instance of neural network
# n = neuralNetwork(input_nodes,hidden_nodes,output_nodes,
# learning_rate)

# # Train some amount
# # epochs = 1000
# # for e in range(epochs):    
# #     for key in assets_files.keys():
# #         # Prepare the value
# #         inputs = numpy.asfarray(assets_files[key]).reshape(1024)
# #         targets = numpy.zeros(13)
# #         targets[input_index[key]] = 1
# #         n.train(inputs, targets)

# # img_array = imageio.imread("0_sized.png", as_gray=True)
# # img_data = img_array.reshape(1024)
# # res = n.query(img_data)
# # print("Test 1", res)

# # Test
# #print("Output:", input_index)
# #print("Test 1", n.query(assets_files["1"]))

# # In our case who -> (13, 100)
# # In our case wih -> (100, 1024)
# # n.save_as_c("neural_v1")

# plt.imshow(assets_files["0"])
# plt.show()

img = io.imread("../generated/0.png")
# img_array = img*255/0.99 + 0.1
img_array = img
plt.imshow(img_array)
plt.show()