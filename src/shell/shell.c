#include "shell/shell.h"

const char *cpuid(void){
    static uint32_t regs[4];

    __asm__ volatile("cpuid"
        : "=a"(regs[3]), "=b"(regs[0]), "=d"(regs[1]), "=c"(regs[2])
        : "a"(0));

    char *vendor = (char *)regs;
    vendor[12] = '\0';

    return vendor;
}

void execute_command(char *command){
    if(strcmp(command, "help") == 0){
        print_string("help\nclear\necho\nhalt\nreboot\ncpuid\ncolor\npeek\ndump\ntetost\ncursor\nrandom\nguess\nfetch\ncowsay\nloop");
    } else if(strcmp(command, "clear") == 0){
        clear();
    } else if(strncmp(command, "echo", 4) == 0){
        if(strlen(command) > 4){
            print_string(command + 5);
        }
    } else if(strcmp(command, "halt") == 0){
        while(1){
            __asm__ volatile("cli; hlt");
        }
    } else if(strcmp(command, "reboot") == 0){
        uint8_t good = 0x02;
        while(good & 0x02)
            good = inb(0x64);
        outb(0x64, 0xFE);
    } else if(strcmp(command, "cpuid") == 0){
        print_string(cpuid());
    } else if(strncmp(command, "color", 5) == 0){
        if(strlen(command) > 5){
            char *color = command + 6;

            if(strcmp(color, "black") == 0){set_color(0x00, 0x00);}
            if(strcmp(color, "blue") == 0){set_color(0x01, 0x00);}
            if(strcmp(color, "green") == 0){set_color(0x02, 0x00);}
            if(strcmp(color, "cyan") == 0){set_color(0x03, 0x00);}
            if(strcmp(color, "red") == 0){set_color(0x04, 0x00);}
            if(strcmp(color, "magenta") == 0){set_color(0x05, 0x00);}
            if(strcmp(color, "brown") == 0){set_color(0x06, 0x00);}
            if(strcmp(color, "white") == 0){set_color(0x0F, 0x00);}
        }
    } else if(strncmp(command, "peek ", 5) == 0){
        uint32_t addr = string_to_hex(command + 5);
        uint8_t *ptr = (uint8_t *)addr;
        uint8_t value = *ptr;
        print_hex(value);
    } else if(strncmp(command, "dump ", 5) == 0){
        uint32_t addr = string_to_hex(command + 5);
        uint8_t *ptr = (uint8_t *)addr;

        for(int i = 0; i < 16; i++){
            print_hex_byte(ptr[i]);
            print_character(' ');
            if(i == 7) print_string("| ");
        }
    } else if(strcmp(command, "tetost") == 0){
        uint8_t initial_color = get_color();
        set_color(0x05, 0x00);
        print_string("Tetost is a femboy!");
        set_color(initial_color, 0x00);
    } else if(strncmp(command, "cursor ", 7) == 0){
        uint8_t cursor_value = string_to_hex(command + 7);
        change_cursor(cursor_value);
    } else if(strcmp(command, "random") == 0){
        uint32_t random_number = random();
        char random_number_string[64];
        itoa(random_number, random_number_string);
        print_string(random_number_string);
    } else if(strncmp(command, "guess", 5) == 0){
        char *guess = command + 6;
        uint32_t random_number = random();
        random_number = random_number % 11;

        char random_number_string[8];
        itoa(random_number, random_number_string);

        if(strcmp(guess, random_number_string) == 0){
            print_string("You were right!");
        } else {
            print_string("You were wrong!\nIt was ");
            print_string(random_number_string);
        }
    } else if(strcmp(command, "fetch") == 0){
        uint8_t initial_color = get_color();
        uint32_t cycles = get_seed_from_rdtsc();

        char cycles_string[64];
        itoa(cycles, cycles_string);

        set_color(0x01, 0x00);
        print_string("##@@           @@## Kernel: Vahix\n###@@         @@### Shell: Vahix shell\n@@@@##       ##@@@@ CPU: ");
        print_string(cpuid());
        print_string("\n @@@###     ###@@@  Color: ");

        const char *color_name;

        switch(initial_color){
            case 0x00: color_name = "black"; break;
            case 0x01: color_name = "blue"; break;
            case 0x02: color_name = "green"; break;
            case 0x03: color_name = "cyan"; break;
            case 0x04: color_name = "red"; break;
            case 0x05: color_name = "magenta"; break;
            case 0x06: color_name = "brown"; break;
            case 0x0F: color_name = "white"; break;
            default: color_name = "unknown"; break;
        }

        print_string(color_name);
        print_string("\n  ###@@     @@###   Cycles: ");
        print_string(cycles_string);
        print_string("\n   ##@@@   @@@##\n    ##@@@@@@@##\n      @@@@@@@");

        set_color(initial_color, 0x00);
    } else if(strncmp(command, "cowsay", 6) == 0){
        char *message = command + 7;

        print_string("< ");
        print_string(message);
        print_string(" >\n -------\n        \\   ^__^\n         \\  (oo)\\_______\n            (__)\\       )\\/\\\n                ||----w |\n                ||     ||\n");
    } else if(strcmp(command, "loop") == 0){
        while(true) {
            print_string("loop");
        }
    } else {
        print_string(command);
        print_string(": command not found");
    }
}

void shell(void){
    bool running = true;
    char input_buffer[64];
    int index = 0;
    char c;

    print_string("Welcome to Vahix Shell!\n");
    print_string("To see available commands, type 'help'\n");
    print_string("# ");

    while(running){
        if(keyboard_poll_char(&c)){
            if(c == '\n'){
                print_character('\n');
                input_buffer[index] = '\0';

                execute_command(input_buffer);

                index = 0;
                print_string("\n# ");
                continue;
            } else if(c == '\b'){
                if(index > 0){
                    index--;
                    input_buffer[index] = '\0';
                    print_character('\b');
                }
            } else {
                if(index < 63){
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
