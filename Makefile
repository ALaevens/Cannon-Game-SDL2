GPP = x86_64-w64-mingw32-g++ -std=c++17 -O3
LIBRARY_ROOT = windowslibs/x64

INCLUDE = -Iinclude -I$(LIBRARY_ROOT)/SDL2/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_image/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_mixer/include/SDL2
LINK = -L$(LIBRARY_ROOT)/SDL2/lib -L$(LIBRARY_ROOT)/SDL2_image/lib -L$(LIBRARY_ROOT)/SDL2_mixer/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

all: out.exe

out.exe: directories dlls main.o game.o Window.o Clock.o util.o TextureManager.o ECS.o
	$(GPP) -static-libgcc -static-libstdc++ -o makebuild/final/out.exe makebuild/main.o makebuild/game.o makebuild/ECS.o makebuild/Window.o makebuild/Clock.o makebuild/util.o makebuild/TextureManager.o $(LINK)
	rsync -avh assets makebuild/final/ --delete
	

main.o: src/main.cpp
	$(GPP) -o makebuild/main.o -c src/main.cpp $(INCLUDE)

ECS.o: src/Engine/ECS.cpp
	$(GPP) -o makebuild/ECS.o -c src/Engine/ECS.cpp $(INCLUDE)

game.o: src/game.cpp
	$(GPP) -o makebuild/game.o -c src/game.cpp $(INCLUDE)

Window.o: src/Engine/Window.cpp
	$(GPP) -o makebuild/Window.o -c src/Engine/Window.cpp $(INCLUDE)

Clock.o: src/Engine/Clock.cpp
	$(GPP) -o makebuild/Clock.o -c src/Engine/Clock.cpp $(INCLUDE)

util.o: src/Engine/util.cpp
	$(GPP) -o makebuild/util.o -c src/Engine/util.cpp $(INCLUDE)

TextureManager.o: src/Engine/TextureManager.cpp
	$(GPP) -o makebuild/TextureManager.o -c src/Engine/TextureManager.cpp $(INCLUDE)

directories:
	mkdir -p makebuild/final/

clean:
	rm makebuild/*.o

dlls:
	cp $(LIBRARY_ROOT)/SDL2/bin/SDL2.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/SDL2_image.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/libpng16-16.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/zlib1.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_mixer/bin/SDL2_mixer.dll makebuild/final/.

