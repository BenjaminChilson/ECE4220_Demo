CFLAGS := -std=gnu99 -Wno-declaration-after-statement -g

obj-m+=ISR.o

SOURCES := demo.c \
	demo_functions.c

OBJS := $(SOURCES:%.c=%.o)

LDFLAGS := -L/usr/local/lib
LDLIBS := -lwiringPi -lpthread

demo : $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ $(LDLIBS) -o $@

ISR :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

.PHONY : clean
clean :
	-rm -rf demo
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
