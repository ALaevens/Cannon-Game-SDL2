CC := x86_64-w64-mingw32-g++ -std=c++17 -O3 -mwindows
LIBRARY_ROOT := windowslibs/x64
LIBRARIES := -L$(LIBRARY_ROOT)/SDL2/lib -L$(LIBRARY_ROOT)/SDL2_image/lib -L$(LIBRARY_ROOT)/SDL2_mixer/lib -L$(LIBRARY_ROOT)/SDL2_ttf/lib -L$(LIBRARY_ROOT)/SDL2_gfx/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx

INCLUDE := -Iinclude -I$(LIBRARY_ROOT)/SDL2/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_image/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_mixer/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_ttf/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_gfx/include/SDL2
CFLAGS := -std=c++17 -O3 -mwindows
LFLAGS := -static-libgcc -static-libstdc++

TARGET := makebuild/cannon.exe

SRCS := $(wildcard src/*.cpp src/Engine/*.cpp src/Scenes/*.cpp)
OBJS := $(addprefix winobj/,$(subst src/, ,$(patsubst %.cpp,%.o,$(SRCS)))) # produces winobj/main.o winobj/Engine/Game.o etc

all: directories dlls $(TARGET)

$(TARGET): $(OBJS) cannon.res
	$(CC) $(LFLAGS) -o $@ $^ $(LIBRARIES)
	rsync -avh assets makebuild/ --delete

cannon.res:
	x86_64-w64-mingw32-windres cannon.rc -O coff -o cannon.res

winobj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)


clean:
	rm -rf makebuild/
	rm -rf winobj/

directories:
	mkdir -p makebuild/
	mkdir -p winobj/Engine
	mkdir -p winobj/Scenes

dlls:
	cp $(LIBRARY_ROOT)/SDL2/bin/SDL2.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/SDL2_image.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/libpng16-16.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/zlib1.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_mixer/bin/SDL2_mixer.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_ttf/bin/SDL2_ttf.dll makebuild/.
	cp $(LIBRARY_ROOT)/SDL2_gfx/bin/libSDL2_gfx-1-0-0.dll makebuild/.

.PHONY: all clean directories dlls