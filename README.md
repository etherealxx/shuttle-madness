# Shuttle Madness
 OpenGL C++ game made with glut.h, [stb_image.h](https://github.com/nothings/stb) and [miniaudio.h](https://github.com/mackron/miniaudio). Third semester final project. For Windows only. The point of this project is to use only single file header libraries (other than GLUT and standard library) to make a presentable arcade game.
<!--- ![image](https://github.com/etherealxx/shuttle-madness/assets/64251396/0bc9834d-fad1-4e6c-aa57-82a3e6d8e951) -->
<!-- <p align="center">
  <img src="https://github.com/etherealxx/shuttle-madness/assets/64251396/0bc9834d-fad1-4e6c-aa57-82a3e6d8e951" alt="shuttle-madness" width="500"/>
</p> -->

## Workspace Setup (VSCode, the easy way)
- Clone this repository
- Download [this zip](https://github.com/MdAlbinHossain/Computer-Graphics/archive/refs/heads/main.zip) containing OpenGL dependencies from [MdAlbinHossain/Computer-Graphics](https://github.com/MdAlbinHossain/Computer-Graphics)
- Extract the zip, open the folder `Computer-Graphics > OpenGL Programming`
- Inside, copy 'mingw' and '.vscode' folder into this repo's folder you've cloned earlier (`shuttle-madness`)
- On VSCode, install an extension called Run++ ([this one](https://marketplace.visualstudio.com/items?itemName=AlbinBD.run))
- Still on VSCode, `File > Open Folder...` and choose the cloned directory (`shuttle-madness`)

## Running the game
- Open `shuttle_madness.cpp`
- Click the `Run++` button on the top right, OR press Alt + B

#### Controls
- WASD controls: `W` to jump/double jump. `S` to fastfall/crouch.
- `Space` to swing the racket
- Immediately jumping after releasing crouch button will result in high jump.
- Hit the pink shuttlecock to score point, dodge the gray shuttlecock, and don't hit the yellow shuttlecock.

## Building the game
- Download cmake from [here](https://cmake.org/download/) and install it. (Choose the Windows x64 Installer, recommended to install for all user).

#### Python Method (assuming you installed one)
- Run `cmakeautomate.py` inside the `tools` folder
- Go to newly made `build` folder, the executable name is `ShuttleMadness.exe` (it needs the png and wav files to be on the same directory as it).

#### Manual Method
- Inside the repo folder, create a new folder named `build`
- Download ninja-win.zip [from here](https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip)
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
- Visit `build` folder, the executable name is `ShuttleMadness_boxed.exe` (you can move it wherever you like)

#### Manual Method (assuming you installed one)
- Open the Enigma Virtual Box app.
- Browse the input file name, choose the newly built `ShuttleMadness.exe`
- Click `Add > Add File(s)`, choose every png and wav file inside the `build` folder
- Click `Process` on the bottom right
- Visit `build` folder, the executable name is `ShuttleMadness_boxed.exe` (you can move it wherever you like)

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

## Used Libraries
- [GLUT](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)
- [stb_image](https://github.com/nothings/stb)
- [miniaudio](https://github.com/mackron/miniaudio)
- [FreeGLUT](https://github.com/FreeGLUTProject/freeglut)

## Special Thanks
- Ikhsan Ridwan who made the `shuttle_knight.png` sprite
