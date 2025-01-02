#ifndef DIE_H 
#define DIE_H

#include <string>
#include <algorithm>

#include "Compute.h"
#include "Communication.h"
#include "Memory.h"
#include "Macro.h"

using namespace std;

// class Die{
//     public: 
//         Die(float padding, Compute Compute_unit, Memory Memory_unit, Communication Communication_unit, string up, string down, string left, string right);
//         Die(const Die& obj);
//         ~Die();

//         const float get_size(int index)const;
//         const float get_padding()const;
//         const float get_tflops()const;
//         const float get_capacity()const;
//         const float get_memory_bandwidth()const;
//         const float get_communication_bandwidth()const;
//         const Compute get_Compute_unit()const;
//         const Memory get_Memory_unit()const;
//         const Communication get_Communication_unit()const;
//         const string get_permutation_up()const;
//         const string get_permutation_down()const;
//         const string get_permutation_left()const;
//         const string get_permutation_right()const;


//     private:
//         float sizes[2];
//         float padding;
//         float tflops;
//         float capacity;
//         float memory_bandwidth;
//         float communication_bandwidth;
//         Compute Compute_unit;
//         Memory Memory_unit;
//         Communication Communication_unit;

//         // unit permutation on four edges of the die is represented by a string
//         string up; 
//         string down; 
//         string left;
//         string right;

//         void update(); // update size, padding, tflops, capacity and bandwidth based on the new permutation

    


// };

#endif