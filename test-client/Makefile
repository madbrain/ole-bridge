
SRCS=main.cpp iid.cpp my-storage.cpp my-client-site.cpp my-stream.cpp

OBJS=$(SRCS:.cpp=.o)

#PREFIX=i586-mingw32msvc-
CC=$(PREFIX)g++
DEFINES=-Wall -MMD
LIBS=-luuid -lole32

TestClient.exe : $(OBJS)
	$(CC) $(DEFINES) -static-libgcc -static-libstdc++ -o $@ $(OBJS) $(LIBS)

%.o : %.cpp
	$(CC) $(DEFINES) -c -o $@ $<

clean:
	rm -rf *.o *.d *.exe *.dll

-include *.d
