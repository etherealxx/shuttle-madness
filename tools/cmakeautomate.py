import os
import subprocess
import shutil

# Get the current script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))

# Check if the last directory component is "tools"
if os.path.basename(script_dir) == "tools":
    # If it is, step up one directory
    script_dir = os.path.dirname(script_dir)

# Define the folder name you want to detect
folder_name = "build"

# Create the absolute path to the folder
build_folder_path = os.path.join(script_dir, folder_name)

shutil.rmtree(build_folder_path)
os.makedirs(build_folder_path, exist_ok=True)

# Define the URL to download and the filename
url = "https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip"
filename = "ninja-win.zip"
ninja_zip_path = os.path.join(build_folder_path, filename)

unzip_command = f"powershell Expand-Archive -Path {ninja_zip_path} -Destination {build_folder_path}"

# Download the file using curl
download_command = f"curl -L -o {ninja_zip_path} {url}"
subprocess.call(download_command, shell=True)

# Check if the download was successful
if os.path.exists(ninja_zip_path):
    # Extract the downloaded zip file
    subprocess.call(unzip_command, shell=True)

    # Clean up the zip file
    os.remove(ninja_zip_path)
    print("Done downloading ninja.")
else:
    print("Download failed.")
    exit()

subprocess.call("cmake -G \"Ninja\" -DCMAKE_MAKE_PROGRAM=ninja.exe ..", shell=True, cwd=build_folder_path)
subprocess.call("cmake ..", shell=True, cwd=build_folder_path)
subprocess.call("ninja", shell=True, cwd=build_folder_path)
print(f"Done building: {build_folder_path}")