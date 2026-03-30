import os
import shutil

base = r"d:\VS projects\error_tolerance\pyscript"

# Create subdirectories
os.makedirs(os.path.join(base, "python"), exist_ok=True)
os.makedirs(os.path.join(base, "shell"), exist_ok=True)

# Python files to move
py_files = [
    "chakra_workload_generator.py",
    "gen_config_txt.py",
    "gen_logical_network.py",
    "gen_network.py",
    "gen_system.py",
    "simulated_annealing_ns3.py",
    "wafer_utils.py"
]

# Shell files to move
sh_files = ["generate_all_configs.sh"]

# Move Python files
for f in py_files:
    src = os.path.join(base, f)
    dst = os.path.join(base, "python", f)
    if os.path.exists(src):
        shutil.move(src, dst)
        print(f"Moved: {f} -> python/")

# Move shell files
for f in sh_files:
    src = os.path.join(base, f)
    dst = os.path.join(base, "shell", f)
    if os.path.exists(src):
        shutil.move(src, dst)
        print(f"Moved: {f} -> shell/")

print("\nReorganization complete!")
print("\nNew structure:")
for item in sorted(os.listdir(base)):
    path = os.path.join(base, item)
    if os.path.isdir(path) and not item.startswith("__"):
        print(f"  {item}/")
        for subitem in sorted(os.listdir(path)):
            print(f"    - {subitem}")
