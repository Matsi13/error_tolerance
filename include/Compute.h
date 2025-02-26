#ifndef COMPUTE_H
#define COMPUTE_H

class Compute{
    public:
        Compute(){};
        Compute(float* sizes, float TFLOPS, float padding);
        Compute(const Compute &obj);

        Compute& operator = (const Compute &obj);

        const float get_size(int index)const;
        const float get_TFLOPS()const;
        const float get_padding()const;

        void set_size(float size, int index);
        void set_TFLOPS(float TFLOPS);
        void set_padding(float padding);
        
    private:
        float sizes[2];  // measured by mm. size[0] is the length, size[1] is the width
        float TFLOPS; // tera float operations per second
        float padding;   // The padding between the compute chip and memory or communication units 

};

#endif