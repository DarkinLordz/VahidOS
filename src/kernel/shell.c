#include <kernel/shell.h>

void execute_command(char *command){
    if (strcmp(command, "help") == 0) {
        print_string("Commands: help, clear, echo, halt, reboot, cpuid, whoami\n");
    } else if (strcmp(command, "clear") == 0){
        clear();
    } else if (strncmp(command, "echo", 4) == 0){
        if (strlen(command) > 4){
            print_string(command + 5);
            print_character('\n');
        }
    } else if (strcmp(command, "halt") == 0){
        while (1){
            __asm__ volatile("cli; hlt");
        }
    } else if (strcmp(command, "reboot") == 0) {
        uint8_t good = 0x02;
        while (good & 0x02)
            good = inb(0x64);
        outb(0x64, 0xFE);
    } else if (strcmp(command, "cpuid") == 0) {
        uint32_t regs[4];
        __asm__ volatile("cpuid" : "=a"(regs[3]), "=b"(regs[0]), "=d"(regs[1]), "=c"(regs[2]) : "a"(0));

        char *vendor = (char *)regs;

        vendor[12] = '\0';

        print_string(vendor);
    } else if (strcmp(command, "whoami") == 0) {
        print_string("root");
    }
}

void shell(void){
    int running = 1; // I use 0 and 1 for false and true
    char input_buffer[64];
    int index = 0;
    char c;
    print_string("\n# ");
    while (running){
        if (keyboard_poll_char(&c)){
            if (c == '\n'){
                print_character('\n');
                input_buffer[index] = '\0';

                execute_command(input_buffer);

                index = 0;
                print_string("\n# ");

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
