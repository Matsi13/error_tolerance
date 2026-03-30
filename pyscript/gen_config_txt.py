import os
from wafer_utils import extract_config, load_wafer_file, ensure_dir, out_filename


CONFIG_TEMPLATE = [
    "ENABLE_QCN 1",
    "USE_DYNAMIC_PFC_THRESHOLD 1",
    "",
    "PACKET_PAYLOAD_SIZE 1000",
    "",
    "TOPOLOGY_FILE __PLACEHOLDER__",
    "FLOW_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/flow.txt",
    "TRACE_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/trace.txt",
    "TRACE_OUTPUT_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/mix.tr",
    "FCT_OUTPUT_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/fct.txt",
    "PFC_OUTPUT_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/pfc.txt",
    "QLEN_MON_FILE /home/matsi/workspace/astra-sim/extern/network_backend/ns-3/scratch/output/qlen.txt",
    "QLEN_MON_START 0",
    "QLEN_MON_END 20000",
    "",
    "",
    "SIMULATOR_STOP_TIME 40000000000000.00",
    "",
    "CC_MODE 12",
    "ALPHA_RESUME_INTERVAL 1",
    "RATE_DECREASE_INTERVAL 4",
    "CLAMP_TARGET_RATE 0",
    "RP_TIMER 900",
    "EWMA_GAIN 0.00390625",
    "FAST_RECOVERY_TIMES 1",
    "RATE_AI 50Mb/s",
    "RATE_HAI 100Mb/s",
    "MIN_RATE 100Mb/s",
    "DCTCP_RATE_AI 1000Mb/s",
    "",
    "ERROR_RATE_PER_LINK 0.0000",
    "L2_CHUNK_SIZE 4000",
    "L2_ACK_INTERVAL 1",
    "L2_BACK_TO_ZERO 0",
    "",
    "HAS_WIN 1",
    "GLOBAL_T 0",
    "VAR_WIN 1",
    "FAST_REACT 1",
    "U_TARGET 0.95",
    "MI_THRESH 0",
    "INT_MULTI 1",
    "MULTI_RATE 0",
    "SAMPLE_FEEDBACK 0",
    "PINT_LOG_BASE 1.05",
    "PINT_PROB 1.0",
    "NIC_TOTAL_PAUSE_TIME 0",
    "",
    "RATE_BOUND 1",
    "",
    "ACK_HIGH_PRIO 0",
    "",
    "LINK_DOWN 0 0 0",
    "",
    "ENABLE_TRACE 1",
    "",
    "KMAX_MAP 6 25000000000 400 40000000000 800 100000000000 1600 200000000000 2400 400000000000 3200 2400000000000 3200",
    "KMIN_MAP 6 25000000000 100 40000000000 200 100000000000 400 200000000000 600 400000000000 800 2400000000000 800",
    "PMAX_MAP 6 25000000000 0.2 40000000000 0.2 100000000000 0.2 200000000000 0.2 400000000000 0.2 2400000000000 0.2",
    "",
    "BUFFER_SIZE 32"
]


def generate_config_txt_from_wafer_file(wafer_file: str, physical_filepath: str, output_filepath: str):
    config = extract_config(wafer_file)
    ensure_dir(output_filepath)
    for entry in load_wafer_file(wafer_file):
        physical_file = os.path.join(
            physical_filepath,
            out_filename(config, entry.idx, "physical_network.txt")
        )
        lines = []
        for i, line in enumerate(CONFIG_TEMPLATE):
            if i == 5:
                lines.append(f"TOPOLOGY_FILE {physical_file}\n")
            else:
                lines.append(line + "\n")
        filename = out_filename(config, entry.idx, "config.txt")
        with open(os.path.join(output_filepath, filename), "w") as f:
            f.writelines(lines)
        print(f"Generated: {filename}")


def generate_config_txt(wafer_num: int, config: str, physical_filepath: str, output_filepath: str):
    """Backward-compatible wrapper."""
    class _FakeEntry:
        def __init__(self, idx):
            self.idx = idx
    ensure_dir(output_filepath)
    for idx in range(wafer_num):
        entry = _FakeEntry(idx)
        physical_file = os.path.join(
            physical_filepath,
            out_filename(config, idx, "physical_network.txt")
        )
        lines = []
        for i, line in enumerate(CONFIG_TEMPLATE):
            if i == 5:
                lines.append(f"TOPOLOGY_FILE {physical_file}\n")
            else:
                lines.append(line + "\n")
        filename = out_filename(config, idx, "config.txt")
        with open(os.path.join(output_filepath, filename), "w") as f:
            f.writelines(lines)
        print(f"Generated: {filename}")


if __name__ == "__main__":
    wafer_file = "/data/login_home/lijinxi/error_tolerance/output/0070/0070_Wafer.txt"
    config = extract_config(wafer_file)
    physical_filepath = f"/home/matsi/workspace/astra-sim/configs/{config}/physical_network/"
    output_filepath = "/data/login_home/lijinxi/error_tolerance/output/config/"
    generate_config_txt_from_wafer_file(wafer_file, physical_filepath, output_filepath)
