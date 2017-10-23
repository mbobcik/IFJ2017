TARGET = main

SRC = src
BUILD = build
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 
SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(patsubst %, $(BUILD)/%, $(notdir $(SOURCE:.c=.o)))

$(BUILD)/$(TARGET) : $(OBJECT)
		$(CC) -o $@ $^

$(BUILD)/%.o : $(SRC)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

run : $(BUILD)/$(TARGET)
	$(BUILD)/$(TARGET)

clean :
	rm -f $(OBJECT) $(BUILD)/$(TARGET)