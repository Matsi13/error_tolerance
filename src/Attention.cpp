#include <string>

#include "Workload.h"
#include "Attention.h"

using namespace std;


Attention::Attention(int d_model, int head_num, int d_k, int d_v, int d_hidden, int prompt_avg, int output_avg, string norm_function, string act_function) : Workload(){

    this->d_model = d_model;
    this->head_num = head_num;
    this->d_k = d_k;
    this->d_v = d_v;
    this->d_hidden = d_hidden;
    this->prompt_avg = prompt_avg;
    this->output_avg = output_avg;
    this->norm_function = norm_function;
    this->act_function = act_function;

    update();

}


Attention::Attention(const Attention &obj) : Workload(obj){

    this->d_model = obj.get_d_model();
    this->head_num = obj.get_head_num();
    this->d_k = obj.get_d_k();
    this->d_v = obj.get_d_v();
    this->d_hidden = obj.get_d_hidden();
    this->prompt_avg = obj.get_prompt_avg();
    this->output_avg = obj.get_output_avg();
    this->norm_function = obj.get_norm_function();
    this->act_function = obj.get_act_function();

}


Attention& Attention::operator = (const Attention &obj){

    if (this != &obj){

        Workload::operator=(obj);
        this->d_model = obj.get_d_model();
        this->head_num = obj.get_head_num();
        this->d_k = obj.get_d_k();
        this->d_v = obj.get_d_v();
        this->d_hidden = obj.get_d_hidden();
        this->prompt_avg = obj.get_prompt_avg();
        this->output_avg = obj.get_output_avg();
        this->norm_function = obj.get_norm_function();
        this->act_function = obj.get_act_function();

    }

    return *this;
    
}


Attention::~Attention(){

    norm_function.clear();
    act_function.clear();

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


const string Attention::get_act_function()const{

    return act_function;
    
}


const string Attention::get_norm_function()const{

    return norm_function;
    
}


void Attention::set_d_model(int d_model){

    this->d_model = d_model;

}


void Attention::set_head_num(int head_num){

    this->head_num = head_num;

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


void Attention::set_act_function(string act_function){

    this->act_function = act_function;

}


void Attention::set_norm_function(string norm_function){

    this->norm_function = norm_function;

}


void Attention::update_TFLOPs(){

    


}


void Attention::update(){

    void update_TFLOPs();
    void update_paramsize();
    void update_access();
    void update_traffic();

}