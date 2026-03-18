# Testing Guide for New Constraint Features

## Overview
This guide explains how to test the new die size and D2D bandwidth constraint features that have been implemented in the error_tolerance project.

## Features Implemented

### 1. Maximum Die Size Constraints (33mm × 28mm)
- **Purpose**: Rejects die configurations that exceed realistic manufacturing limits
- **Implementation**: `Die::check_reticle_limit()` method (returns false if dimensions exceed 33mm × 28mm)
- **Constants**: `MAX_DIE_LENGTH = 33.0f`, `MAX_DIE_WIDTH = 28.0f`

### 2. D2D Bandwidth Constraints Based on Die Perimeter
- **Purpose**: Models physical communication limitations for larger dies
- **Implementation**: `Die::apply_d2d_bandwidth_constraints()` method
- **Behavior**: Linear interpolation from 20 TB/s (small) to 10 TB/s (large)

## How to Test

### Option 1: Using Visual Studio
1. Open the project in Visual Studio
2. Build the solution (F7 or Build → Build Solution)
3. Run the Die_test from the test directory
4. Check console output for constraint validation messages

### Option 2: Using CMake with Visual Studio Developer Command Prompt
1. Open "Developer Command Prompt for VS"
2. Navigate to project directory:
   ```cmd
   cd "d:\VS projects\error_tolerance"
   ```
3. Create build directory and configure:
   ```cmd
   mkdir build
   cd build
   cmake ..
   ```
4. Build the project:
   ```cmd
   cmake --build . --config Debug
   ```
5. Run tests:
   ```cmd
   cd test
   Die_test.exe
   ```

### Option 3: Using VS Code with C++ Extension
1. Ensure you have the C/C++ extension installed
2. Install a C++ compiler (MinGW, Visual Studio Build Tools, etc.)
3. Open the project in VS Code
4. Use the integrated terminal to build:
   ```cmd
   g++ -I. -std=c++23 test/Die_test.cpp src/Die.cpp src/Compute.cpp src/Memory.cpp src/Communication.cpp -o Die_test.exe
   ```

## Expected Test Results

### Die Size Constraint Test
When you run `Die_test.cpp`, you should see output similar to:
```
Testing die size constraints...
Large die is within reticle limit: false
Large die size: ... x ... mm
Max allowed size: 33.0 x 28.0 mm
```

This confirms that dies exceeding the maximum dimensions are correctly identified (check_reticle_limit returns false).

### D2D Bandwidth Constraint Test
You should see output similar to:
```
Testing D2D bandwidth constraints...
Small die perimeter: 60.0 mm
Small die communication bandwidth: 20.0 TB/s

Large die perimeter: 122.0 mm
Large die communication bandwidth: 10.0 TB/s
```

This confirms that:
- Small dies get maximum bandwidth (20 TB/s)
- Large dies get minimum bandwidth (10 TB/s)
- Bandwidth scales linearly with perimeter

## Manual Verification Code

You can also create a simple test program to verify the constraints work:

```cpp
#include <iostream>
#include "include/Die.h"
#include "include/Compute.h"
#include "include/Memory.h"
#include "include/Communication.h"

int main() {
    // Test 1: Oversized die
    float large_sizes[2] = {40.0f, 35.0f};
    Compute large_compute(large_sizes, 1.0f, 1.0f, 0.1f); // 40x35mm
    float mem_sizes[2] = {5.0f, 2.0f};
    Memory memory_unit(mem_sizes, 10.0f, 1.0f, 0.05f);
    float comm_sizes[2] = {3.0f, 1.0f};
    Communication comm_unit(comm_sizes, 15.0f, 0.02f);
    
    string up = "00000"; // Many memory units
    string down = "";
    string left = "";
    string right = "";
    
    Die large_die(0.1f, 1.0f, 0.7f, large_compute, memory_unit, comm_unit, up, down, left, right);
    
    std::cout << "Large die final size: " << large_die.get_size(0) 
              << " x " << large_die.get_size(1) << " mm" << std::endl;
    std::cout << "Within reticle limit: " << (large_die.check_reticle_limit() ? "yes" : "no") << std::endl;
    
    // Test 2: D2D bandwidth
    float perimeter = 2.0f * (large_die.get_size(0) + large_die.get_size(1));
    std::cout << "Die perimeter: " << perimeter << " mm" << std::endl;
    std::cout << "Communication bandwidth: " << large_die.get_communication_bandwidth() 
              << " TB/s" << std::endl;
    
    return 0;
}
```

## Validation Checklist

- [ ] Dies exceeding 33mm × 28mm are rejected (check_reticle_limit returns false)
- [ ] Small dies (≤20mm perimeter) get full 20 TB/s bandwidth
- [ ] Large dies (≥122mm perimeter) get minimum 10 TB/s bandwidth
- [ ] Medium dies get linearly interpolated bandwidth
- [ ] No existing functionality is broken
- [ ] Tests pass without assertions

## Integration Verification

The constraints are automatically applied in the `Die::update()` method, so they work with:
- All existing Die constructors
- Permutation algorithms
- Wafer optimization
- Any code that creates or modifies Die objects

## Troubleshooting

### Build Issues
- **Compiler not found**: Install Visual Studio Build Tools or MinGW
- **Include path issues**: Ensure you're building from the project root directory
- **Link errors**: Make sure all source files are compiled and linked

### Test Failures
- **Assertion failures**: Check that constraint constants are set correctly
- **Unexpected values**: Verify the calculation order in `update()` method
- **No constraint applied**: Ensure `check_reticle_limit()` is used in Permutation and `apply_d2d_bandwidth_constraints()` is called in `update()`

## Performance Impact

The constraint methods add minimal overhead:
- Reticle limit check: Simple comparison operations
- Bandwidth constraint: Linear interpolation (constant time)
- Called once per die update: O(1) complexity

These constraints actually improve optimization performance by reducing the search space to physically feasible solutions.
