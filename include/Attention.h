#ifndef ATTENTION_H
#define ATTENTION_H

#include "Macro.h"
#include "Workload.h"

class Attention : public Workload{

    public:
        Attention():Workload(){};
        Attention(int d_model, int head_num, int d_q, int d_k, int d_v, int d_hidden, int prompt_avg, int output_avg, int norm_function, int act_function, int mode);
        Attention(const Attention &obj);
        Attention& operator = (const Attention &obj);

        virtual const float get_TFLOPs()const;
        virtual const float get_paramsize()const;
        virtual const float get_access()const;
        virtual const float get_traffic()const;
 
        const float get_kvcache_size()const;
        const float get_model_size()const;

        const int get_d_model()const;
        const int get_head_num()const;
        const int get_d_q()const;
        const int get_d_k()const;
        const int get_d_v()const;
        const int get_d_hidden()const;
        const int get_prompt_avg()const;
        const int get_output_avg()const;
        const int get_act_function()const;
        const int get_norm_function()const;
        const int get_mode()const;

        void set_d_model(int d_model);
        void set_head_num(int head_num);
        void set_d_q(int d_q);
        void set_d_k(int d_k);
        void set_d_v(int d_v);
        void set_d_hidden(int d_hidden);
        void set_prompt_avg(int prompt_avg);
        void set_output_avg(int output_avg);
        void set_norm_function(int norm_function);
        void set_act_function(int act_function);
        void set_mode(int mode);
        
    private:
        int d_model;
        int head_num;
        int d_q;
        int d_k;
        int d_v;
        int d_hidden;
        int prompt_avg;
        int output_avg;
        int norm_function;
        int act_function;
        int mode;

        float kvcache_size;
        float model_size;

        void update();
        void update_TFLOPs();
        void update_paramsize();
        void update_access();
        void update_traffic();

        float cal_norm_TFLOPs();

};

#endif