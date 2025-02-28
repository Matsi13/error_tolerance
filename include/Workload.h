#ifndef WORKLOAD_H
#define WORKLOAD_H

class Workload{
    // base class for all tasks, like LLM, CNN, etc.
    public:
        Workload(){};
        Workload(float TFLOPs, float paramsize, float access, float traffic);
        Workload(const Workload &obj);
        Workload& operator = (const Workload &obj);

        virtual const float get_TFLOPs() const;
        virtual const float get_paramsize() const;
        virtual const float get_access() const;
        virtual const float get_traffic() const;

        void set_TFLOPs(float TFLOPs);
        void set_paramsize(float paramsize);
        void set_access(float access);
        void set_traffic(float traffic);

    private:
        float TFLOPs;       // total compute requirement of the task
        float paramsize;    // total capacity requirement of the task
        float access;       // total memory access requirement of the task
        float traffic;      // total communication requirement of the task
};

#endif