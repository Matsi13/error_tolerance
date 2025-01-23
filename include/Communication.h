#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
    public:
        Communication(){};
        Communication(float* sizes, float bandwidth, float padding);
        Communication(const Communication &obj);

        Communication& operator = (const Communication &obj);

        const float get_size(int index)const;
        const float get_bandwidth()const;
        const float get_padding()const;

        void set_size(float size, int index);
        void set_bandwidth(float bandwidth);
        void set_padding(float padding);
    private:
        float sizes[2];  // measured by mm. size[0] is the edge with links.
        float bandwidth; // TB/s
        float padding;   // mm
};

#endif