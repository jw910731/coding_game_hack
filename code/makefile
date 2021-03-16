CC:= g++
OBJS:= detour.o main.o
EXE:= exe_bin

%.o:%.cpp
	$(CC) -c -o $@ $<

all:$(OBJS)
	gcc $(OBJS) -o $(EXE)
	strip $(EXE)
	cat $(EXE)|base64 > $(EXE).b64
clean:
	rm -rf $(OBJS) $(EXE) $(EXE).b64