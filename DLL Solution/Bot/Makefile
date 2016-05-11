SRCDIR=SpelunkBot
INCDIR=SpelunkBot

BUILD=build
VENDOR=vendor

# ARCH=x86_64
ARCH=i686
CPP=$(ARCH)-w64-mingw32.static-g++
DLLTOOL=$(ARCH)-w64-mingw32.static-dlltool
FLAGS=-std=c++14 -DSPELUNKBOT_EXPORTS -static -DLINUX_CROSS_COMPILE_FLAG

OBJECTS= \
	$(BUILD)/BasicBotExampleOne.o \
	$(BUILD)/Bot.o \
	$(BUILD)/DiscoveryDan.o \
	$(BUILD)/FishBot.o \
	$(BUILD)/GoldDigger.o \
	$(BUILD)/IBot.o \
	$(BUILD)/Indie.o \
	$(BUILD)/JordanBot.o \
	$(BUILD)/NotSoSolidSnake.o \
	$(BUILD)/SeanBean.o \
	$(BUILD)/PlaygroundBot.o \
	$(BUILD)/stdafx.o

VENDOR_OBJECTS= \
	$(VENDOR)/NEAT/innovation.o

all: build_dir Bot.dll Bot.lib

clean:
	rm -- $(BUILD)/*
	rm -- Bot.dll
	rm -- Bot.lib

build_dir:
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(FLAGS) -I $(INCDIR) -shared -o $@ -c $<

Bot.dll: $(OBJECTS)
	$(CPP) $(FLAGS) -I $(INCDIR) \
		-shared $(VENDOR_OBJECTS) $(OBJECTS) \
		$(SRCDIR)/dllmain.cpp -o Bot.dll

Bot.lib: Bot.dll
	$(DLLTOOL) -l Bot.lib Bot.dll
