# straight from chatgpt cuz i'm lazy and my code has been assessed anyway, so here's just a bonus
import os
import subprocess
from PIL import Image, ImageDraw

# Get the current script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))

# Check if the last directory component is "tools"
if os.path.basename(script_dir) == "tools":
    # If it is, step up one directory
    script_dir = os.path.dirname(script_dir)

# Define icon dimensions
icon_size = 32
icon = Image.new('RGBA', (icon_size, icon_size), (0, 0, 0, 0))
draw = ImageDraw.Draw(icon)

# red square
draw.rectangle([0, 0, icon_size * 4 // 5, icon_size - 1], fill=(255, 0, 0, 255))

# white square on the right part
draw.rectangle([icon_size * 4 // 5, 0, icon_size - 1, icon_size - 1], fill=(255, 255, 255, 255))

# black stroke wrapping the square
stroke_width = 3
draw.rectangle([0, 0, icon_size - 1, icon_size - 1], outline=(0, 0, 0, 255), width=stroke_width)

icon.save(os.path.join(script_dir, "shuttle_icon.ico"))
print("Icon (shuttle_icon.ico) created.")

with open(os.path.join(script_dir,'shuttle_icon.rc'), 'w') as file:
    file.write('''IDI_ICON1               ICON        DISCARDABLE            shuttle_icon.ico''')
print("shuttle_icon.rc created.")

windres_path = os.path.join(script_dir, r"mingw32\bin\windres.exe")
if os.path.exists(windres_path):
    subprocess.call(f"\"{windres_path}\" shuttle_icon.rc -o shuttle_icon.o", shell=True, cwd=script_dir)
    print("shuttle_icon.o created.")
    
windres_path_64 = r"C:\msys64\mingw64\bin\windres.exe" # assuming default path
if os.path.exists(windres_path_64):
    subprocess.call(f"\"{windres_path_64}\" --target=\"pe-x86-64\" shuttle_icon.rc -o shuttle_icon_64.o", shell=True, cwd=script_dir)
    print("shuttle_icon_64.o created.")