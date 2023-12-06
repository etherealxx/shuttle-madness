import os
import subprocess
import shutil
from zipfile import ZipFile

def zip_build(source_dir, output_zip):
    exclude_files = set([
        'CMakeFiles',
        '.ninja_deps',
        '.ninja_log',
        'build.ninja',
        'cmake_install.cmake',
        'ninja.exe',
        'CMakeCache.txt',
        os.path.basename(output_zip)
    ])

    with ZipFile(output_zip, 'w') as zipf:
        for root, dirs, files in os.walk(source_dir):
            # Exclude unwanted files and folders
            dirs[:] = [d for d in dirs if d not in exclude_files]
            files = [f for f in files if f not in exclude_files]

            for file in files:
                file_path = os.path.join(root, file)
                arcname = os.path.relpath(file_path, source_dir)
                zipf.write(file_path, arcname)


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

if os.path.exists(build_folder_path):
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

ninja_exe_path = os.path.join(build_folder_path, "ninja.exe")
mingw_path = os.path.join(script_dir, "mingw32")
# if os.path.exists(mingw_path):
#     c_compiler_path = os.path.join(script_dir, r"mingw32\bin\gcc.exe").replace("\\","/")
#     cxx_compiler_path = os.path.join(script_dir, r"mingw32\bin\c++.exe").replace("\\","/")
#     subprocess.call(f"cmake -G \"Ninja\" -DCMAKE_MAKE_PROGRAM={ninja_exe_path} -DCMAKE_PREFIX_PATH={mingw_path} -DCMAKE_C_COMPILER={c_compiler_path} -DCMAKE_CXX_COMPILER={cxx_compiler_path} ..", shell=True, cwd=build_folder_path)
# else:
subprocess.call("cmake -G \"Ninja\" -DCMAKE_MAKE_PROGRAM=ninja.exe ..", shell=True, cwd=build_folder_path)
    
subprocess.call("cmake ..", shell=True, cwd=build_folder_path)
subprocess.call("ninja", shell=True, cwd=build_folder_path)
print(f"Done building: {build_folder_path}")

zip_filename = os.path.join(build_folder_path, "ShuttleMadness.zip")
zip_build(build_folder_path, zip_filename)
print(f"Zipped the build to {zip_filename}")