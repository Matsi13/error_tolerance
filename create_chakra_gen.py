content = '''#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

struct WaferConfig {
    int rows;
    int columns;
    int num_npus;
};

WaferConfig parse_wafer_line(const string& line) {
    WaferConfig config;
    istringstream iss(line);
    
    float dummy;
    for (int i = 0; i < 5; i++) {
        iss >> dummy;
    }
    
    iss >> config.rows >> config.columns;
    config.num_npus = config.rows * config.columns;
    
    return config;
}

bool run_chakra_converter(const string& input_workload, 
                          const string& output_dir,
                          int num_npus,
                          int wafer_idx) {
    if (!fs::exists(output_dir)) {
        fs::create_directories(output_dir);
    }
    
    stringstream cmd;
    cmd << "chakra_converter Text "
        << "--input " << input_workload << " "
        << "--output " << output_dir << "/wafer_" << setw(4) << setfill('0') << wafer_idx << " "
        << "--num-npus " << num_npus << " "
        << "--num-passes 1";
    
    cout << "  [Wafer " << wafer_idx << "] Running: " << cmd.str() << endl;
    
    int ret = system(cmd.str().c_str());
    if (ret != 0) {
        cerr << "  [Wafer " << wafer_idx << "] Error: chakra_converter failed with return code " << ret << endl;
        return false;
    }
    
    cout << "  [Wafer " << wafer_idx << "] Success: Generated workload for " << num_npus << " NPUs" << endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <wafer_config_file> <text_workload_file> <output_directory>" << endl;
        cerr << "Example: " << argv[0] << " ./output/0070/0070_Wafer.txt ./workloads/Llama_3B.txt ./output/0070/chakra_workloads" << endl;
        return 1;
    }
    
    string wafer_config_file = argv[1];
    string text_workload_file = argv[2];
    string output_directory = argv[3];
    
    if (!fs::exists(wafer_config_file)) {
        cerr << "Error: Wafer configuration file not found: " << wafer_config_file << endl;
        return 1;
    }
    
    if (!fs::exists(text_workload_file)) {
        cerr << "Error: Text workload file not found: " << text_workload_file << endl;
        return 1;
    }
    
    cout << "=== Chakra Workload Generator ===" << endl;
    cout << "Wafer config: " << wafer_config_file << endl;
    cout << "Text workload: " << text_workload_file << endl;
    cout << "Output directory: " << output_directory << endl << endl;
    
    ifstream wafer_file(wafer_config_file);
    if (!wafer_file.is_open()) {
        cerr << "Error: Failed to open wafer configuration file" << endl;
        return 1;
    }
    
    vector<WaferConfig> wafers;
    string line;
    int wafer_count = 0;
    
    cout << "Reading wafer configurations..." << endl;
    while (getline(wafer_file, line)) {
        if (line.empty()) continue;
        
        try {
            WaferConfig config = parse_wafer_line(line);
            wafers.push_back(config);
            cout << "  Wafer " << wafer_count << ": " << config.rows << "x" << config.columns 
                 << " = " << config.num_npus << " NPUs" << endl;
            wafer_count++;
        } catch (const exception& e) {
            cerr << "Warning: Failed to parse wafer line: " << e.what() << endl;
        }
    }
    wafer_file.close();
    
    if (wafers.empty()) {
        cerr << "Error: No valid wafer configurations found" << endl;
        return 1;
    }
    
    cout << "\\nTotal wafers: " << wafers.size() << endl << endl;
    
    cout << "Generating Chakra workloads..." << endl;
    int success_count = 0;
    int fail_count = 0;
    
    for (int i = 0; i < wafers.size(); i++) {
        string wafer_output_dir = output_directory;
        
        if (run_chakra_converter(text_workload_file, wafer_output_dir, wafers[i].num_npus, i)) {
            success_count++;
        } else {
            fail_count++;
        }
    }
    
    cout << "\\n=== Generation Complete ===" << endl;
    cout << "Successful: " << success_count << endl;
    cout << "Failed: " << fail_count << endl;
    cout << "Output directory: " << output_directory << endl;
    
    if (fail_count > 0) {
        return 1;
    }
    
    return 0;
}
'''

with open(r'd:\VS projects\error_tolerance\chakra_workload_generator.cpp', 'w') as f:
    f.write(content)

print('File created successfully at d:\\VS projects\\error_tolerance\\chakra_workload_generator.cpp')
