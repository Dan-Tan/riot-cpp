#include "auto_encoder.h"

using namespace nn;

FCLayer::FCLayer(int in_shape, int out_shape, double learning_rate, double momentum) {
    this->weights = Eigen::MatrixXd::Random(out_shape, in_shape);
    this->bias = Eigen::VectorXd::Random(out_shape, 1);
    this->learning_rate = learning_rate;
    this->momentum = momentum;
}

Eigen::MatrixXd FCLayer::forward_propagation(Eigen::MatrixXd input) {
    return this->weights * input + this->bias;
}

Eigen::MatrixXd FCLayer::back_propogation(Eigen::MatrixXd input) {

}

FCLayer::~FCLayer() {};

AutoEncoder::AutoEncoder(std::vector<int> architecture, std::vector<double> learning_rate, std::vector<double> momentum) {
    int n_layer = architecture.size() - 1;

    this->encoder = std::vector<FCLayer>();
    this->decoder = std::vector<FCLayer>();

    for (int i = 0; i < n_layer; i++) {
        encoder.push_back(FCLayer(architecture[i], architecture[i+1], learning_rate[i], momentum[i]));
        decoder.push_back(FCLayer(architecture[n_layer -i], architecture[n_layer -i -1], learning_rate[n_layer - i], momentum[n_layer - i]));
    }
}

Eigen::MatrixXd AutoEncoder::forward_propagation(Eigen::MatrixXd input) {

}

Eigen::MatrixXd AutoEncoder::back_propogation(Eigen::MatrixXd input) {

}

AutoEncoder::~AutoEncoder() {};
