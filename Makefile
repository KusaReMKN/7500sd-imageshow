CL=	wcl
CLFLAGS= -bt=dos -mc
#WATCOM= /path/to/watcom
INCLUDES= -i=$(WATCOM)/h

main.exe: main.c
	$(CL) $(CLFLAGS) $(INCLUDES) main.c

clean: .SYMBOLIC
	rm -f main.exe main.o
