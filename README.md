# M64 Flipper Base App

Note: Project structure was created using 

```
ubft create APPID=m64_world
```

To build, you need to build against your firmware. On flipper standard firmware, you can continue to use ubft:
To build:
```
ubft
```
To run on flipper: connect flipper, run
```
ubft launch
```

To build/run for Momentum or other custom firmware:
 - clone firmware, cd into it
 - copy project into applications_user/
 - run `./fbt launch APPSRC=m64_world` to build and run on connected flipper, or
 - run `./fbt APPSRC=m64_world` to build only

For neovim lsp configuration: create a compile_command.json: grep the lines containing 'm64_world' out of your 
./build/f7-firmware-C/compile_commands.json, after you have built via fbt. Then add the include 
```
-Itoolchain/x86_64-linux/arm-none-eabi/include
```
for the stdlib includes.
