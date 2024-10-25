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


