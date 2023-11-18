# Shuttle Madness
 OpenGL C++ game made with glut.h, [stb_image.h](https://github.com/nothings/stb) and [miniaudio.h](https://github.com/mackron/miniaudio). Third semester final project. For Windows only. The point of this project is to use only single file header libraries (other than GLUT and standard library) to make a presentable arcade game.
<!--- ![image](https://github.com/etherealxx/shuttle-madness/assets/64251396/0bc9834d-fad1-4e6c-aa57-82a3e6d8e951) -->
<!-- <p align="center">
  <img src="https://github.com/etherealxx/shuttle-madness/assets/64251396/0bc9834d-fad1-4e6c-aa57-82a3e6d8e951" alt="shuttle-madness" width="500"/>
</p> -->
![shuttle_anim](https://github.com/etherealxx/shuttle-madness/assets/64251396/78e1388d-d4e6-46f2-98b4-1b0e641b3d8f)

## Workspace Setup (VSCode, the easy way)
- Clone this repository
- Download [this zip](https://github.com/MdAlbinHossain/Computer-Graphics/archive/refs/heads/main.zip) containing OpenGL dependencies from [MdAlbinHossain/Computer-Graphics](https://github.com/MdAlbinHossain/Computer-Graphics)
- Extract the zip, open the folder `Computer-Graphics > OpenGL Programming`
- Inside, copy 'mingw' and '.vscode' folder into this repo's folder you've cloned earlier (`shuttle-madness`)
- On VSCode, install an extension called Run++ ([this one](https://marketplace.visualstudio.com/items?itemName=AlbinBD.run))
- Still on VSCode, `File > Open Folder...` and choose the cloned directory (`shuttle-madness`)

#### The Manual Way
I haven't tried this myself, but you need to do a GLUT installation by yourself, and run the program with this (there might be other dependencies, tbh idk how to install those manually).
```
g++.exe shuttle_madness.cpp -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 -Wl,--subsystem,windows -o shuttle_madness
```

## Running the game
- Open `shuttle_madness.cpp`
- Click the `Run++` button on the top right, OR press Alt + B

#### Controls
- WASD controls: `W` to jump/double jump. `S` to fastfall/crouch.
- `Space` to swing the racket
- Immediately jumping after releasing crouch button will result in high jump.
- Hit the pink shuttlecock to score point, dodge the gray shuttlecock, and don't hit the yellow shuttlecock.

## Building the game
- Download `cmake` from [here](https://cmake.org/download/) and install it. (Choose the Windows x64 Installer, recommended to install for all user).
- **Note:** Apparently you need `mingw64` from `MSYS2` to build this game. I don't know why, but it wouldn't work when i tried to build it using the `mingw32` from the `OpenGL Programming` folder. You can install MSYS2 from [here](https://github.com/msys2/msys2-installer/releases/download/2023-10-26/msys2-x86_64-20231026.exe)

#### Python Method (assuming you installed one)
- Run `cmakeautomate.py` inside the `tools` folder
- Go to newly made `build` folder, the executable name is `ShuttleMadness.exe` (it needs the png and wav files to be on the same directory as it). There will also a zip file that contains everything you need to run the game.

#### Manual Method
- Inside the repo folder, create a new folder named `build`
- Download `ninja-win.zip` [from here](https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip)
- Extract it, put the `ninja.exe` inside the `build folder`
- Launch command prompt (`cmd.exe`), change directory (`cd`) to the build folder
- Type these in succesion
```
cmake -G "Ninja" -DCMAKE_MAKE_PROGRAM=ninja.exe ..
cmake ..
ninja
```
- Go to newly made `build` folder, the executable name is `ShuttleMadness.exe` (it needs the png and wav files to be on the same directory as it).

## Static Linking (Single Portable Executable)
- Download [Enigma Virtual Box](https://enigmaprotector.com/en/downloads.html), install it.

#### Python Method (assuming you installed one)
- Run `enigmavirtualboxsetup.py` inside the `tools` folder
- Double click the `shuttle_build.evb`, it should open the Enigma Virtual Box app.
- Click `Process` on the bottom right
- Visit `build` folder, the executable name is `ShuttleMadnessPortable.exe` (you can move it wherever you like)

#### Manual Method (assuming you installed one)
- Open the `Enigma Virtual Box` app.
- Browse the input file name, choose the newly built `ShuttleMadness.exe`
- Click `Add > Add File(s)`, choose every png and wav file inside the `build` folder
- Click `Process` on the bottom right
- Visit `build` folder, the executable name is `ShuttleMadnessPortable.exe` (you can move it wherever you like)

## To-Dos
- Make challenges (like how the way the shuttlecock spawns) as a reason for the player to not just stay on the bottom without jumping at all. (also challenges that utilizes ducking)
- Make the directory clean (separate files into folders based on extension)
- Figure out the build dependency problem
- Static linking the `libfreeglut.dll`

## Attribution
- `Arcade Music Loop.wav` by joshuaempyre on [freesound.org](https://freesound.org/people/joshuaempyre/sounds/251461/)
Adjusted so that the volume is 20% of the original

- `Jump_C_05.wav` by cabled_mess on [freesound.org](https://freesound.org/people/cabled_mess/sounds/350905/)

- `Tennis racket_whoosh.wav` by speedygonzo on [freesound.org](https://freesound.org/people/speedygonzo/sounds/235727/)
Cutted so that only the second last swing that plays

- `Badminton.wav` by PerMagnusLindborg on [freesound.org](https://freesound.org/people/PerMagnusLindborg/sounds/324244/)
Cutted so that it plays a little bit faster

- `8-bit damage sound.wav by EVRetro` on [freesound.org](https://freesound.org/people/EVRetro/sounds/501104/)
Cutted so that it ends earlier

- `retro taking damage sfx.wav` by stumpbutt on [freesound.org](https://freesound.org/people/stumpbutt/sounds/629656/)

- `8-bit Explosion2.aiff` by timgormly on [freesound.org](https://freesound.org/people/EVRetro/sounds/501104/)
Converted to 24bit pcm wav, decreased the volume to 50%

- `Game over sounds 1.wav` by afleetingspeck on [freesound.org](https://freesound.org/people/afleetingspeck/sounds/232444/)
Cutted so that only the first part is used

- `pause.mp3` by crisstanza on [freesound.org](https://freesound.org/people/crisstanza/sounds/167127/)
Converted to 24bit pcm wav, decreased the volume to 60%

- `Chiptune Loop Episode 01.wav` by orginaljun on [freesound.org](https://freesound.org/people/orginaljun/sounds/396960/)
Decreased the tempo by 30%

- `Menu screen mouse over.wav` by DrMinky on [freesound.org](https://freesound.org/people/DrMinky/sounds/166186/)
Amplified by 15dB

- `SFX UI Button Click.wav` by suntemple on [freesound.org](https://freesound.org/people/suntemple/sounds/253168/)
Amplified by 15dB

## Used Libraries
- [GLUT](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)/[FreeGLUT](https://github.com/FreeGLUTProject/freeglut)
- [stb_image](https://github.com/nothings/stb)
- [miniaudio](https://github.com/mackron/miniaudio)

## Special Thanks
- Ikhsan Ridwan who made the `shuttle_knight.png` sprite
