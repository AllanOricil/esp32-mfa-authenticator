import os
import subprocess
import shutil


def check_installation(tool):
    if shutil.which(tool) is None:
        raise Exception(f"{tool} is not installed. Please install it to continue.")


print("Running install-node-deps.py")

# Check if Node and pnpm are installed
check_installation("node")
check_installation("npm")

if not os.path.isdir("node_modules"):
    subprocess.run(["npm", "install"])
