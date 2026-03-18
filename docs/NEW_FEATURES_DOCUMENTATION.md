# New Features: Die Size and D2D Bandwidth Constraints

## Overview

Two new constraint features have been added to the error_tolerance project to improve the realism of wafer-scale architecture optimization:

1. **Maximum Die Size Constraints**: Limits die dimensions to 33mm × 28mm
2. **D2D Bandwidth Constraints**: Limits communication bandwidth based on die perimeter

## Feature 1: Reticle Limit Constraints

### Purpose
Enforces realistic physical manufacturing constraints on die dimensions by abandoning combinations that exceed reticle limits.

### Implementation
- **Maximum Length**: 33mm
- **Maximum Width**: 28mm
- **Validation**: `Die::check_reticle_limit()` method returns boolean (true if within limits, false if exceeding)
- **Applied in**: Permutation algorithm skips combinations that exceed the reticle limit (invalid combinations are abandoned, not capped)

### Code Details
```cpp
// Constants in Die.h
static constexpr float MAX_DIE_LENGTH = 33.0f; // mm
static constexpr float MAX_DIE_WIDTH = 28.0f;   // mm

// Implementation in Die.cpp
bool Die::check_reticle_limit(){
    float current_length = get_size(0);
    float current_width = get_size(1);
    
    if (current_length > MAX_DIE_LENGTH || current_width > MAX_DIE_WIDTH) {
        return false; // Die exceeds reticle limit
    }
    
    return true; // Die is within reticle limit
}
```

### Behavior
- If a die calculation results in dimensions exceeding the maximum, that permutation is rejected (skipped) in the Permutation search
- This ensures only physically manufacturable die configurations are considered

## Feature 2: D2D Bandwidth Constraints

### Purpose
Models the physical limitation that larger dies have reduced D2D (die-to-die) communication bandwidth due to longer signal paths and increased interference.

### Implementation
- **Reference Point**: 20mm × 20mm die → 20 TB/s bandwidth
- **Maximum Die**: 33mm × 28mm die → 10 TB/s bandwidth
- **Relationship**: Linear interpolation based on die perimeter
- **Applied in**: `Die::apply_d2d_bandwidth_constraints()` method
- **Called from**: `Die::update()` method

### Code Details
```cpp
// Constants in Die.h
static constexpr float REFERENCE_PERIMETER = 80.0f;  // 20mm * 4mm
static constexpr float REFERENCE_BANDWIDTH = 20.0f; // TB/s for 20mm*20mm die
static constexpr float MAX_PERIMETER = 122.0f;      // 2 * (33mm + 28mm)
static constexpr float MIN_BANDWIDTH = 10.0f;       // TB/s for 33mm*28mm die

// Implementation in Die.cpp
void Die::apply_d2d_bandwidth_constraints(){
    float current_length = get_size(0);
    float current_width = get_size(1);
    float current_perimeter = 2.0f * (current_length + current_width);
    
    float d2d_bandwidth;
    
    if (current_perimeter <= REFERENCE_PERIMETER) {
        d2d_bandwidth = REFERENCE_BANDWIDTH;
    } else if (current_perimeter >= MAX_PERIMETER) {
        d2d_bandwidth = MIN_BANDWIDTH;
    } else {
        // Linear interpolation
        float ratio = (current_perimeter - REFERENCE_PERIMETER) / (MAX_PERIMETER - REFERENCE_PERIMETER);
        d2d_bandwidth = REFERENCE_BANDWIDTH - ratio * (REFERENCE_BANDWIDTH - MIN_BANDWIDTH);
    }
    
    // Limit communication bandwidth to D2D bandwidth constraint
    float current_comm_bandwidth = get_communication_bandwidth();
    if (current_comm_bandwidth > d2d_bandwidth) {
        set_communication_bandwidth(d2d_bandwidth);
    }
}
```

### Behavior
- **Small dies** (perimeter ≤ 80mm): Maximum 20 TB/s bandwidth
- **Large dies** (perimeter ≥ 122mm): Minimum 10 TB/s bandwidth  
- **Medium dies**: Linear interpolation between 10-20 TB/s
- Communication bandwidth is capped at the calculated D2D limit

## Integration with Existing System

### Update Flow
The constraint methods are called at the end of the `Die::update()` method:
```cpp
void Die::update(){
    update_size(); 
    update_TFLOPS(); 
    update_DRAM_capacity(); 
    update_SRAM_capacity();
    update_bandwidth();
    apply_d2d_bandwidth_constraints(); // NEW
    return;
}
```

### Impact on Optimization
- **Permutation algorithm**: Now only considers physically feasible die configurations
- **Wafer optimization**: Results in more realistic performance predictions
- **Error tolerance**: Accounts for manufacturing constraints in solution space

## Testing

### Test Cases Added
1. **Die Size Constraint Tests**: Verify oversized dies are capped correctly
2. **D2D Bandwidth Tests**: Verify bandwidth limits are applied based on die size
3. **Edge Case Tests**: Test behavior at boundary conditions

### Test Files
- `test/Die_test.cpp`: Updated with constraint validation tests
- `test_constraints.cpp`: Standalone demonstration program

## Usage Examples

### Example 1: Oversized Die
```cpp
// Create a die that would be 40mm x 35mm
Compute large_compute(...);
Die large_die(..., large_compute, ...);

// check_reticle_limit() returns false; such dies are skipped in Permutation search
bool valid = large_die.check_reticle_limit(); // false if size exceeds 33mm x 28mm
```

### Example 2: Bandwidth Limitation
```cpp
// Small die (15mm x 15mm) → high bandwidth allowed
// Large die (33mm x 28mm) → bandwidth limited to 10 TB/s
float small_bandwidth = small_die.get_communication_bandwidth(); // ≤ 20 TB/s
float large_bandwidth = large_die.get_communication_bandwidth(); // ≤ 10 TB/s
```

## Benefits

1. **Realistic Constraints**: Ensures generated architectures are physically manufacturable
2. **Accurate Modeling**: Better reflects actual D2D communication limitations
3. **Improved Optimization**: More relevant search space for architectural optimization
4. **Better Error Tolerance**: Accounts for physical constraints in error margin calculations

## Future Extensions

Potential enhancements could include:
- Asymmetric die size constraints
- Non-linear bandwidth models
- Temperature-dependent constraints
- Process-specific limitations
- Yield-based optimization

## Notes

- Constraints are applied automatically during die creation and updates
- Existing code using the Die class requires no modifications
- All constraint parameters are configurable via the constants in Die.h
- The implementation maintains backward compatibility with existing functionality
