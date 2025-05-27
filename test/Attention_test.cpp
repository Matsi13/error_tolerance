#include <cassert>
#include <iostream>

#include "Macro.h"
#include "Workload.h"
#include "Attention.h"

using namespace std;

int main(){

    int d_model = 512;
    int head_num = 8;
    int d_q = 64;
    int d_k = 64;
    int d_v = 64;
    int d_hidden = 1024;
    int prompt_avg = 1024;
    int output_avg = 10240;
    int norm_function = RMSNORM;
    int act_function = RELU;
    int mode = INFERENCE;

    Attention layer(d_model, head_num, d_q, d_k, d_v, d_hidden, prompt_avg, output_avg, norm_function, act_function, mode);
    cout << layer.get_TFLOPs() << endl;
    cout << layer.get_paramsize() << endl;
    cout << layer.get_access() << endl;
    cout << layer.get_traffic() << endl;

    // Attention layer1(layer);
    // cout << layer1.get_TFLOPs() << endl;
    
    // Attention layer2 = layer;
    // cout << layer2.get_TFLOPs() << endl;

    return 0;

}
