# Pixel Projectiles

## Screenshots
**Main Menu**
![main menu](https://user-images.githubusercontent.com/44736322/198861932-4b2ab919-4ce1-4ede-bc25-ad0ddd0b9fcc.png)

**Game Screen**
![gamewide](https://user-images.githubusercontent.com/44736322/198861910-93166471-56a1-4ee6-bd99-a3c3a9c7f020.png)

**Controls Screen**
![controls](https://user-images.githubusercontent.com/44736322/198861915-6b56c5ef-3bb5-4153-a3ac-655cbbdb8a88.png)

---

## Building
I developed this on Ubuntu, so that is the only platform I can give exact build instructions for. However, using x86_64-w64-mingw32-g++ allows the application to be cross compiled for the Windows platform.

### Prerequisites
```
# Runtime libraries
$ sudo apt-get install libsdl2-2.0-0 libsdl2-gfx-1.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0 libsdl2-ttf-2.0-0

# Development libraries
$ sudo apt-get install libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

# Compilation tools
$ sudo apt-get install make cmake g++ mingw-w64

# Get the source code
$ git clone --recurse-submodules https://github.com/ALaevens/Cannon-Game-SDL2
$ cd Cannon-Game-SDL2

```

### Building for Ubuntu
```
$ mkdir build
$ cd build
$ cmake ../ -DCMAKE_BUILD_TYPE=Release
$ make 

# Then to run it simply:
$ ./cannon
```

### Building for Windows
```
# The makefile in the root directory will take care of it
# Copy the contents of the generated "makebuild" folder to a windows PC and run cannon.exe
$ make
```
