#include "Macro.h"
#include "Workload.h"
#include "Attention.h"

using namespace std;


Attention::Attention(int d_model, int head_num, int d_q, int d_k, int d_v, int d_hidden, int prompt_avg, int output_avg, int norm_function, int act_function, int mode) : Workload(){

    this->d_model = d_model;
    this->head_num = head_num;
    this->d_q = d_q;
    this->d_k = d_k;
    this->d_v = d_v;
    this->d_hidden = d_hidden;
    this->prompt_avg = prompt_avg;
    this->output_avg = output_avg;
    this->norm_function = norm_function;
    this->act_function = act_function;
    this->mode = mode;

    update();

}


Attention::Attention(const Attention &obj) : Workload(obj){

    this->d_model = obj.get_d_model();
    this->head_num = obj.get_head_num();
    this->d_q = obj.get_d_q();
    this->d_k = obj.get_d_k();
    this->d_v = obj.get_d_v();
    this->d_hidden = obj.get_d_hidden();
    this->prompt_avg = obj.get_prompt_avg();
    this->output_avg = obj.get_output_avg();
    this->norm_function = obj.get_norm_function();
    this->act_function = obj.get_act_function();
    this->mode = obj.get_mode();

    update();

}


Attention& Attention::operator = (const Attention &obj){

    if (this != &obj){

        Workload::operator=(obj);
        this->d_model = obj.get_d_model();
        this->head_num = obj.get_head_num();
        this->d_q = obj.get_d_q();
        this->d_k = obj.get_d_k();
        this->d_v = obj.get_d_v();
        this->d_hidden = obj.get_d_hidden();
        this->prompt_avg = obj.get_prompt_avg();
        this->output_avg = obj.get_output_avg();
        this->norm_function = obj.get_norm_function();
        this->act_function = obj.get_act_function();
        this->mode = obj.get_mode();

        update();

    }

    return *this;
    
}


const float Attention::get_TFLOPs()const{

    return Workload::get_TFLOPs();

}


const float Attention::get_paramsize()const{

    return Workload::get_paramsize();

}


const float Attention::get_access()const{

    return Workload::get_access();

}


const float Attention::get_traffic()const{

    return Workload::get_traffic();

}


const int Attention::get_d_model()const{

    return d_model;

}


const int Attention::get_head_num()const{

    return head_num;

}


const int Attention::get_d_q()const{

    return d_q;

}


const int Attention::get_d_k()const{

    return d_k;

}


const int Attention::get_d_v()const{

    return d_v;
    
}


const int Attention::get_d_hidden()const{

    return d_hidden;
    
}


const int Attention::get_prompt_avg()const{

    return prompt_avg;
    
}


const int Attention::get_output_avg()const{

    return output_avg;
    
}


const int Attention::get_act_function()const{

    return act_function;
    
}


const int Attention::get_norm_function()const{

    return norm_function;
    
}


const int Attention::get_mode()const{

    return mode;
    
}


void Attention::set_d_model(int d_model){

    this->d_model = d_model;

}


void Attention::set_head_num(int head_num){

    this->head_num = head_num;

}


void Attention::set_d_q(int d_q){

    this->d_q = d_q;

}


void Attention::set_d_k(int d_k){

    this->d_k = d_k;

}


void Attention::set_d_v(int d_v){

    this->d_v = d_v;

}


void Attention::set_d_hidden(int d_hidden){

    this->d_hidden = d_hidden;

}


void Attention::set_prompt_avg(int prompt_avg){

    this->prompt_avg = prompt_avg;

}


void Attention::set_output_avg(int output_avg){

    this->output_avg = output_avg;

}


void Attention::set_act_function(int act_function){

    this->act_function = act_function;

}


void Attention::set_norm_function(int norm_function){

    this->norm_function = norm_function;

}


void Attention::set_mode(int mode){

    this->mode = mode;

}


float Attention::cal_norm_TFLOPs(){

    float norm_TFLOPs = 0;

    switch(norm_function) {

        case LAYERNORM:{

            norm_TFLOPs = d_model * 8 * 1e-12;
            break;

        }
        case RMSNORM:{

            norm_TFLOPs = d_model * 4 * 1e-12;
            break;

        }
        default : break;
    }

    return norm_TFLOPs;


}


void Attention::update_TFLOPs(){

    float norm_prefill_TFLOPs = prompt_avg * cal_norm_TFLOPs();
    float linear_0_prefill_TFLOPs = prompt_avg * d_model * d_model * 2 * 1e-12;
    float q_prefill_TFLOPs = prompt_avg * d_model * d_q * head_num * 2 * 1e-12;
    float k_prefill_TFLOPs = prompt_avg * d_model * d_k * head_num * 2 * 1e-12;
    float v_prefill_TFLOPs = prompt_avg * d_model * d_v * head_num * 2 * 1e-12;
    float RoPE_prefill_TFLOPs = prompt_avg * (d_k + d_q) * head_num * 9 * 1e-12; 
    float attention_score_prefill_TFLOPs = prompt_avg * d_k * prompt_avg * head_num * 2 * 1e-12;
    float softmax_prefill_TFLOPs = prompt_avg * prompt_avg * head_num * 3 * 1e-12;
    float dot_product_prefill_TFLOPs = prompt_avg * prompt_avg * d_v * head_num * 1e-12;
    float linear_1_prefill_TFLOPs = prompt_avg * d_v * head_num * d_hidden * 2 * 1e-12;
    float add_residul_prefill_TFLOPs = prompt_avg * head_num * d_hidden * 1e-12;

    float prefill_TFLOPs = norm_prefill_TFLOPs + linear_0_prefill_TFLOPs + q_prefill_TFLOPs + k_prefill_TFLOPs + v_prefill_TFLOPs + RoPE_prefill_TFLOPs + attention_score_prefill_TFLOPs + dot_product_prefill_TFLOPs + linear_1_prefill_TFLOPs + add_residul_prefill_TFLOPs;

    float norm_decode_TFLOPs = output_avg * cal_norm_TFLOPs();
    float linear_0_decode_TFLOPs = output_avg * d_model * d_model * 2 * 1e-12;
    float q_decode_TFLOPs = output_avg * d_model * d_q * head_num * 2 * 1e-12;
    float k_decode_TFLOPs = output_avg * d_model * d_k * head_num * 2 * 1e-12;
    float v_decode_TFLOPs = output_avg * d_model * d_v * head_num * 2 * 1e-12;
    float RoPE_decode_TFLOPs = output_avg * (d_k + d_q) * head_num * 9 * 1e-12;
    float attention_score_decode_TFLOPs = d_k * head_num * (2 * prompt_avg + output_avg + 1) * output_avg * 1e-12; 
    float softmax_decode_TFLOPs = 3 * (2 * prompt_avg + 1 + output_avg) * output_avg;
    float dot_product_decode_TFLOPs = d_v * head_num * (2 * prompt_avg + 1 + output_avg) * output_avg * 1e-12;
    float linear_decode_TFLOPs = head_num * d_v * d_hidden  * (2 * prompt_avg + 1 + output_avg) * output_avg * 1e-12;

    float decode_TFLOPs = norm_decode_TFLOPs + linear_0_decode_TFLOPs + q_decode_TFLOPs + k_decode_TFLOPs + v_decode_TFLOPs + RoPE_decode_TFLOPs + attention_score_decode_TFLOPs + softmax_decode_TFLOPs + dot_product_decode_TFLOPs + linear_decode_TFLOPs;

    float total_TFLOPs = 0;

    switch(mode){

        case INFERENCE : {

            total_TFLOPs = prefill_TFLOPs + decode_TFLOPs;
            break;

        }
        case TRAINING : {

            total_TFLOPs = 3 * (prefill_TFLOPs + decode_TFLOPs);
            break;

        }
        default : break;
    }

    Workload::set_TFLOPs(total_TFLOPs);
    return;

}


void Attention::update_paramsize(){

    float KV_cache_paramsize = (d_k + d_v) * (prompt_avg + output_avg) * sizeof(float) * 1e-10;
    Workload::set_paramsize(KV_cache_paramsize);
    return;

}


void Attention::update_access(){

    float KV_cache_access = (d_k + d_v) * (2 * prompt_avg + output_avg) * (output_avg + 1) / 2 * sizeof(float) * 1e-9;
    float total_access = 0;
    switch(mode){

        case INFERENCE : {

            total_access = KV_cache_access; 
            break;

        }

        case TRAINING : {

            total_access = 2 * KV_cache_access;
            break;

        }

        default : break;
    }

    Workload::set_access(total_access);
    return;

}


void Attention::update_traffic(){

    float output_traffic = d_model * (prompt_avg + output_avg) * sizeof(float) * 1e-12;
    float total_traffic = 0;

    switch(mode){

        case INFERENCE : {

            total_traffic = output_traffic; 
            break;

        }

        case TRAINING : {

            total_traffic = output_traffic * 2;
            break;

        }

        default : break;

    }

    Workload::set_traffic(total_traffic);
    return;
}


void Attention::update(){

    update_TFLOPs();
    update_paramsize();
    update_access();
    update_traffic();

}

