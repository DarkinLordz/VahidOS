#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "keyboard.h"
#include "vga.h"
#include "io.h"

void execute_command(char *command){
    if (strcmp(command, "help") == 0) {
        print_string("Commands: help, clear\n");
    } else if (strcmp(command, "clear") == 0){
        clear();
    } else if (strcmp(command, "exit") == 0){
        outw(0x604, 0x2000);
        outw(0x4004, 0x3400);
        outw(0xB004, 0x2000);
        print_string("It is now safe to turn off your computer.");
    
        __asm__ volatile ("cli");
        while(1) {
            __asm__ volatile ("hlt");
        }
    }
}

void shell(void){
    int running = 1; // I use 0 and 1 for false and true
    char input_buffer[64];
    int index = 0;
    char c;
    print_string("$ ");
    while (running){
        if (keyboard_poll_char(&c)){
            if (c == '\n'){
                print_character('\n');
                input_buffer[index] = '\0';

                execute_command(input_buffer);

                index = 0;
                print_string("$ ");

                continue;
            } else if (c == '\b'){
                if (index > 0){
                    index--;
                    input_buffer[index] = '\0';
                    print_character('\b');
                }
            } else {
                if (index < 63) { 
                    print_character(c);
                    input_buffer[index] = c;
                    index++;
                    input_buffer[index] = '\0';
                }
            }
        }
        __asm__ volatile("pause");
    }
}
