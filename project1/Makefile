OBJS=project.o robot.o pose.o firfilter.o kalman.o rovioKalmanFilter.o utilities.o logger.o PID.o
CFLAGS=-ggdb -g3
LIB_FLAGS=-L. -lrobot_if
CPP_LIB_FLAGS=$(LIB_FLAGS) -lrobot_if++
LIB_LINK=-lhighgui -lcv -lcxcore -lm -lgslcblas -L/usr/lib64/atlas -lclapack
LIB_LINK_NEW=-lopencv_core -lopencv_imgproc -lopencv_highgui -lm -lgslcblas -L/usr/lib64/atlas -llapack

all: $(OBJS)
	g++ $(CFLAGS) -o project.out $(OBJS) $(CPP_LIB_FLAGS) $(LIB_LINK)

new: $(OBJS) 
	g++ $(CFLAGS) -o project.out $(OBJS) $(CPP_LIB_FLAGS) $(LIB_LINK_NEW)

project.o: project.cpp
	g++ $(CFLAGS) -c project.cpp

robot.o: robot.cpp robot.h
	g++ $(CFLAGS) -c robot.cpp

pose.o: pose.cpp pose.h
	g++ $(CFLAGS) -c pose.cpp

firfilter.o: firfilter.cpp firfilter.h
	g++ $(CFLAGS) -c firfilter.cpp

kalman.o: kalman.cpp kalman.h
	g++ $(CFLAGS) -c kalman.cpp

rovioKalmanFilter.o: kalmanfilter/rovioKalmanFilter.c
	g++ ${CFLAGS} -Ikalmanfilter -c kalmanfilter/rovioKalmanFilter.c	

utilities.o: utilities.cpp utilities.h
	g++ $(CFLAGS) -c utilities.cpp

PID.o: PID.cpp PID.h
	g++ $(CFLAGS) -c PID.cpp

logger.o: logger.cpp logger.h
	g++ $(CFLAGS) -c logger.cpp

clean:
	rm -f *.o
	rm -f *.gch
	rm -f project.out
