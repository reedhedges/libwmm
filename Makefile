CC ?= gcc
CXX ?= g++
AR ?= ar
RANLIB ?= ranlib
CFLAGS = -O3 -flto=auto -Wall -W -Wextra -Iinclude -Isrc
CXXFLAGS = -std=c++20 $(CFLAGS)
# Note could also use -flto=thin on Clang, or -flto if -flto=auto is not supported.
LDFLAGS = -fuse-linker-plugin # allow linking programs to use LTO
LIBS = -lm

all: libwmm.a test

libwmm.a: obj/wmm.o obj/GeomagnetismLibrary.o
	$(AR) -cr $@ $^ 
	$(RANLIB) $@

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) -c -o $@ $(CFLAGS) $<

obj/%.o: src/%.cc
	@mkdir -p obj
	$(CXX) -c -o $@ $(CXXFLAGS) $<

wmm_point: src/wmm_point.o src/GeomagnetismLibrary.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

test: test.cc libwmm.a
	$(CXX) -o $@ $(CXXFLAGS) $< libwmm.a $(LIBS)

clean:
	-rm -f obj/*.o
	-rm -f libwmm.a

# (Re)-generate compile_commands.json which is used by tools like Intellisense (VS Code)
# clang-tidy, other linters/analyzers.  Regenerate this if VS code won't get rid of some 
# compile errors in the Problems panel.
# "bear" tool is required. Install with apt install bear.
compile_commands.json: Makefile clean
	bear -- $(MAKE)

.PHONY: all clean dep

Makefile.dep:
	$(CXX) $(CFLAGS) -MM src/*.c src/*.cc | awk '$$1 ~ /:/{printf "obj/%s\n", $$0} $$1 !~ /:/' > Makefile.dep

include Makefile.dep
	
