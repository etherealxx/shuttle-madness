import os

def uprcsdrv(input_str): # turn drive letter into uppercase
    if len(input_str) >= 2 and input_str[0].isalpha() and input_str[1] == ':':
        result = input_str[0].upper() + input_str[1:]
    else:
        result = input_str
    return result

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

# Check if the folder exists
if os.path.exists(build_folder_path) and os.path.isdir(build_folder_path):
    # print(f"Absolute path to '{folder_name}' folder: {build_folder_path}")
    
    # Create the text to be written into the "test.evb" file
    text_to_write = f"""<?xml version="1.0" encoding="windows-1252"?>
<>
  <InputFile>{uprcsdrv(os.path.join(build_folder_path, "ShuttleMadness.exe"))}</InputFile>
  <OutputFile>{uprcsdrv(os.path.join(build_folder_path, "ShuttleMadnessPortable.exe"))}</OutputFile>
  <Files>
    <Enabled>True</Enabled>
    <DeleteExtractedOnExit>False</DeleteExtractedOnExit>
    <CompressFiles>False</CompressFiles>
    <Files>
      <File>
        <Type>3</Type>
        <Name>%DEFAULT FOLDER%</Name>
        <Action>0</Action>
        <OverwriteDateTime>False</OverwriteDateTime>
        <OverwriteAttributes>False</OverwriteAttributes>
        <HideFromDialogs>0</HideFromDialogs>
        <Files>\n"""
    
    # Define the list of file extensions to search for
    file_extensions = [".png", ".wav", ".dll"]
    specificfile = ["shuttle_howtoplay.txt", "shuttle_audio_credit.txt"]

    # Iterate over the files in the build folder with specified extensions
    for file in os.listdir(build_folder_path):
        if any(file.endswith(ext) for ext in file_extensions):
            file_path = uprcsdrv(os.path.join(build_folder_path, file))
            
            # Add the XML snippet to "test.evb" for each file
            text_to_write += f"""          <File>
            <Type>2</Type>
            <Name>{file}</Name>
            <File>{file_path}</File>
            <ActiveX>False</ActiveX>
            <ActiveXInstall>False</ActiveXInstall>
            <Action>0</Action>
            <OverwriteDateTime>False</OverwriteDateTime>
            <OverwriteAttributes>False</OverwriteAttributes>
            <PassCommandLine>False</PassCommandLine>
            <HideFromDialogs>0</HideFromDialogs>
          </File>
"""

        elif any((file == filename) for filename in specificfile):
            file_path = uprcsdrv(os.path.join(build_folder_path, file))
            
            # Add the XML snippet to "test.evb" for each file
            text_to_write += f"""          <File>
            <Type>2</Type>
            <Name>{file}</Name>
            <File>{file_path}</File>
            <ActiveX>False</ActiveX>
            <ActiveXInstall>False</ActiveXInstall>
            <Action>0</Action>
            <OverwriteDateTime>False</OverwriteDateTime>
            <OverwriteAttributes>False</OverwriteAttributes>
            <PassCommandLine>False</PassCommandLine>
            <HideFromDialogs>0</HideFromDialogs>
          </File>
"""  
    
    text_to_write += f"""        </Files>
      </File>
    </Files>
  </Files>
  <Registries>
    <Enabled>False</Enabled>
    <Registries>
      <Registry>
        <Type>1</Type>
        <Virtual>True</Virtual>
        <Name>Classes</Name>
        <ValueType>0</ValueType>
        <Value/>
        <Registries/>
      </Registry>
      <Registry>
        <Type>1</Type>
        <Virtual>True</Virtual>
        <Name>User</Name>
        <ValueType>0</ValueType>
        <Value/>
        <Registries/>
      </Registry>
      <Registry>
        <Type>1</Type>
        <Virtual>True</Virtual>
        <Name>Machine</Name>
        <ValueType>0</ValueType>
        <Value/>
        <Registries/>
      </Registry>
      <Registry>
        <Type>1</Type>
        <Virtual>True</Virtual>
        <Name>Users</Name>
        <ValueType>0</ValueType>
        <Value/>
        <Registries/>
      </Registry>
      <Registry>
        <Type>1</Type>
        <Virtual>True</Virtual>
        <Name>Config</Name>
        <ValueType>0</ValueType>
        <Value/>
        <Registries/>
      </Registry>
    </Registries>
  </Registries>
  <Packaging>
    <Enabled>False</Enabled>
  </Packaging>
  <Options>
    <ShareVirtualSystem>False</ShareVirtualSystem>
    <MapExecutableWithTemporaryFile>True</MapExecutableWithTemporaryFile>
    <TemporaryFileMask/>
    <AllowRunningOfVirtualExeFiles>True</AllowRunningOfVirtualExeFiles>
    <ProcessesOfAnyPlatforms>False</ProcessesOfAnyPlatforms>
  </Options>
  <Storage>
    <Files>
      <Enabled>False</Enabled>
      <Folder>%DEFAULT FOLDER%\</Folder>
      <RandomFileNames>False</RandomFileNames>
      <EncryptContent>False</EncryptContent>
    </Files>
  </Storage>
</>"""

    # Create the absolute path to the "shuttle_build.evb" file
    shuttle_build_path = os.path.join(script_dir, "shuttle_build.evb")
    
    # Write the text to the "shuttle_build.evb" file
    with open(shuttle_build_path, 'w') as evb_file:
        evb_file.write(text_to_write)
    
    print(f"evb file written to '{shuttle_build_path}'")
else:
    print(f"'{folder_name}' folder not found in the script's directory.")