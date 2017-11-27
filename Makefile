TARGET = main

SRC = src
BUILD = build
DST=k_odevzdani
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 
SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(patsubst %, $(BUILD)/%, $(notdir $(SOURCE:.c=.o)))

$(TARGET) : $(OBJECT)
		$(CC) -o $@ $^

$(BUILD)/%.o : $(SRC)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

run : $(BUILD)/$(TARGET)
	$(BUILD)/$(TARGET)

clean :
	rm -f $(OBJECT) $(TARGET)

move:
	cp -t ./$(DST) $(SOURCE) $(wildcard $(SRC)/*.h)