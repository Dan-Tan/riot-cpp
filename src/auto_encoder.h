#pragma once
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

namespace nn {

    class FCLayer {

        public:
            FCLayer(int in_shape, int out_shape, double learning_rate, double momentum, std::string activation_func);
            ~FCLayer();

            Eigen::MatrixXd forward_propagation(Eigen::MatrixXd input);
            Eigen::MatrixXd back_propogation(Eigen::MatrixXd input);

        private:
            double learning_rate;
            double momentum;

            int in_size;
            int out_size;

            Eigen::MatrixXd weights;
            Eigen::MatrixXd bias;

            double (*activation_function)(double);
            double (*activation_prime)(double);

            Eigen::MatrixXd pre_activation;
            Eigen::MatrixXd activation;

    };

    class AutoEncoder {

        public:
            AutoEncoder(std::vector<int> architecture, std::vector<double> learning_rate, std::vector<double> momentum);
            ~AutoEncoder(); 

            Eigen::MatrixXd forward_propagation(Eigen::MatrixXd input);
            Eigen::MatrixXd back_propogation(Eigen::MatrixXd input);

            Eigen::MatrixXd encode(Eigen::MatrixXd input);
            Eigen::MatrixXd decode(Eigen::MatrixXd embedding);

            void read_params(std::string path);
            void save_params(std::string path);

        private:
            std::vector<FCLayer> encoder;
            std::vector<FCLayer> decoder;

            int n_layers;

    };
}
