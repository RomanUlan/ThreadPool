#Compilers
CC=gcc 
CXX=g++
CIDL=tao_idl

#IDL(s) directories
IDL_DIRS=

#IDL(s) out dir
IDL_OUT_DIR=./

#Source directories
SRC_DIRS= ./Threading ./EventEngines ./IO ./EchoResponder

#Headers directories
INC_DIRS=$(IDL_OUT_DIR)

#Compilers flags
CFLAGS=-MMD -std=c++11 -g -c -Wall
IFLAGS=-v -o $(IDL_OUT_DIR)

#Dynamic libraries to build
LIBRARIES=

#Dynamic libraries
LDFLAGS= -pthread

#Get all IDLs from IDL_DIRS
IDLS=$(foreach IDL_DIR,$(IDL_DIRS),$(wildcard $(IDL_DIR)/*.idl))

#All IDLs source files
IDL_SOURCES:=$(addprefix $(IDL_OUT_DIR)/,$(notdir $(subst .idl,C.cpp,$(IDLS)) $(subst .idl,S.cpp,$(IDLS))))

#All IDLs header files
IDL_HEADERS=$(addprefix $(IDL_OUT_DIR)/,$(notdir $(subst .idl,C.h,$(IDLS)) $(subst .idl,S.h,$(IDLS))))

#All IDLs inline files
IDL_INLINES=$(addprefix $(IDL_OUT_DIR)/,$(notdir $(subst .idl,C.inl,$(IDLS))))

#Compilation source files - all cpp files from SRC_DIRS and IDLs compilation result cpp files
SOURCES=$(IDL_SOURCES) $(foreach SRC_DIR,$(SRC_DIRS),$(wildcard $(SRC_DIR)/*.cpp))

#Libs to clean
LIBS_CLEAN=$(addsuffix _clean,$(LIBRARIES))

#Source compilation results
OBJECTS=$(SOURCES:.cpp=.o)

#Compilation dependencies files
DFILES=$(SOURCES:.cpp=.d)

#All idl directories
VPATH = $(IDL_DIRS)

#Build result
EXECUTABLE=prog

all: $(LIBRARIES) $(EXECUTABLE)

$(EXECUTABLE): $(IDL_SOURCES) $(OBJECTS)
	$(CXX) $(OBJECTS) -g -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $< $(foreach INC_DIR,$(INC_DIRS),-I$(INC_DIR)) -o $@

$(filter %C.cpp,$(IDL_SOURCES)):$(IDL_OUT_DIR)/%C.cpp: $(1)%.idl
	$(CIDL) $(IFLAGS) $<

$(filter %S.cpp,$(IDL_SOURCES)):$(IDL_OUT_DIR)/%S.cpp: $(1)%.idl
	$(CIDL) $(IFLAGS) $<

$(LIBRARIES):
	make -C $(dir $@)

clean:
	@-rm -fv $(OBJECTS) $(DFILES) 
	@-rm -fv $(IDL_SOURCES) $(IDL_HEADERS) $(IDL_INLINES)
	@-rm -fv $(EXECUTABLE)

$(LIBS_CLEAN):
	make clean -C $(dir $@)

clean_all: clean $(LIBS_CLEAN)

-include $(DFILES)

