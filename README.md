# particles

Implements particle demos

![firework](https://github.com/thumun/particles/blob/main/img/firework.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ start particles.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/billboard-axis.exe
particles/build $ ../bin/Debug/explosion.exe
particles/build $ ../bin/Debug/sparkle-trail.exe
particles/build $ ../bin/Debug/sparkles.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/billboard-axis
particles/build $ ../bin/sparkles
particles/build $ ../bin/sparkle-trail
particles/build $ ../bin/explosion
```

## Demo of basic features

Sparkle-Trail:
![sparkle trail](https://github.com/thumun/particles/blob/main/img/particles.gif)

Explosion:
![explosion](https://github.com/thumun/particles/blob/main/img/explosion.gif)

Billboard-axis:
![billboard](https://github.com/thumun/particles/blob/main/img/billboard.gif)

## Unique features 

Firework: 
![firework](https://github.com/thumun/particles/blob/main/img/particles.gif)
