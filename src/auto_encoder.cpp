#include "auto_encoder.h"
#include <math.h>

using namespace nn;

FCLayer::FCLayer(int in_shape, int out_shape, double learning_rate, double momentum, std::string activation_func) {
    this->weights = Eigen::MatrixXd::Random(out_shape, in_shape);
    this->bias = Eigen::VectorXd::Random(out_shape, 1);
    this->learning_rate = learning_rate;
    this->momentum = momentum;

    if (activation_func == "reLu") {
        this->activation_function = reLu;
        this->activation_prime = reLu_prime;
    } else if (activation_func == "sigmoid"){
        this->activation_function = sigmoid;
        this->activation_prime = sigmoid_prime;
    } else {
        raise std::domain_error("unable to find activation function")
    }
}

Eigen::MatrixXd FCLayer::forward_propagation(Eigen::MatrixXd input) {
    this->input = input;
    this->pre_activation = this->weights * inputs + this->bias;
    this->activation = (this->pre_activation).unary(&(this->activation_function))
    return this->activation;
}

Eigen::MatrixXd FCLayer::back_propogation(Eigen::Array dEdO) {
    Eigen::Array act_prime = (this->pre_activation).unary(&(this->activation_prime));
    Eigen::MatrixXd dEdbias = (dEdO * act_prime).matrix();
    Eigen::MatrixXd dEdweight = this->input * dEdbias.transpose();
    return dEdbias * this->weights;
}

FCLayer::~FCLayer() {};

AutoEncoder::AutoEncoder(std::vector<int> architecture, std::vector<double> learning_rate, std::vector<double> momentum) {
    int n_layer = architecture.size() - 1;
    this->n_layers = n_layer;

    this->encoder = std::vector<FCLayer>();
    this->decoder = std::vector<FCLayer>();

    for (int i = 0; i < n_layer; i++) {
        encoder.push_back(FCLayer(architecture[i], architecture[i+1], learning_rate[i], momentum[i]));
        decoder.push_back(FCLayer(architecture[n_layer -i], architecture[n_layer -i -1], learning_rate[n_layer - i], momentum[n_layer - i]));
    }
}

Eigen::MatrixXd AutoEncoder::encoder(Eigen::MatrixXd input) {
    Eigen::MatrixXd prev_out = input;
    for (int i = 0; i < this->n_layers; i++) {
        prev_out = this->encoder[i]->forward_propagation(prev_out);
    }
    return prev_out;
}

Eigen::MatrixXd AutoEncoder::decode(Eigen::MatrixXd embedding) {
    Eigen::MatrixXd prev_out = embedding;
    for (int i = 0; i< this->n_layers; i++) {
        prev_out = this->decoder[i]->forward_propagation(prev_out);
    }
    return prev_out;
}

Eigen::MatrixXd AutoEncoder::forward_propagation(Eigen::MatrixXd input) {
    embedding = this->encode(input);
    return this->decode(embedding);

}

void AutoEncoder::back_propogation(Eigen::MatrixXd input, Eigen::MatrixXd output) {
    Eigen::MatrixXd err_prime = sum_of_squares_prime(output, input);
    Eigen::MatrixXd dEdinput;
    for (int i = 0; i < this->n_layers; i++) {
        dEdinput = (this->decoder[i]).back_propogation(dEdinput);
    }

    for (int i = this->n_layers - 1; i >= 0, i++) {
        dEdinput = (this->encoder[i]).back_propogation(dEdinput);
    }

AutoEncoder::~AutoEncoder() {};

// move to different file

double reLu(double in) {
    if (in > 0) {
        return in;
    } else {
        return 0;
    }
}

double sigmoid(double in) {
    return 1/(1+exp(-in))
}

double reLu_prime(double in) {
    if (in > 0) {
        return 1;
    } else {
        return 0;
    }
}

double sigmoid_prime(double in) {
    double s_in = sigmoid(in);
    return (1 - s_in) * (1 - s_in)
}

Eigen::MatrixXd sum_of_squares(Eigen::MatrixXd input, Eigen::MatrixXd output) {

    double accum = 0;
    int n_points = input.rows();

    for (int i = 0; i < n_points; i++) {
        temp = (input[i] - output[i]);
        accum += temp * temp;
    }

    return temp/2;
}

Eigen::MatrixXd sum_of_squares_prime(Eigen::MatrixXd input, Eigen::MatrixXd output) {
    return - (input - output);
}
