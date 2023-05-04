# Model Viewer
Model viewing (and potentially sculpting) program written in C on OpenGL with GLFW/GLAD. I have a few plans for this engine, namely model importing/exporting, and voxel sculpting.

#### TODO
- [ ] Finish README
- [ ] Model Textures
- [ ] Model exporting
- [ ] Voxel sculpting
- [ ] Model sculpting
- [ ] Fix my sleep schedule

## Building
### Windows
This project doesn't use Make or CMake. If you're building this program on Windows, all of the binaries are already provided. You may run the following from the root directory:
```cmd
clang -o program.exe src/main.c -Iinclude/ -Llibs/ -lglfw3_mt -luser32 -lgdi32 -lshell32 -lglad -lcglm -lassimp-vc143-mtd.lib
```
and obviously whatever debugging or optimization options you wish to use. Any compiler options should work fine with this program.

> `Use -DDEBUG flag for mklog messages. If not enabled, mklog does nothing.`

> There should be no arbitrary FPS caps as VSync is disabled, but let me know otherwise.

### Linux/MacOS
The project can be built using the same command, but the library binaries will have to be compiled for your system (with the exception of shell32, gdi32 and user32, which are Windows-specific binaries, and Windows dependencies for GLFW).

## Issues
If you have an issue with linking GLFW, try looking into what external binaries it might depend on. I had this issue for Windows. If you built GLFW with CMake you probably won't run into these issues, but I did, because I didn't use CMake.
