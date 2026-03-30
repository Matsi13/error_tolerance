import os
import sys
import math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyArrowPatch
from dataclasses import dataclass, field
from typing import List

# ---- constants (from Macro.h) ----
MEMORY_UNIT = "0"
COMMUNICATION_UNIT = "1"

COLORS = {
    "compute":       "#4A90D9",
    "memory":        "#E8A838",
    "communication": "#5CB85C",
    "padding":       "#F0F0F0",
    "die_bg":        "#FAFAFA",
    "wafer_bg":      "#1A1A2E",
    "die_tile":      "#2A4A7F",
    "die_border":    "#AACCFF",
}

# ---- data classes ----
@dataclass
class ComputeUnit:
    w: float; h: float; TFLOPS: float; SRAM: float; padding: float

@dataclass
class MemoryUnit:
    w: float; h: float; capacity: float; bandwidth: float; padding: float

@dataclass
class CommunicationUnit:
    w: float; h: float; bandwidth: float; padding: float

@dataclass
class DieEntry:
    idx: int
    TFLOPS: float; SRAM: float; DRAM: float; mem_bw: float; comm_bw: float
    rows: int; columns: int
    wafer_w: float; wafer_h: float
    compute: ComputeUnit
    memory: MemoryUnit
    comm: CommunicationUnit
    up: str; down: str; left: str; right: str
    padding: float; bw_per_area: float; mem_bw_ratio: float
    die_w: float; die_h: float

# ---- parse wafer file ----
def parse_wafer_file(path: str) -> List[DieEntry]:
    entries = []
    with open(path, "r") as f:
        for idx, line in enumerate(f):
            p = line.strip().split()
            if len(p) < 32:
                continue
            # columns 0-4: TFLOPS SRAM DRAM mem_bw comm_bw
            # columns 5-6: rows columns
            # columns 7-8: wafer_w wafer_h
            # columns 9-13: compute w h TFLOPS SRAM padding
            # columns 14-18: memory w h capacity bandwidth padding
            # columns 19-22: comm w h bandwidth padding
            # columns 23-26: up down left right
            # columns 27-29: die_padding bw_per_area mem_bw_ratio
            # columns 30-31: die_w die_h
            c = ComputeUnit(float(p[9]),float(p[10]),float(p[11]),float(p[12]),float(p[13]))
            m = MemoryUnit(float(p[14]),float(p[15]),float(p[16]),float(p[17]),float(p[18]))
            k = CommunicationUnit(float(p[19]),float(p[20]),float(p[21]),float(p[22]))
            e = DieEntry(
                idx=idx,
                TFLOPS=float(p[0]),SRAM=float(p[1]),DRAM=float(p[2]),
                mem_bw=float(p[3]),comm_bw=float(p[4]),
                rows=int(p[5]),columns=int(p[6]),
                wafer_w=float(p[7]),wafer_h=float(p[8]),
                compute=c, memory=m, comm=k,
                up=p[23],down=p[24],left=p[25],right=p[26],
                padding=float(p[27]),bw_per_area=float(p[28]),mem_bw_ratio=float(p[29]),
                die_w=float(p[30]),die_h=float(p[31])
            )
            entries.append(e)
    return entries

# ---- draw a single die and return figure ----
def draw_die(e: DieEntry) -> plt.Figure:
    SCALE = 18  # pixels per mm
    comp_pad = max(e.memory.padding, e.comm.padding)

    cw, ch = e.compute.w, e.compute.h
    mw, mh = e.memory.w, e.memory.h
    kw, kh = e.comm.w, e.comm.h

    # compute overall die size (mirrors update_size in Die.cpp)
    def side_dims(perm):
        """total length and max width of a side string"""
        length, width = 0.0, 0.0
        for c in perm:
            if c == "0":  length += mw; width = max(width, mh)
            elif c == "1": length += kw; width = max(width, kh)
        if len(perm) > 1: length += (len(perm)-1)*comp_pad
        return length, width

    _, up_h   = side_dims(e.up)   if e.up   != "." else (0,0)
    _, dn_h   = side_dims(e.down) if e.down != "." else (0,0)
    lf_l, _  = side_dims(e.left) if e.left != "." else (0,0)
    rt_l, _  = side_dims(e.right)if e.right!= "." else (0,0)

    total_w = cw + lf_l + rt_l + 2*e.padding
    total_h = ch + up_h + dn_h + 2*e.padding

    fw = max(total_w * SCALE / 72, 5)
    fh = max(total_h * SCALE / 72 + 2.8, 4.5)
    fig, ax = plt.subplots(figsize=(fw, fh))
    fig.patch.set_facecolor("#1A1A2E")
    ax.set_facecolor(COLORS["die_bg"])
    ax.set_xlim(-0.5, total_w+0.5)
    ax.set_ylim(-0.5, total_h+0.5)
    ax.set_aspect("equal")
    ax.axis("off")

    def rect(ax, x, y, w, h, color, label=None, alpha=0.92):
        r = patches.FancyBboxPatch((x,y),w,h,
            boxstyle="round,pad=0.02",
            linewidth=0.8, edgecolor="white", facecolor=color, alpha=alpha)
        ax.add_patch(r)
        if label:
            ax.text(x+w/2, y+h/2, label, ha="center", va="center",
                    fontsize=max(5, min(8, w*SCALE/8)), color="white",
                    fontweight="bold", wrap=True)

    # die border
    rect(ax, 0, 0, total_w, total_h, COLORS["die_bg"], alpha=1.0)
    border = patches.FancyBboxPatch((0,0),total_w,total_h,
        boxstyle="round,pad=0.05",
        linewidth=2, edgecolor=COLORS["die_border"], facecolor="none")
    ax.add_patch(border)

    # compute core (centered)
    cx = e.padding + lf_l
    cy = e.padding + dn_h
    rect(ax, cx, cy, cw, ch, COLORS["compute"],
         f"Compute\n{e.compute.TFLOPS:.0f} TFLOPS\n{e.compute.SRAM:.0f} MB SRAM")

    def draw_side(perm, side):
        if not perm or perm == ".":
            return
        units = []
        total_len = 0.0
        for c in perm:
            if c == "0":   units.append(("M", mw, mh)); total_len += mw
            elif c == "1": units.append(("C", kw, kh)); total_len += kw
        total_len += (len(units)-1)*comp_pad

        if side in ("up","down"):
            start_x = cx + cw/2 - total_len/2
            x = start_x
            for t, uw, uh in units:
                if side == "up": uy = cy+ch
                else:            uy = cy-uh
                color = COLORS["memory"] if t=="M" else COLORS["communication"]
                lbl = (f"Mem\n{e.memory.capacity:.0f}GB\n{e.memory.bandwidth:.0f}GB/s"
                       if t=="M" else
                       f"Comm\n{e.comm.bandwidth:.1f}TB/s")
                rect(ax, x, uy, uw, uh, color, lbl)
                x += uw + comp_pad
        else:  # left / right
            start_y = cy + ch/2 - total_len/2
            y = start_y
            for t, uw, uh in units:
                if side == "left": ux = cx-uw
                else:              ux = cx+cw
                color = COLORS["memory"] if t=="M" else COLORS["communication"]
                lbl = (f"Mem\n{e.memory.capacity:.0f}GB\n{e.memory.bandwidth:.0f}GB/s"
                       if t=="M" else
                       f"Comm\n{e.comm.bandwidth:.1f}TB/s")
                rect(ax, ux, y, uw, uh, color, lbl)
                y += uw + comp_pad

    draw_side(e.up if e.up != "." else "", "up")
    draw_side(e.down if e.down != "." else "", "down")
    draw_side(e.left if e.left != "." else "", "left")
    draw_side(e.right if e.right != "." else "", "right")

    num_dies = e.rows * e.columns

    title = (f"Wafer {e.idx:04d}  |  {e.rows} x {e.columns} = {num_dies} dies  |  "
             f"Die {total_w:.1f} x {total_h:.1f} mm")
    stats = (f"TFLOPS: {e.TFLOPS:.1f}   SRAM: {e.SRAM:.1f} MB   DRAM: {e.DRAM:.1f} GB   "
             f"MemBW: {e.mem_bw:.1f} GB/s   CommBW: {e.comm_bw:.1f} TB/s")
    fig.text(0.5, 0.97, title, ha="center", va="top",
             color="white", fontsize=10, fontweight="bold")
    fig.text(0.5, 0.92, stats, ha="center", va="top",
             color="#AACCFF", fontsize=8)

    # legend
    legend_items = [
        patches.Patch(color=COLORS["compute"],       label="Compute Core"),
        patches.Patch(color=COLORS["memory"],        label="Memory (HBM)"),
        patches.Patch(color=COLORS["communication"], label="Communication"),
    ]
    ax.legend(handles=legend_items, loc="lower center",
              bbox_to_anchor=(0.5, -0.18), ncol=3,
              framealpha=0.3, facecolor="#1A1A2E", edgecolor="#AACCFF", fontsize=7)

    plt.tight_layout(rect=[0,0.05,1,0.90])
    return fig

# ---- main ----
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 visualize_die.py <wafer_file> [output_dir]")
        print("Example: python3 visualize_die.py ./output/0070/0070_Wafer.txt ./output/0070/die_viz")
        sys.exit(1)

    wafer_file  = sys.argv[1]
    output_dir  = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(wafer_file), "die_viz")
    os.makedirs(output_dir, exist_ok=True)

    config = os.path.basename(wafer_file).split("_")[0]
    entries = parse_wafer_file(wafer_file)

    if not entries:
        print("Error: No valid wafer entries found.")
        sys.exit(1)

    print(f"Found {len(entries)} wafers in {wafer_file}")
    print(f"Saving die visualizations to {output_dir}/")
    print()

    for e in entries:
        num_dies = e.rows * e.columns
        print(f"  Wafer {e.idx:04d}: {e.rows} x {e.columns} = {num_dies} dies  "
              f"| Die size {e.die_w:.1f} x {e.die_h:.1f} mm  "
              f"| {e.TFLOPS:.1f} TFLOPS")
        fig = draw_die(e)
        fname = os.path.join(output_dir, f"{config}_{e.idx:04d}_die.png")
        fig.savefig(fname, dpi=150, bbox_inches="tight",
                    facecolor=fig.get_facecolor())
        plt.close(fig)

    print()
    print(f"Done. {len(entries)} images saved to {output_dir}/")

