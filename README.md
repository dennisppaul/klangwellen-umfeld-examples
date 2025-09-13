A Series of Examples for Klangwellen using Umfeld

## build and run examples

to build and run examples navigate to an example folder and execute CMake commands:

```
$ cd ./examples/simple-example/
$ cmake -B build .
$ cmake --build build --target run
```

to rebuild and run ( e.g after having made changes to source code ) just execute:

```
$ cmake --build build --target run
```

to clean the build folder run:

```
$ cmake --build build --target clean
```
