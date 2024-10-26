# M64 Flipper Base App

Note: Project structure was initially created via 
```
ufbt create APPID=m64_world
```
## Building
There are some differences between building this app "standalone" (possible for the standard firmware), or in context / as a part of a whole (custom) firmware. When building for the standard-library, you can use the flipper simplified build tool `ufbt`. Else you will need to build with the standard flipper build tool `fbt`. 

### Build for the standard firmware
To build, you need to build against your firmware. On flipper standard firmware, you can use ufbt:
To build:
```
ufbt
```
To run on flipper: connect flipper, run
```
ufbt launch
```

### Build for custom firmware
To build/run for Momentum or other custom firmware:
 - clone firmware, cd into it
 - copy project into applications_user/
 - run `./fbt launch APPSRC=m64_world` to build and run on connected flipper, or
 - run `./fbt APPSRC=m64_world` to build only

## neovim / lsp / clangd
The flipper build tools both `fbt` and `ufbt` both generate a `compile_commands.json` file. This file can be used for the lsp configuration in neovim. In theory all you need to do is to instruct clangd where to find this compile_commands.json file. You can do this by using a `.clangd` file, and specify the folder. But there are some issues you will need to adress manually: both compile_commands.json are missing for example the includes to the standard libraries.

`ufbt` creates the build database in the subfolder `.vscode`. When you work with a full (standard or custom) firmware, `fbt` will create the compile_commands.json in the folder `build`.  

### For standard firmware, ufbt build

For using ufbt I created the following .clangd file in this project:
```
CompileFlags:
  CompilationDatabase: .vscode/
```
When you experience errors of not finding standard libraries - manually edit the compile_commands.json 
- insert -I`path-to-your-home-directory`/.ufbt/toolchain/x86_64-linux/arm-none-eabi/include/ into it

### For custom / full firmware, fbt build

```
-Itoolchain/x86_64-linux/arm-none-eabi/include
```
for the stdlib includes.
