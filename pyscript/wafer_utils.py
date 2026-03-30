"""Shared utilities for wafer configuration file parsing.

All generator scripts (gen_config_txt.py, gen_network.py,
gen_system.py, gen_logical_network.py) import from this module.
"""

import os
from dataclasses import dataclass
from typing import List


@dataclass
class WaferEntry:
    idx: int
    rows: int
    columns: int
    mem_bw: float
    comm_bw: float


def extract_config(wafer_file: str) -> str:
    base = os.path.basename(wafer_file)
    stem, _ = os.path.splitext(base)
    return stem.split("_")[0] if "_" in stem else stem


def load_wafer_file(wafer_file: str) -> List[WaferEntry]:
    entries = []
    with open(wafer_file, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 32:
                print(f"Warning: skipping short line ({len(parts)} cols)")
                continue
            entries.append(WaferEntry(
                idx=len(entries),
                rows=int(parts[5]),
                columns=int(parts[6]),
                mem_bw=float(parts[30]),
                comm_bw=float(parts[31]),
            ))
    return entries


def ensure_dir(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def out_filename(config: str, idx: int, suffix: str) -> str:
    return f"{config}_{idx:06d}_{suffix}"
