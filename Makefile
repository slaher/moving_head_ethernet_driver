# makefile, written by guido socher
#MCU=atmega168
#DUDECPUTYPE=m168
MCU=atmega88
DUDECPUTYPE=m88
#MCU=atmega328p
#DUDECPUTYPE=m328
#
LOADCMD=avrdude
LOADARG=-p $(DUDECPUTYPE) -c stk500v2 -e -U flash:w:
#
#
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
#-------------------
.PHONY: all
#
all: eth_rem_dev_tcp.hex 
	@echo "done"
#
eth_rem_dev_tcp: eth_rem_dev_tcp.hex
	@echo "done"
#
size: eth_rem_dev_tcp.out
	avr-size *.out
#-------------------
help: 
	@echo "Usage: make all|load|eth_rem_dev_tcp|rdfuses|size"
	@echo "or"
	@echo "Usage: make clean"
	@echo " "
	@echo "For new hardware with clock from enc38j60 (all new boards): make fuse"
#-------------------
eth_rem_dev_tcp.hex: eth_rem_dev_tcp.out 
	$(OBJCOPY) -R .eeprom -O ihex eth_rem_dev_tcp.out eth_rem_dev_tcp.hex 
	avr-size eth_rem_dev_tcp.out
	@echo " "
	@echo "Expl.: data=initialized data, bss=uninitialized data, text=code"
	@echo " "

eth_rem_dev_tcp.out: main.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o analog.o
	$(CC) $(CFLAGS) -o eth_rem_dev_tcp.out -Wl,-Map,eth_rem_dev_tcp.map main.o ip_arp_udp_tcp.o enc28j60.o websrv_help_functions.o analog.o
websrv_help_functions.o: websrv_help_functions.c websrv_help_functions.h ip_config.h 
	$(CC) $(CFLAGS) -Os -c websrv_help_functions.c
enc28j60.o: enc28j60.c timeout.h enc28j60.h
	$(CC) $(CFLAGS) -Os -c enc28j60.c
analog.o: analog.c analog.h 
	$(CC) $(CFLAGS) -Os -c analog.c
ip_arp_udp_tcp.o: ip_arp_udp_tcp.c net.h enc28j60.h ip_config.h
	$(CC) $(CFLAGS) -Os -c ip_arp_udp_tcp.c
main.o: main.c ip_arp_udp_tcp.h enc28j60.h usart.h timeout.h net.h websrv_help_functions.h ip_config.h analog.h
	$(CC) $(CFLAGS) -Os -c main.c
#------------------
pre: eth_rem_dev_tcp.hex
	cp eth_rem_dev_tcp.hex eth_rem_dev_tcp_pre168.hex
#
load: eth_rem_dev_tcp.hex
	$(LOADCMD) $(LOADARG)eth_rem_dev_tcp.hex
#
#-------------------
# Check this with make rdfuses
rdfuses:
	$(LOADCMD) -p $(DUDECPUTYPE) -c stk500v2 -v -q
#
fuse:
	@echo "warning: run this command only if you have an external clock on xtal1"
	@echo "The is the last chance to stop. Press crtl-C to abort"
	@sleep 2
	$(LOADCMD) -p  $(DUDECPUTYPE) -c stk500v2 -u -v -U lfuse:w:0x60:m
#-------------------
clean:
	rm -f *.o *.map *.out eth_rem_dev_tcp.hex
#-------------------
