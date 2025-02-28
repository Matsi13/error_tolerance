#ifndef ATTENTION_H
#define ATTENTION_H

#include <string>

#include "Workload.h"

class Attention : public Workload{

    public:
        Attention():Workload(){};
        Attention(int d_model, int head_num, int d_k, int d_v, int d_hidden, int prompt_avg, int output_avg, string norm_function, string act_function);
        Attention(const Attention &obj);
        Attention& operator = (const Attention &obj);
        ~Attention();

        virtual const float get_TFLOPs()const;
        virtual const float get_paramsize()const;
        virtual const float get_access()const;
        virtual const float get_traffic()const;

        const int get_d_model()const;
        const int get_head_num()const;
        const int get_d_k()const;
        const int get_d_v()const;
        const int get_d_hidden()const;
        const int get_prompt_avg()const;
        const int get_output_avg()const;
        const string get_act_function()const;
        const string get_norm_function()const;

        void set_d_model(int d_model);
        void set_head_num(int head_num);
        void set_d_k(int d_k);
        void set_d_v(int d_v);
        void set_d_hidden(int d_hidden);
        void set_prompt_avg(int prompt_avg);
        void set_output_avg(int output_avg);
        void set_norm_function(string norm_function);
        void set_act_function(string act_function);
        
    private:
        int d_model;
        int head_num;
        int d_k;
        int d_v;
        int d_hidden;
        int prompt_avg;
        int output_avg;
        string norm_function;
        string act_function;

        void update();
        void update_TFLOPs();
        void update_paramsize();
        void update_access();
        void update_traffic();

};

#endif