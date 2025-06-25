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
    int d_hidden = 2048;
    int prompt_avg = 64;
    int output_avg = 5818;
    int norm_function = RMSNORM;
    int act_function = RELU;
    int mode = INFERENCE;

    Attention layer(d_model, head_num, d_q, d_k, d_v, d_hidden, prompt_avg, output_avg, norm_function, act_function, mode);
    cout << layer.get_TFLOPs() << endl;
    cout << layer.get_paramsize() << endl;
    cout << layer.get_access() << endl;
    cout << layer.get_traffic() << endl;
    cout << layer.get_kvcache_size() << endl;
    cout << layer.get_model_size() << endl;

    return 0;

}
