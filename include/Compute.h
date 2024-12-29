#ifndef COMPUTE_H
#define COMPUTE_H

class Compute{
    public:
        Compute(float* sizes, float flops);
        float get_size(int index);
        float get_flops();
        void set_size(float size, int index);
        void set_flops(float flops);
        
    private:
        float sizes[2];  // mm
        float flops; // float operations per second

};

#endif