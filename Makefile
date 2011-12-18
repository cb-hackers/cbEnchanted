BIN = CBEmu
export PKG_CONFIG_PATH = /usr/local/lib/pkgconfig/
SOURCES = main cbemu cbvariableholder mathinterface stringinterface textinterface fileinterface meminterface inputinterface sysinterface soundinterface animinterface imageinterface gfxinterface objectinterface camerainterface mapinterface effectinterface mathoperations drawingprimitives
OBJ = $(patsubst %,src/%.o,$(SOURCES))

CXXFLAGS += -pthread -O3

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) -lsfml-window -lsfml-graphics -lGL

clean:
	rm -f $(OBJ) $(BIN)

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
