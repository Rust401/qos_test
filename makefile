TARGET=qos_interface.c main.c auth_test.c qos_test.c concurrency_test.c test_attr.c
LEVEL=-O1
COMPILER_ARM64=aarch64-linux-gnu-gcc
COMPILER=clang-12
CPP_STD=-std=c11
OUT_NAME=qos_test
FLAGS=--static

all: $(TARGET)
	$(COMPILER) $(CPP_STD) -o $(OUT_NAME) $(TARGET) $(LEVEL) -pthread

arm64: $(TARGET)
	$(COMPILER_ARM64) $(CPP_STD) -o $(OUT_NAME) $(FLAGS) $(TARGET) $(LEVEL) -pthread

clean:
	rm -f peda* .gdb* *.i *.ll *.s *.o
