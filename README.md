# M64 Flipper Base App

Note: Project structure was created using 

```
ufbt create APPID=m64_world
```
## Build for the standard firmware
To build, you need to build against your firmware. On flipper standard firmware, you can use ufbt:
To build:
```
ufbt
```
To run on flipper: connect flipper, run
```
ufbt launch
```

## Build for custom firmware
To build/run for Momentum or other custom firmware:
 - clone firmware, cd into it
 - copy project into applications_user/
 - run `./fbt launch APPSRC=m64_world` to build and run on connected flipper, or
 - run `./fbt APPSRC=m64_world` to build only

## neovim / lsp / clangd
The flipper build tools both `fbt` and `ufbt` generate a `compile_commands.json` file. This file can be used for the lsp configuration in neovim. In theory all you need to do is to instruct clangd where to find this compile_commands.json file. You can do this by using a `.clangd` file, and specifying the folder as in the example below. In practice there is at least the issue, these generated files do not include references to the standard libraries.  
```
CompileFlags:
  CompilationDatabase: .vscode/
```



### For standard firmware / building via ufbt
I therefor created the .clangd file in the project root:
```
CompileFlags:
  Add: -ferror-limit=0
  CompilationDatabase: build/
```



ufbt will create the compile_commands.json in a subfolder .vscode/ . Custom firmwares (or when you compile the project as part of the whole firmware) - either way, they create the 


The first entry disables the Lsp from stopping and displaying "too many errors" (especially useful on some include files). Next it instructs clangd to look into .vscode/ for the compile_commands.json.  
This would be a clean way for getting the lsp to work, but unfortunately it does not include the standard libraries. 

### For standard firmware
ufbt creates a directory .vscode/ where the compile_commands.json con be found. The provided .clangd file points the lsp there automatically. Unfortunately, this compilationdatabase does not include the standard libraries.

### For custom firmware
Create a compile_command.json: grep the lines containing 'm64_world' out of your 
./build/f7-firmware-C/compile_commands.json, after you have built via fbt. Then add the include 
```
-Itoolchain/x86_64-linux/arm-none-eabi/include
```
for the stdlib includes.
