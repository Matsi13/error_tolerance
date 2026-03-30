# MATLAB Scripts — Usage Guide

All scripts are located in the `matlab/` folder. They are standalone MATLAB scripts (`.m` files) used for visualising and analysing results produced by the C++ simulation framework.

---

## Prerequisites

- MATLAB R2019b or later (all plots use standard built-in functions; no toolboxes required).
- Run scripts from inside the `matlab/` directory, or add it to the MATLAB path, so that data files can be found by relative path.

---

## Data File Conventions

Most scripts load plain-text data files exported by the C++ tools. The files follow a naming pattern:

| Pattern | Contents |
|---|---|
| `<prefix>_wafer.txt` | One row per wafer solution. Columns: `TFLOPS`, `SRAM_capacity`, `on_die_mem_capacity`, `memory_bandwidth`, `communication_bandwidth`, `rows`, `columns`, `wafer_size_0`, `wafer_size_1`. |
| `<prefix>_error_curve.txt` | Two columns: error magnitude and number of possible-optimal wafers. |
| `<prefix>_stair_curve.txt` | Two columns: error magnitude and number of possible-optimal wafers (staircase version). |

The prefix (e.g. `0034`, `1108`) identifies a specific configuration run. Change the `file_prefix` variable at the top of the relevant script to switch datasets.

---

## Script Reference

### `performance.m` — Wafer Solution Performance Bar Chart

**Purpose:** Loads a wafer solution file and plots the normalised TFLOPS distribution across all solutions as a bar chart, deduplicating identical values.

**How to use:**
1. Edit the hard-coded filename on line 2:
   ```matlab
   load 0010_wafer.txt
   ```
   Replace `0010` with your configuration prefix.
2. Run the script. A bar chart appears showing `TFLOPS / max(TFLOPS)` for each unique solution.

**Output:** One figure — bar chart of normalised TFLOPS per unique solution.

---

### `visualize.m` — 3-D Scatter Plot of Wafer Solution Space

**Purpose:** Visualises the multi-dimensional solution space of wafer designs. Plots TFLOPS (x), on-die memory capacity (y), and memory bandwidth (z) in a 3-D scatter, with communication bandwidth encoded as colour.

**How to use:**
1. Set the prefix at the top of the script:
   ```matlab
   file_prefix = '1108';  % Change to your prefix
   ```
2. Run the script.

**Output:** One figure — 3-D scatter plot. Axis labels and tick labels are intentionally hidden for paper-ready output; tick marks remain for scale reference. Axis ranges are fixed:
- X (TFLOPS): 0 – 1500
- Y (on-die mem capacity): 0 – 3000
- Z (memory bandwidth): 0 – 9000
- Colorbar (communication bandwidth): 0 – 8000

**Note:** `DRAM_capacity` in the variable names inside this script refers to on-die memory capacity (`on_die_mem_capacity` in the C++ codebase).

---

### `error_fit.m` — Error-Tolerance Curve Fit and Plot

**Purpose:** Loads an error curve file and plots how the fraction of possible-optimal wafers changes as the error magnitude varies. Data is rescaled to a fixed axis range for paper figures.

**How to use:**
1. Set the prefix:
   ```matlab
   file_prefix = '0034';  % Change to your prefix
   ```
2. Run the script.

**Output:** One figure — line plot of (scaled) error magnitude vs. (scaled) fraction of possible-optimal solutions. Axis tick labels are hidden; axis limits are fixed at x ∈ [0.84, 1.00], y ∈ [0.8, 1.0].

---

### `stair.m` — Staircase Error Curve

**Purpose:** Plots the raw staircase relationship between TFLOPs error and the number of possible-optimal wafers, with scatter markers overlaid.

**How to use:**
1. Ensure `0034_stair_curve.txt` is present in the working directory (or update the `load` line with your filename).
2. Run the script.

**Output:** One figure — line plot with overlaid scatter markers showing the staircase curve for the first 50 error-magnitude steps.

---

### `SOPC.m` — Performance Oracle / Prediction Interval Visualisation

**Purpose:** Demonstrates the prediction-interval concept. Generates 20 synthetic oracle performance values and computes upper/lower bounds at two error levels (`err_mid = 0.08`, `err_low = 0.15`). Produces stacked-bar charts showing actual performance vs. predicted ranges, and a scatter plot of oracle values.

**How to use:**
- No input files required. Run directly.
- The random seed is fixed (`rng(13)`) for reproducibility.

**Output:** Three figures:
- **Figure 4** — Stacked bar (mid error): predicted interval bars with actual values overlaid as scatter.
- **Figure 5** — Stacked bar (low error): same layout for the wider error level.
- **Figure 6** — Scatter of oracle performance values alone.

---

### `speed_up.m` — Framework Speed-Up Comparison (Bar Chart)

**Purpose:** Compares design-space exploration speed and accuracy between the WaFoCuS framework, a coarse-grained baseline, and a full-search baseline. Produces multiple bar charts.

**How to use:**
1. This script depends on a variable `speedup` already loaded in the MATLAB workspace. `speedup` is an N×2 matrix where column 1 is the number of all designs and column 2 is the number of coarse-grained designs per configuration.
2. Load your data first, then run the script:
   ```matlab
   load your_speedup_data.mat   % provides 'speedup'
   run('speed_up.m')
   ```

**Output:** Several figures:
- **Bar chart 1** — Relative search-space size: baseline vs. framework vs. coarse.
- **Bar chart 2** — Normalised performance accuracy.
- **Bar chart 3** — Normalised total time (search + RTL re-spin).
- **Bar chart 4** — Per-workload performance comparison (Long-LLM, Medium-LLM, Short-LLM, CNN).

---

### `economic.m` — Time-Cost Curve (Framework vs. Coarse-Grained)

**Purpose:** Plots cumulative engineering cost over time for WaFoCuS vs. a coarse-grained approach across four workload types (Long-LLM, Medium-LLM, Short-LLM, CNN). Mean curves are highlighted.

**How to use:**
- No input files required. All data is hard-coded in the script.
- Run directly.

**Output:** Two figures:
- **Figure 1** — Multi-curve time-cost plot. Curves 1–8 (one WaFoCuS + one coarse pair per workload) are shown in lighter colours; curves 9–10 are bold mean curves in blue (`#0072BD`) and orange (`#D95319`). Dashed segments indicate design-space search phases; solid segments indicate RTL implementation.
- **Figure 2** — Bar chart of total cost at completion for each workload and method.

Console output lists the bar values and hex colour codes for all 10 curves.

---

### `explosion.m` — Solution-Space Size vs. Device Count

**Purpose:** Illustrates how the number of valid solutions grows with the number of devices (quadratic fit), with random noise added to simulate real measurement scatter.

**How to use:**
- No input files required.
- The random seed is fixed (`rng(13)`) for reproducibility.
- Run directly.

**Output:** One figure — scatter plot of device count (128–8192) vs. solution count.

---

## Quick Start Summary

```matlab
% 1. Navigate to the matlab/ directory
cd('path/to/error_tolerance/matlab')

% 2. Run any self-contained script directly
performance    % requires 0010_wafer.txt in working directory
visualise      % set file_prefix first
error_fit      % set file_prefix first
stair          % requires 0034_stair_curve.txt
SOPC           % no data file needed
explosion      % no data file needed
economic       % no data file needed

% 3. For speed_up.m, load the speedup matrix first
load your_speedup_data.mat
speed_up
```

---

## Notes

- Scripts that load `.txt` files use `eval(['load ' file_prefix '_wafer.txt'])`. MATLAB's `load` on a plain-text matrix file creates a variable named after the filename (with dots replaced by underscores), e.g. `X0034_wafer`.
- Axis tick labels are intentionally removed in most scripts to produce clean paper-ready figures. The underlying data and scale are preserved via tick positions.
- The variable `DRAM_capacity` appearing inside some older scripts corresponds to `on_die_mem_capacity` in the current C++ codebase (on-die eDRAM/SRAM chiplet capacity).
