
SRCS=ole-bridge.cpp iid.cpp car.cpp car-class-factory.cpp

OBJS=$(SRCS:.cpp=.o)

CC=i586-mingw32msvc-g++
DEFINES=-Wall -MMD
LIBS=-luuid -lole32

default : install

OleBridge.dll : $(OBJS)
	$(CC) $(DEFINES) -shared -o $@ $(OBJS) $(LIBS) CarDll.def

#OleBridge.exe : $(OBJS)
#	$(CC) $(DEFINES) -o $@ $(OBJS) $(LIBS)

%.o : %.cpp
	$(CC) $(DEFINES) -c -o $@ $<

install : OleBridge.dll
	cp -f $< $(HOME)/.wine/drive_c/Tutu

clean:
	rm -rf *.o *.d *.exe *.dll

-include *.d
