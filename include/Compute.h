#ifndef COMPUTE_H
#define COMPUTE_H

class Compute{
    public:
        Compute(float* sizes, float tflops, float padding);
        Compute(const Compute &obj);
        const float get_size(int index)const;
        const float get_tflops()const;
        const float get_padding()const;
        void set_size(float size, int index);
        void set_tflops(float tflops);
        void set_padding(float padding);
        
    private:
        float sizes[2];  // mm
        float tflops; // tera float operations per second
        float padding;   // mm

};

#endif