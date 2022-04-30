GPP = x86_64-w64-mingw32-g++ -std=c++17
LIBRARY_ROOT = windowslibs/x64

INCLUDE = -Iinclude -I$(LIBRARY_ROOT)/SDL2/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_image/include/SDL2 -I$(LIBRARY_ROOT)/SDL2_mixer/include/SDL2
LINK = -L$(LIBRARY_ROOT)/SDL2/lib -L$(LIBRARY_ROOT)/SDL2_image/lib -L$(LIBRARY_ROOT)/SDL2_mixer/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

out.exe: directories dlls main.o Window.o Clock.o util.o TextureManager.o
	$(GPP) -static-libgcc -static-libstdc++ -o makebuild/final/out.exe makebuild/main.o makebuild/Window.o makebuild/Clock.o makebuild/util.o makebuild/TextureManager.o $(LINK)
	rsync -avh assets makebuild/final/ --delete
	

main.o: src/main.cpp
	$(GPP) -o makebuild/main.o -c src/main.cpp $(INCLUDE)
Window.o: src/Window.cpp
	$(GPP) -o makebuild/Window.o -c src/Window.cpp $(INCLUDE)

Clock.o: src/Clock.cpp
	$(GPP) -o makebuild/Clock.o -c src/Clock.cpp $(INCLUDE)

util.o: src/util.cpp
	$(GPP) -o makebuild/util.o -c src/util.cpp $(INCLUDE)

TextureManager.o: src/TextureManager.cpp
	$(GPP) -o makebuild/TextureManager.o -c src/TextureManager.cpp $(INCLUDE)

directories:
	mkdir -p makebuild/final/

dlls:
	cp $(LIBRARY_ROOT)/SDL2/bin/SDL2.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/SDL2_image.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/libpng16-16.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_image/bin/zlib1.dll makebuild/final/.
	cp $(LIBRARY_ROOT)/SDL2_mixer/bin/SDL2_mixer.dll makebuild/final/.
