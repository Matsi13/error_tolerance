#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
    public:
        Communication(float* sizes, float bandwidth, float padding);
        float get_size(int index);
        float get_bandwidth();
        float get_padding();
        void set_size(float size, int index);
        void set_bandwidth(float bandwidth);
        void set_padding(float padding);
    private:
        float sizes[2];  // mm
        float bandwidth; // TB/s
        float padding;   // mm
};

#endif