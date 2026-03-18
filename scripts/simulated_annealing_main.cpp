#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#include "Simulated_Annealing.h"
#include "Generate.h"
#include "Astra_API.h"

using namespace std;

// Helper: Create system.json configuration file according to astra-sim guide
void create_system_json(const string& output_path, float mem_bandwidth) {
    ofstream file(output_path);
    if (!file.is_open()) {
        cerr << "Error: Failed to create system.json: " << output_path << endl;
        return;
    }
    
    file << "{\n"
         << "  \"scheduling-policy\": \"LIFO\",\n"
         << "  \"intra-dimension-scheduling\": \"FIFO\",\n"
         << "  \"inter-dimension-scheduling\": \"baseline\",\n"
         << "  \"endpoint-delay\": 10,\n"
         << "  \"active-chunks-per-dimension\": 1,\n"
         << "  \"preferred-dataset-splits\": 1,\n"
         << "  \"all-reduce-implementation\": [\"ring\", \"ring\"],\n"
         << "  \"all-gather-implementation\": [\"ring\", \"ring\"],\n"
         << "  \"reduce-scatter-implementation\": [\"ring\", \"ring\"],\n"
         << "  \"all-to-all-implementation\": [\"ring\", \"ring\"],\n"
         << "  \"local_mem_bw\": " << mem_bandwidth << ",\n"
         << "  \"boost_mode\": 0\n"
         << "}\n";
    
    file.close();
}

// Helper: Create network.yml configuration file according to astra-sim guide
void create_network_yml(const string& output_path, int rows, int columns, float bandwidth) {
    ofstream file(output_path);
    if (!file.is_open()) {
        cerr << "Error: Failed to create network.yml: " << output_path << endl;
        return;
    }
    
    file << "topology: [Ring, Ring]\n"
         << "npus_count: [" << columns << ", " << rows << "]\n"
         << "bandwidth: [" << bandwidth << ", " << bandwidth << "]\n"
         << "latency: [10.0, 10.0]\n";
    
    file.close();
}

// Helper: Create remote_memory.json configuration file
void create_remote_memory_json(const string& output_path) {
    ofstream file(output_path);
    if (!file.is_open()) {
        cerr << "Error: Failed to create remote_memory.json: " << output_path << endl;
        return;
    }
    
    file << "{\n"
         << "  \"memory-type\": \"NO_MEMORY_EXPANSION\",\n"
         << "  \"remote-mem-latency\": 100,\n"
         << "  \"remote-mem-bw\": 10\n"
         << "}\n";
    
    file.close();
}

// Helper: Create logical_network.json configuration file
void create_logical_network_json(const string& output_path, int rows, int columns) {
    ofstream file(output_path);
    if (!file.is_open()) {
        cerr << "Error: Failed to create logical_network.json: " << output_path << endl;
        return;
    }
    
    // Format matches gen_logical_network.py: logical-dims with string values
    file << "{\n"
         << "  \"logical-dims\": [\"" << columns << "\", \"" << rows << "\"]\n"
         << "}\n";
    
    file.close();
}

// Wrapper function to call ns-3 simulator and parse output
float ns3_simulator_wrapper(const Wafer& wafer, const string& ns3_command_template, 
                            const string& temp_dir, int& call_count) {
    
    // Generate unique ID for this evaluation
    stringstream ss;
    ss << "sa_eval_" << setw(6) << setfill('0') << call_count;
    string eval_id = ss.str();
    call_count++;
    
    // Generate workload and network files for this wafer
    Die die = wafer.get_die();
    int columns = wafer.get_columns();
    int rows = wafer.get_rows();
    
    if (columns <= 0 || rows <= 0) {
        cerr << "Error: Invalid wafer configuration (columns or rows <= 0)" << endl;
        return 1e12;
    }
    
    // Create file paths
    string workload_path = temp_dir + "/" + eval_id + "_workload.txt";
    string network_config_path = temp_dir + "/" + eval_id + "_config.txt";
    string system_path = temp_dir + "/" + eval_id + "_system.json";
    string network_yml_path = temp_dir + "/" + eval_id + "_network.yml";
    string remote_memory_path = temp_dir + "/" + eval_id + "_remote_memory.json";
    string logical_network_path = temp_dir + "/" + eval_id + "_logical_network.json";
    string output_path = temp_dir + "/" + eval_id + "_ns3_output.txt";
    
    // Write workload file (Astra-sim format)
    ofstream workload_file(workload_path);
    if (!workload_file.is_open()) {
        cerr << "Error: Failed to create workload file: " << workload_path << endl;
        return 1e12;
    }
    
    // Example workload generation (you should pass real parameters)
    int forward_cycle = 1000;
    int forward_comm_size = 1000;
    int weight_comm_size = 500;
    
    workload_file << "DATA" << endl;
    workload_file << columns << endl;
    for (int i = 0; i < columns; i++) {
        workload_file << "layer_" << i << " -1 " 
                     << forward_cycle << " ALLREDUCE " << forward_comm_size << " "
                     << forward_cycle << " NONE 0 "
                     << forward_cycle << " ALLREDUCE " << weight_comm_size << " 10" << endl;
    }
    workload_file.close();
    
    // Write physical network file (ns-3 format)
    ofstream network_file(network_config_path);
    if (!network_file.is_open()) {
        cerr << "Error: Failed to create network config file: " << network_config_path << endl;
        return 1e12;
    }
    
    int num_nodes = rows * columns;
    int num_links = 2 * rows * columns;
    // physical_network.txt: bandwidth in Gbps = comm_bw / 4 * 8 (matching Astra_API.cpp)
    float die_bandwidth_gbps = die.get_communication_bandwidth() / 4 * 8; // Gbps for physical network
    // network.yml: bandwidth in GB/s per direction = comm_bw / 4 (matching gen_network.py)
    float die_bandwidth_gbs = die.get_communication_bandwidth() / 4; // GB/s for network.yml
    
    network_file << num_nodes << " 0 " << num_links << endl << endl;
    
    // Generate 2D torus topology (ring in both dimensions)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int start = i * columns + j;
            int end1 = ((i + 1) % rows) * columns + j;
            int end2 = i * columns + ((j + 1) % columns);
            
            network_file << start << " " << end1 << " " << die_bandwidth_gbps << "Gbps 0.001ms 0" << endl;
            network_file << start << " " << end2 << " " << die_bandwidth_gbps << "Gbps 0.001ms 0" << endl;
        }
    }
    network_file.close();
    
    // Create configuration files according to astra-sim guide
    float mem_bandwidth = die.get_memory_bandwidth();
    create_system_json(system_path, mem_bandwidth);
    create_network_yml(network_yml_path, rows, columns, die_bandwidth_gbs);
    create_remote_memory_json(remote_memory_path);
    create_logical_network_json(logical_network_path, rows, columns);
    
    // Construct and execute ns-3 command
    string command = ns3_command_template;
    
    // Replace placeholders in command template
    size_t pos;
    while ((pos = command.find("{WORKLOAD}")) != string::npos) {
        command.replace(pos, 10, workload_path);
    }
    while ((pos = command.find("{NETWORK}")) != string::npos) {
        command.replace(pos, 9, network_config_path);
    }
    while ((pos = command.find("{SYSTEM}")) != string::npos) {
        command.replace(pos, 8, system_path);
    }
    while ((pos = command.find("{NETWORK_YML}")) != string::npos) {
        command.replace(pos, 13, network_yml_path);
    }
    while ((pos = command.find("{REMOTE_MEMORY}")) != string::npos) {
        command.replace(pos, 15, remote_memory_path);
    }
    while ((pos = command.find("{LOGICAL_NETWORK}")) != string::npos) {
        command.replace(pos, 17, logical_network_path);
    }
    while ((pos = command.find("{OUTPUT}")) != string::npos) {
        command.replace(pos, 8, output_path);
    }
    
    cout << "  [Eval " << call_count - 1 << "] Executing ns-3 simulation..." << endl;
    
    // Execute ns-3 simulation
    int ret = system(command.c_str());
    if (ret != 0) {
        cerr << "  [Eval " << call_count - 1 << "] Warning: ns-3 simulation failed with return code: " << ret << endl;
        return 1e12;
    }
    
    // Parse output to extract max cycle count
    ifstream output_file(output_path);
    if (!output_file.is_open()) {
        cerr << "  [Eval " << call_count - 1 << "] Warning: Failed to open ns-3 output file" << endl;
        return 1e12;
    }
    
    float max_cycles = 0;
    string line;
    while (getline(output_file, line)) {
        // Look for pattern: "sys[X] finished, Y cycles"
        size_t pos = line.find("finished,");
        if (pos != string::npos) {
            size_t cycle_pos = line.find("cycles", pos);
            if (cycle_pos != string::npos) {
                string cycles_str = line.substr(pos + 9, cycle_pos - pos - 10);
                try {
                    float cycles = stof(cycles_str);
                    max_cycles = max(max_cycles, cycles);
                } catch (const exception& e) {
                    cerr << "  [Eval " << call_count - 1 << "] Warning: Failed to parse cycles" << endl;
                }
            }
        }
    }
    output_file.close();
    
    if (max_cycles == 0) {
        cerr << "  [Eval " << call_count - 1 << "] Warning: No valid cycle count found in ns-3 output" << endl;
        return 1e12;
    }
    
    cout << "  [Eval " << call_count - 1 << "] Completed: " << max_cycles << " cycles" << endl;
    
    return max_cycles;
}
