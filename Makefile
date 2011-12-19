BIN = CBEnchanted
export PKG_CONFIG_PATH = /usr/local/lib/pkgconfig/
SOURCES = main cbenchanted cbvariableholder mathinterface stringinterface textinterface fileinterface meminterface inputinterface sysinterface soundinterface animinterface imageinterface gfxinterface objectinterface camerainterface mapinterface effectinterface mathoperations drawingprimitives
OBJ = $(patsubst %,src/%.o,$(SOURCES))

CXXFLAGS += -pthread -O3 -Wno-int-to-pointer-cast -fpermissive

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) -lsfml-window -lsfml-graphics -lsfml-system -lGL

clean:
	rm -f $(OBJ) $(BIN)

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
