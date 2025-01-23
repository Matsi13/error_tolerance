#ifndef COMPUTE_H
#define COMPUTE_H

class Compute{
    public:
        Compute(){};
        Compute(float* sizes, float tflops, float padding);
        Compute(const Compute &obj);

        Compute& operator = (const Compute &obj);

        const float get_size(int index)const;
        const float get_tflops()const;
        const float get_padding()const;

        void set_size(float size, int index);
        void set_tflops(float tflops);
        void set_padding(float padding);

        
    private:
        float sizes[2];  // measured by mm. size[0] is the length, size[1] is the width
        float tflops; // tera float operations per second
        float padding;   // mm

};

#endif