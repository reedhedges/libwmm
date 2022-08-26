INSTALL_DIR ?= /usr/local
OBJ_DIR ?= obj
LIB_DIR ?= lib

CC ?= gcc
CXX ?= g++
AR ?= ar
RANLIB ?= ranlib
INSTALL ?= install

COMMON_CFLAGS = -g -O3 -fPIC -Wall -W -Wextra -Iinclude -Isrc  #-flto=auto
CFLAGS = $(COMMON_CFLAGS) $(EXTRA_CFLAGS)
CXXFLAGS = -std=c++20 $(COMMON_CFLAGS) $(EXTRA_CXXFLAGS)
# Note could also use -flto=thin on Clang, or -flto if -flto=auto is not supported.
LDFLAGS = #-fuse-linker-plugin # allow linking programs to use LTO
LIBS = -lm

all: $(LIB_DIR)/libwmm.a

info: FORCE
	@echo
	@echo libwmm:
	@echo   CC=$(CC)
	@echo   CXX=$(CXX)
	@echo   OBJ_DIR=$(OBJ_DIR)
	@echo   LIB_DIR=$(LIB_DIR)
	@echo   INSTALL_DIR=$(INSTALL_DIR)
	@echo   AR=$(AR)
	@echo   RANLIB=$(RANLIB)
	@echo   CFLAGS=$(CFLAGS)
	@echo   CXXFLAGS=$(CXXFLAGS)
	@echo   EXTRA_LDFLAGS=$(EXTRA_LDFLAGS)
	@echo

$(LIB_DIR)/libwmm.a: $(OBJ_DIR)/wmm.o $(OBJ_DIR)/GeomagnetismLibrary.o $(OBJ_DIR)/EGM9615.o
	@mkdir -p $(LIB_DIR)
	$(AR) crv $@ $^ 
	$(RANLIB) $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJ_DIR)/%.o: src/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

wmm_point: src/wmm_point.o src/GeomagnetismLibrary.o src/EGM9615.o
	$(CC) -o $@ $(CFLAGS) $^ $(LIBS)

test: test.cc $(LIB_DIR)/libwmm.a
	$(CXX) -o $@ $(CXXFLAGS) $< $(LIB_DIR)/libwmm.a $(LIBS)

clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f $(LIB_DIR)/libwmm.a

install: $(LIB_DIR)/libwmm.a include/libwmm/wmm.hh
	$(INSTALL) -D -m 644 $(LIB_DIR)/libwmm.a $(INSTALL_DIR)/lib/libwmm.a
	$(INSTALL) -D -m 644 include/libwmm/wmm.hh $(INSTALL_DIR)/include/libwmm/wmm.hh
	

# (Re)-generate compile_commands.json which is used by tools like Intellisense (VS Code)
# clang-tidy, other linters/analyzers.  Regenerate this if VS code won't get rid of some 
# compile errors in the Problems panel.
# "bear" tool is required. Install with apt install bear.
compile_commands.json: Makefile clean
	bear -- $(MAKE)

.PHONY: all clean install

Makefile.dep:
	$(CXX) $(CFLAGS) -MM src/*.c src/*.cc | awk '$$1 ~ /:/{printf "$(OBJ_DIR)/%s\n", $$0} $$1 !~ /:/' > Makefile.dep

include Makefile.dep
	
FORCE:

