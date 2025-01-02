#ifndef COMPUTE_H
#define COMPUTE_H

class Compute{
    public:
        Compute(float* sizes, float flops, float padding);
        float get_size(int index);
        float get_flops();
        float get_padding();
        void set_size(float size, int index);
        void set_flops(float flops);
        void set_padding(float padding);
        
    private:
        float sizes[2];  // mm
        float flops; // float operations per second
        float padding;   // mm

};

#endif