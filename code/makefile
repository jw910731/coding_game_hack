CXX = g++
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CXXFLAGS+=-Wall -Wextra -std=c++17
LDFLAGS=
STRIP=1 

TARGET = exe_bin
OBJ= main.o detour.o maze.o robot.o mapAdapter.o

.PHONY: all

all: CXXFLAGS += -O3
all: STRIP=1
all: $(TARGET) 

debug: CXXFLAGS += -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS += -fsanitize=address -lubsan -lasan 
debug: STRIP=0
debug: $(TARGET)

dev: CXXFLAGS += -g -DDEBUG
dev: STRIP=0
dev: $(TARGET)

finalize: $(TARGET)
	if [ $(STRIP) = "1" ]; then \
		strip $(TARGET); \
	fi
	cat $(TARGET)|base64 > $(TARGET).b64

.SECONDEXPANSION:
$(TARGET): $(patsubst %, $(OBJDIR)/%, $(OBJ))
	$(CXX) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.d: %.cpp
	$(CXX) -MM $(CXXFLAGS) -o $@ $<

include $(patsubst %.o, $(OBJDIR)/%.d, $(OBJ))

clean:
	rm -rf $(TARGET) $(BUILD_DIR) obj
