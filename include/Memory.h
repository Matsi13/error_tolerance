#ifndef MEMORY_H
#define MEMORY_H

class Memory{
    public:
        Memory(float* sizes, float bandwidth, float capacity, float padding);
        Memory(const Memory &obj);
        const float get_size(int index)const;
        const float get_bandwidth()const;
        const float get_capacity()const;
        const float get_padding()const;
        void set_size(float size, int index);
        void set_bandwidth(float bandwidth);
        void set_capacity(float capacity);
        void set_padding(float padding);
    private:
        float sizes[2];  // mm
        float bandwidth; // GB/s
        float capacity;  // GB
        float padding;   // mm
};

#endif