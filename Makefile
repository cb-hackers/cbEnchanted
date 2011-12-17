BIN = CBEmu
export PKG_CONFIG_PATH = /usr/local/lib/pkgconfig/
SOURCES = main cbemu cbvariableholder mathinterface stringinterface textinterface fileinterface meminterface inputinterface sysinterface soundinterface animinterface imageinterface gfxinterface objectinterface camerainterface mapinterface effectinterface
OBJ = $(patsubst %,src/%.o,$(SOURCES))
LIBS = clanApp clanDisplay clanCore clanGL


PACKAGES = $(patsubst %,%-2.2,$(LIBS))
CXXFLAGS += `pkg-config --cflags $(PACKAGES)` -pthread -O3

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) `pkg-config --libs $(PACKAGES)`

clean:
	rm -f $(OBJ) $(BIN)

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
