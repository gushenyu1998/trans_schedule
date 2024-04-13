# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET= wave_player
SOURCES= wave_player.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

# -pg for supporting gprof profiling.
#CFLAGS += -pg


all: player wav
	@echo
	@echo '******************************************'
	@echo '***             DONE                   ***'
	@echo '******************************************'
	@echo

install-asound:
	# Do this on the host
	sudo apt-get update
	sudo apt-get install libasound2-dev

	# Add cross-compile support for libasound
	sudo dpkg --add-architecture armhf
	sudo apt-get update
	sudo apt-get install libasound2-dev:armhf


player:
	@echo
	@echo '******************************************'
	@echo '*** If missing alsa/asoundlib.h, run   ***'
	@echo '***       make install-asound          ***'
	@echo '******************************************'
	@echo
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lasound

wav:
	# Copy wave files to the shared folder
	mkdir -p $(PUBDIR)/wave-files/
	cp wave-files/* $(PUBDIR)/wave-files/ 

clean:
	rm -f $(OUTDIR)/$(TARGET)
