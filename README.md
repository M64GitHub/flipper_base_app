# M64 Flipper Base App

Note: the project structure was initially created via 
```
ufbt create APPID=m64_world
```

## Building
There are some differences between building this app "standalone" (in it's own folder - possible for the standard firmware), or in context / as a part of a whole (custom) firmware. When building for the standard-library, you can use the flipper simplified build tool `ufbt`. Else you will need to build with the standard flipper build tool `fbt`. 

- ufbt: https://github.com/flipperdevices/flipperzero-ufbt
- fbt: comes with your firmware (for example https://github.com/Next-Flip/Momentum-Firmware)

### Build for the standard firmware
Build:
```
ufbt
```
Build and run on connected flipper:
```
ufbt launch
```

### Build for custom firmware
To build/run for Momentum or other custom firmware:
 - clone firmware, cd into it
 - copy project into applications_user/

Build:
```
./fbt APPSRC=m64_world
```

Build and run on connected flipper:
```
./fbt launch APPSRC=m64_world
```

## neovim / lsp / clangd
The flipper build tools `fbt` and `ufbt` both generate a `compile_commands.json` file. This file can be used for the lsp configuration in neovim. In theory all you need to do is to instruct clangd where to find this compile_commands.json file. You can do this by using a `.clangd` file, and specify the folder. But there is at least one issue you will need to adress manually: both compile_commands.json are missing the includes to the standard libraries.

`ufbt` creates the build database in the subfolder `.vscode`. `fbt` will create the compile_commands.json in the folder `build/f7-firmware-C/`.  

I therefor created the .clangd file for this project:
```
CompileFlags:
  CompilationDatabase: .vscode/
# CompilationDatabase: ../../build/f7-firmware-C/
```
Please uncomment / comment corresponding to your build situation.  
When you experience errors like "stdlib.h" not found, please proceed like below and manually edit the compile_commands.json:

### For standard firmware, ufbt build 
- find the line for m64_world.c in `.vscode/compile_commands.json`
- insert `-Ipath-to-your-home-directory/.ufbt/toolchain/x86_64-linux/arm-none-eabi/include` into it

### For custom / full firmware, fbt build
 - find the line for m64_world.c in `build/f7-firmware-C/compile_commands.json`
 - insert `-Itoolchain/x86_64-linux/arm-none-eabi/include` into it



