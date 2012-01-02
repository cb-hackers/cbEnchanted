BIN = CBEnchanted
SOURCES = main cbenchanted cbvariableholder mathinterface stringinterface textinterface fileinterface meminterface inputinterface sysinterface soundinterface animinterface imageinterface gfxinterface objectinterface camerainterface mapinterface effectinterface mathoperations drawingprimitives rendertargetpointer cbimage
OBJ = $(patsubst %,src/%.o,$(SOURCES))

CXXFLAGS += -pthread -O3 -Wno-int-to-pointer-cast -fpermissive -DLOG_LEVEL_STUB -DLOG_LEVEL_FIXME -DLOG_LEVEL_DEBUG -DLOG_LEVEL_INFO

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio -lGL -lboost_filesystem -lboost_system

clean:
	rm -f $(OBJ) $(BIN)

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
