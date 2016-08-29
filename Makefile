src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = $(libgl_$(shell uname -s))

libgl_Linux = -lGL -lglut -lm -lGLU 
libgl_Darwin = -framework OpenGL -framework GLUT

flight: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o flight
