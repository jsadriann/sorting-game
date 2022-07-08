CHAIN=$(CHAINPATH)arm-none-eabi
CFLAGS=-mcpu=cortex-a8 -mfpu=vfp -mfloat-abi=softfp
AFLAGS=-mfpu=neon -mcpu=cortex-a8
IPATH=-Iinc/
SRC=src/
OBJ=obj/
BIN=bin/
TFTPBOOT=/tftpboot/

all: app

app: start.o main.o gpio.o interruption.o timers.o memory.o uart.o menus.o pad.o control_module.o clock_module.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)main.o $(OBJ)gpio.o $(OBJ)memory.o $(OBJ)interruption.o $(OBJ)timers.o $(OBJ)uart.o $(OBJ)menus.o $(OBJ)pad.o $(OBJ)control_module.o $(OBJ)clock_module.o -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot $(TFTPBOOT)appTimer.bin

start.o: $(SRC)start.s
	$(CHAIN)-as $(AFLAGS) $(SRC)start.s -o $(OBJ)start.o

main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)main.c -o $(OBJ)main.o
                                        
gpio.o: $(SRC)gpio.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)gpio.c -o $(OBJ)gpio.o

interruption.o: $(SRC)interruption.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)interruption.c -o $(OBJ)interruption.o

timers.o: $(SRC)timers.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)timers.c -o $(OBJ)timers.o

memory.o: $(SRC)memory.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)memory.c -o $(OBJ)memory.o

uart.o: $(SRC)uart.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)uart.c -o $(OBJ)uart.o
 
menus.o: $(SRC)menus.c                          
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)menus.c -o $(OBJ)menus.o

pad.o: $(SRC)pad.c                            
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)pad.c -o $(OBJ)pad.o

control_module.o: $(SRC)control_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)control_module.c -o $(OBJ)control_module.o

clock_module.o: $(SRC)clock_module.c
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $(SRC)clock_module.c -o $(OBJ)clock_module.o

copy:
	cp $(BIN)spl.boot $(TFTPBOOT)appTimer.bin

link:
	ln -f $(TFTPBOOT)appGpio.bin $(TFTPBOOT)download.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot
	rm $(TFTPBOOT)download.bin
	rm $(TFTPBOOT)appTimer.bin

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
