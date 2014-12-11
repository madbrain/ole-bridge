
SRCS=ole-bridge.cpp iid.cpp component.cpp component-class-factory.cpp

OBJS=$(SRCS:.cpp=.o)

#PREFIX=i586-mingw32msvc-
CC=$(PREFIX)g++
DEFINES=-Wall -MMD
LIBS=-luuid -lole32

default : install

OleBridge.dll : $(OBJS)
	$(CC) $(DEFINES) -shared -o $@ $(OBJS) $(LIBS) OleBridge.def

OleBridge.exe : $(OBJS)
	$(CC) $(DEFINES) -static-libgcc -static-libstdc++ -o $@ $(OBJS) $(LIBS)

%.o : %.cpp
	$(CC) $(DEFINES) -c -o $@ $<

install : OleBridge.exe
	#cp -f $< $(HOME)/.wine/drive_c/Tutu

clean:
	rm -rf *.o *.d *.exe *.dll

-include *.d
