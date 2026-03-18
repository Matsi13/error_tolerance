#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"

using namespace std;

int main(){

    float Compute_sizes[2] = {12,16};
    float TFLOPS = 10;
    float SRAM_capacity = 5;
    float Compute_padding = 0.5;
    Compute Compute_unit(Compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);

    float Memory_sizes[2] = {5, 5};
    float Memory_bandwidth = 4;
    float Memory_capacity = 24;
    float Memory_padding = 0.2;
    Memory Memory_unit(Memory_sizes, Memory_bandwidth, Memory_capacity, Memory_padding);

    float Communication_sizes[2] = {1, 2};
    float Communication_bandwidth = 1;
    float Communication_padding = 0.3;
    Communication Communication_unit(Communication_sizes, Communication_bandwidth, Communication_padding);

    float die_padding = 2;
    float bandwidth_per_area = 1;
    float memory_bandwidth_ratio = 0.6;
    string up = {MEMORY_UNIT, MEMORY_UNIT};
    string down = {MEMORY_UNIT, MEMORY_UNIT};
    string left = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};
    string right = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};

    Die Die_instance(die_padding, bandwidth_per_area, memory_bandwidth_ratio, Compute_unit, Memory_unit, Communication_unit, up, down, left, right);
    // cout << Die_instance.get_size(0) << endl;
    // cout << Die_instance.get_size(1) << endl;
    assert(Die_instance.get_size(0) == 26);
    assert(Die_instance.get_size(1) == 30);
    assert(Die_instance.get_padding() == 2);
    assert(Die_instance.get_TFLOPS() == 10);
    assert(Die_instance.get_SRAM_capacity() == 5);
    assert(Die_instance.get_DRAM_capacity() == 192);
    assert(Die_instance.get_memory_bandwidth() == 32);
    assert(Die_instance.get_communication_bandwidth() == 4);
    
    // Test new constraint functionality
    
    // Test 1: Reticle limit validation
    cout << "Testing reticle limit validation..." << endl;
    
    // Create a die that would exceed maximum size
    float large_compute_sizes[2] = {40, 35}; // Exceeds max 33x28
    Compute large_compute_unit(large_compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);
    string large_up = {MEMORY_UNIT, MEMORY_UNIT, MEMORY_UNIT};
    string large_down = "";
    string large_left = "";
    string large_right = "";
    
    Die large_die(die_padding, bandwidth_per_area, memory_bandwidth_ratio, 
                  large_compute_unit, Memory_unit, Communication_unit, 
                  large_up, large_down, large_left, large_right);
    
    // Check if die exceeds reticle limit
    bool is_valid = large_die.check_reticle_limit();
    cout << "Large die is within reticle limit: " << (is_valid ? "true" : "false") << endl;
    cout << "Large die size: " << large_die.get_size(0) 
         << " x " << large_die.get_size(1) << " mm" << endl;
    cout << "Max allowed size: " << Die::MAX_DIE_LENGTH 
         << " x " << Die::MAX_DIE_WIDTH << " mm" << endl;
    
    // Create a die within limits
    float small_compute_sizes[2] = {20, 20}; // Within max 33x28
    Compute small_compute_unit(small_compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);
    string small_up = {MEMORY_UNIT};
    string small_down = "";
    string small_left = "";
    string small_right = "";
    
    Die small_die(die_padding, bandwidth_per_area, memory_bandwidth_ratio,
                  small_compute_unit, Memory_unit, Communication_unit,
                  small_up, small_down, small_left, small_right);
    
    is_valid = small_die.check_reticle_limit();
    cout << "Small die is within reticle limit: " << (is_valid ? "true" : "false") << endl;
    cout << "Small die size: " << small_die.get_size(0)
         << " x " << small_die.get_size(1) << " mm" << endl;
    
    // Test 2: D2D bandwidth constraints
    cout << "\nTesting D2D bandwidth constraints..." << endl;
    
    // Test with a small die (should get max bandwidth)
    float small_compute_sizes[2] = {15, 15};
    Compute small_compute_unit(small_compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);
    string small_up = "";
    string small_down = "";
    string small_left = "";
    string small_right = {COMMUNICATION_UNIT, COMMUNICATION_UNIT}; // High bandwidth demand
    
    Die small_die(die_padding, bandwidth_per_area, memory_bandwidth_ratio,
                  small_compute_unit, Memory_unit, Communication_unit,
                  small_up, small_down, small_left, small_right);
    
    float small_perimeter = 2.0f * (small_die.get_size(0) + small_die.get_size(1));
    cout << "Small die perimeter: " << small_perimeter << " mm" << endl;
    cout << "Small die communication bandwidth: " << small_die.get_communication_bandwidth() << " TB/s" << endl;
    
    // For small dies, bandwidth should be close to reference
    if (small_perimeter <= Die::REFERENCE_PERIMETER) {
        assert(small_die.get_communication_bandwidth() <= Die::REFERENCE_BANDWIDTH);
    }
    
    // Test with a large die (should get min bandwidth)
    float large_perimeter = 2.0f * (large_die.get_size(0) + large_die.get_size(1));
    cout << "\nLarge die perimeter: " << large_perimeter << " mm" << endl;
    cout << "Large die communication bandwidth: " << large_die.get_communication_bandwidth() << " TB/s" << endl;
    
    // For large dies, bandwidth should be limited to minimum
    if (large_perimeter >= Die::MAX_PERIMETER) {
        assert(large_die.get_communication_bandwidth() <= Die::MIN_BANDWIDTH + 1.0f); // Small tolerance
    }
    
    cout << "\nAll constraint tests passed!" << endl;

    return 0;
}
