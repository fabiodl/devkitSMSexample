DEVKIT=../devkitSMS
IHX2SMS=../devkitSMS/ihx2sms/Linux/ihx2sms
CFLAGS= -I$(DEVKIT)/SMSlib/src -I$(DEVKIT)/PSGlib/src
PSGTOOL=../smstools/PSGTool

BUILD_DIR=./build

SOURCES=main.c
ASSETS=build/bank2.rel
OUTPUT=testrom.sms


all: $(OUTPUT)


$(BUILD_DIR):
	mkdir -p $@

assets/%.psg:assetsrc/%.vgm
	java -jar $(PSGTOOL)/PSGTool.jar $^ $@

#graphical assets are created using BMP2Tile. DLL loading fails in mono


OBJS=$(SOURCES:%.c=$(BUILD_DIR)/%.rel)

$(BUILD_DIR)/assets2banks.h: $(BUILD_DIR) assets/music.psg
	cd $(BUILD_DIR) && python3 ../$(DEVKIT)/assets2banks/src/assets2banks.py ../assets --singleheader


$(BUILD_DIR)/bank%.c: $(BUILD_DIR)/assets2banks.h
	@echo Requested $@

$(BUILD_DIR)/bank%.rel: $(BUILD_DIR)/bank%.c
	sdcc -o $@ -c -mz80 $(CFLAGS) $^

$(BUILD_DIR)/%.rel: %.c $(ASSETS)
	sdcc -o $@ -c -mz80 $(CFLAGS) $^

$(BUILD_DIR)/%.ihx: $(OBJS)  $(ASSETS)
	sdcc -o $@ -mz80 --data-loc 0xC000 --no-std-crt0 $(DEVKIT)/crt0/crt0_sms.rel  $(DEVKIT)/PSGlib/PSGlib.rel $(DEVKIT)/SMSlib/SMSlib.lib  $^

%.sms: $(BUILD_DIR)/%.ihx
	$(IHX2SMS) $^ $@

clean:
	rm -rf $(OUTPUT) $(BUILD_DIR)
