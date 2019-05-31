COMPILER = clang
INCLUDE_PATHS = -IC:\msys64\mingw64\include -Isource
LIBRARY_PATHS = -LC:\msys64\mingw64\lib
COMPILER_FLAGS = -std=c11 -O2 -fms-extensions -Wall -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-multichar
LINKER_FLAGS = -lmingw32
FILES = source/**/*.c
OUTPUT = bin/zeta.exe

all: $(FILES)
	$(COMPILER) $(FILES) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)

run:
	$(OUTPUT)
