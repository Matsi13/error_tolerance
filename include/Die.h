#ifndef DIE_H 
#define DIE_H

#include <string>
#include <algorithm>

#include "Compute.h"
#include "Communication.h"
#include "Memory.h"
#include "Macro.h"

using namespace std;

class Die{
    public: 
        Die(){};
        Die(float padding, float bandwidth_per_area, float memory_bandwidth_ratio, Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, string& up, string& down, string& left, string& right);
        Die(const Die& obj);
        Die& operator = (const Die &obj);
        ~Die();

        const float get_size(int index)const;
        const float get_padding()const;
        const float get_bandwidth_per_area()const;
        const float get_memory_bandwidth_ratio()const;
        const float get_TFLOPS()const;
        const float get_SRAM_capacity()const;
        const float get_DRAM_capacity()const;
        const float get_memory_bandwidth()const;
        const float get_communication_bandwidth()const;
        const Compute get_Compute_unit()const;
        const Memory get_Memory_unit()const;
        const Communication get_Communication_unit()const;
        const string get_permutation_up()const;
        const string get_permutation_down()const;
        const string get_permutation_left()const;
        const string get_permutation_right()const;

        void set_padding(float padding); 
        void set_bandwidth_per_area(float bandwidth_per_area);
        void set_memory_bandwidth_ratio(float memory_bandwidth_ratio);  
        void set_Compute_unit(Compute &new_Compute_unit); 
        void set_Memory_unit(Memory &new_Memory_unit);
        void set_Communication_unit(Communication &new_Communication_unit);   
        void set_permutation_up(string &new_permutation);
        void set_permutation_down(string &new_permutation);
        void set_permutation_left(string &new_permutation);
        void set_permutation_right(string &new_permutation);

        void print();

    private:
        float sizes[2];
        float padding;
        float TFLOPS;
        float SRAM_capacity;
        float DRAM_capacity;
        float memory_bandwidth;
        float communication_bandwidth;
        float bandwidth_per_area; // in 2.5D package, the bandwidth available is limited by the area of compute core
        float memory_bandwidth_ratio; // the most percentage of bandwidth that can be used for memory access 
        Compute Compute_unit;
        Memory Memory_unit;
        Communication Communication_unit;

        // unit permutation on four edges of the die is represented by a string
        string up; 
        string down; 
        string left;
        string right;

        void set_size(float size, int index);
        void set_TFLOPS(float TFLOPS);
        void set_SRAM_capacity(float capacity);
        void set_DRAM_capacity(float capacity);

        void set_memory_bandwidth(float memory_bandwidth);
        void set_communication_bandwidth(float communication_bandwidth);
        
        void update_size(); // update size based on all units
        void update_TFLOPS(); // update TFLOPS based on compute unit
        void update_SRAM_capacity(); // update capacity based on memory units
        void update_DRAM_capacity(); // update capacity based on memory units
        void update_memory_bandwidth(); // update memory bandwidth based on based on memory units
        void update_communication_bandwidth(); // update communication bandwidth based on communication units
        void update_bandwidth(); // update memory and communication bandwidth with thershold
        bool check_reticle_limit(); // check if die size exceeds reticle limit (33mm * 28mm)
        void apply_d2d_bandwidth_constraints(); // apply D2D bandwidth based on die perimeter
        void update(); // update size, padding, TFLOPS, capacity and bandwidth based on the new permutation
        
        // Constants for die size constraints
        static constexpr float MAX_DIE_LENGTH = 33.0f; // mm
        static constexpr float MAX_DIE_WIDTH = 28.0f;   // mm
        
        // Constants for D2D bandwidth calculation
        static constexpr float REFERENCE_PERIMETER = 80.0f; // 20mm * 4mm
        static constexpr float REFERENCE_BANDWIDTH = 20.0f; // TB/s for 20mm*20mm die
        static constexpr float MAX_PERIMETER = 122.0f;      // 2 * (33mm + 28mm)
        static constexpr float MIN_BANDWIDTH = 10.0f;       // TB/s for 33mm*28mm die

};

#endif
