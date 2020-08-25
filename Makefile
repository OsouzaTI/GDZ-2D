build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*cpp \
	-o game.exe \
	-I"C:\libsdl\SDL2\include" \
	-L"C:\libsdl\SDL2\lib" \
	-I".\lib\lua" \
	-L".\lib\lua" \
	-llua54 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-LSDL_ttf \
	-LSDL2_mixer \
	-lSDL2

clean: del game.exe

run: start .\game.exe

