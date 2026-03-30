#!/usr/bin/env python3

import argparse
import glob
import os
import re
from typing import Optional, Tuple, List


def parse_metric_from_text(text: str) -> Tuple[Optional[float], str]:
    """Return (metric_value, metric_type).

    Priority:
    1) max cycle count from: sys[<id>] finished, <N> cycles
    2) runtime-like numeric fields (runtime/exec time/total runtime)

    Lower metric is better.
    """
    cycle_matches = re.findall(r"sys\[(\d+)\]\s+finished,\s*(\d+)\s+cycles", text)
    if cycle_matches:
        max_cycles = max(int(m[1]) for m in cycle_matches)
        return float(max_cycles), "cycles"

    runtime_pattern = re.compile(
        r"(?i)(?:total\s*runtime|runtime|exec(?:ution)?\s*time)\s*[:=]\s*"
        r"([0-9]*\.?[0-9]+(?:[eE][+-]?\d+)?)\s*(ns|us|µs|ms|s|sec|seconds)?"
    )
    runtime_matches = runtime_pattern.findall(text)
    if runtime_matches:
        unit_scale = {
            "ns": 1e-9,
            "us": 1e-6,
            "µs": 1e-6,
            "ms": 1e-3,
            "s": 1.0,
            "sec": 1.0,
            "seconds": 1.0,
            "": 1.0,
        }
        vals = []
        for val_str, unit in runtime_matches:
            v = float(val_str)
            vals.append(v * unit_scale.get(unit.lower(), 1.0))
        return max(vals), "seconds"

    return None, "unknown"


def parse_idx_from_filename(filename: str, config: str) -> Optional[int]:
    base = os.path.basename(filename)
    m = re.match(rf"{re.escape(config)}_(\d+)_analytical_output\.txt$", base)
    if not m:
        return None
    return int(m.group(1))


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Find best wafer and wafers within (1+error_bound)*T from analytical outputs."
    )
    parser.add_argument("output_dir", help="Directory containing *_analytical_output.txt files")
    parser.add_argument("config", help="Config prefix, e.g. 0070")
    parser.add_argument("--error-bound", type=float, default=0.05,
                        help="Allowed slowdown over best T (default: 0.05 for 1.05*T)")
    parser.add_argument("--idx-output", default="possible_optimal.txt",
                        help="Output file to save selected wafer indices")
    parser.add_argument("--summary-output", default="",
                        help="Optional summary output file")
    args = parser.parse_args()

    pattern = os.path.join(args.output_dir, f"{args.config}_*_analytical_output.txt")
    files = sorted(glob.glob(pattern))
    if not files:
        print(f"Error: no files found by pattern: {pattern}")
        return 1

    metrics: List[Tuple[int, float, str, str]] = []
    for fp in files:
        idx = parse_idx_from_filename(fp, args.config)
        if idx is None:
            continue

        with open(fp, "r", encoding="utf-8", errors="ignore") as f:
            text = f.read()

        metric, metric_type = parse_metric_from_text(text)
        if metric is None:
            print(f"Warning: no runtime/cycle metric found in {os.path.basename(fp)}")
            continue

        metrics.append((idx, metric, metric_type, fp))

    if not metrics:
        print("Error: no valid metrics parsed from output files.")
        return 2

    best_idx, best_metric, metric_type, _ = min(metrics, key=lambda x: x[1])
    threshold = best_metric * (1.0 + args.error_bound)

    selected = sorted(idx for idx, metric, _, _ in metrics if metric <= threshold)

    with open(args.idx_output, "w", encoding="utf-8") as f:
        for idx in selected:
            f.write(f"{idx}\n")

    print("=== Analytical Selection Result ===")
    print(f"Metric type : {metric_type}")
    print(f"Best wafer  : {best_idx}")
    print(f"Best T      : {best_metric}")
    print(f"Threshold   : {threshold}  (1 + {args.error_bound}) * T")
    print(f"Selected    : {len(selected)} wafers")
    print(f"Saved idx   : {args.idx_output}")

    if args.summary_output:
        with open(args.summary_output, "w", encoding="utf-8") as f:
            f.write("best_wafer_idx=" + str(best_idx) + "\n")
            f.write("metric_type=" + metric_type + "\n")
            f.write("best_T=" + str(best_metric) + "\n")
            f.write("threshold=" + str(threshold) + "\n")
            f.write("selected_count=" + str(len(selected)) + "\n")
            f.write("selected_indices=" + ",".join(map(str, selected)) + "\n")
        print(f"Saved summary: {args.summary_output}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())