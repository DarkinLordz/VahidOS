#include "shell/shell.h"

void cmd_help(char *args) {
    print_string("help\nclear\nhalt\nreboot\ncpuid\ncolor\npeek\ndump\ncursor\nrandom\npoke\n");
}

void cmd_clear(char *args) {
    clear();
}

void cmd_halt(char *args) {
    while(1) {
        __asm__ volatile("cli; hlt");
    }
}

void cmd_reboot(char *args) {
    uint8_t good = 0x02;
    while(good & 0x02) {
        good = inb(0x64);
    }
    outb(0x64, 0xFE);
}

void cmd_cpuid(char *args) {
    static uint32_t regs[4];

    __asm__ volatile("cpuid"
        : "=a"(regs[3]), "=b"(regs[0]), "=d"(regs[1]), "=c"(regs[2])
        : "a"(0));

    char *vendor = (char *)regs;
    vendor[12] = '\0';

    print_string(vendor);
}

void cmd_color(char *args) {
    if (!args || !*args) return;

    char *fg_str = args; // 0xF0 0xA0
    char *bg_str = NULL;

    for (int i = 0; args[i] != '\0'; i++) {
        if (args[i] == ' ') {
            args[i] = '\0';
            bg_str = &args[i+1];
            break;
        }
    }

    if (bg_str == NULL || *bg_str == '\0') {
        return;
    }

    uint8_t fg = (uint8_t)string_to_hex(fg_str);
    uint8_t bg = (uint8_t)string_to_hex(bg_str);

    set_color(fg, bg);
}

void cmd_peek(char *args) {
    if (!args || !*args) return;
    
    uint32_t addr = string_to_hex(args);
    uint8_t *ptr = (uint8_t *)addr;
    uint8_t value = *ptr;
    print_hex(value);
}

void cmd_poke(char *args) {
    if (!args || !*args) return;

    char *addr_str = args;
    char *val_str = NULL;

    for (int i = 0; args[i] != '\0'; i++) {
        if (args[i] == ' ') {
            args[i] = '\0';
            val_str = &args[i+1];
            break;
        }
    }

    if (val_str == NULL || *val_str == '\0') {
        return;
    }

    uint32_t address = string_to_hex(addr_str);
    uint8_t value = (uint8_t)string_to_hex(val_str);

    uint8_t *ptr = (uint8_t *)address;
    *ptr = value;
}

void cmd_dump(char *args) {
    if (!args || !*args) return;

    uint32_t addr = string_to_hex(args);
    uint8_t *ptr = (uint8_t *)addr;

    for(int i = 0; i < 16; i++){
        print_hex_byte(ptr[i]);
        print_character(' ');
        if(i == 7) print_string("| ");
    }
}

void cmd_cursor(char *args) {
    if (!args || !*args) return;

    uint8_t cursor_value = string_to_hex(args);
    change_cursor(cursor_value);
}

void cmd_random(char *args) {
    uint32_t random_number = random();
    char random_number_string[64];
    itoa(random_number, random_number_string);
    print_string(random_number_string);
}

typedef void (*command_handler_t)(char *args);

typedef struct {
    const char *name;
    command_handler_t handler;
} shell_command_t;

shell_command_t commands[] = {
    {"help",   cmd_help},
    {"clear",  cmd_clear},
    {"halt",   cmd_halt},
    {"reboot", cmd_reboot},
    {"cpuid",  cmd_cpuid},
    {"color",  cmd_color},
    {"peek",   cmd_peek},
    {"dump",   cmd_dump},
    {"cursor", cmd_cursor},
    {"random", cmd_random},
    {"poke",   cmd_poke}
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(shell_command_t))

void execute_command(char *input) {
    if (*input == '\0') return;

    char *args = input;

    while (*args != ' ' && *args != '\0') {
        args++;
    }
    
    if (*args == ' ') {
        *args = '\0'; 
        args++;
    }

    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }

    print_string("Unknown command: ");
    print_string(input);
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