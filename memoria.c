#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 22

#define MAX_SEQUENCE 9999

int sequence[MAX_SEQUENCE];
int sequence_length = 0;
int current_index = 0;

void wait_for_button_press(int *pressed_button) {
    while (true) {
        if (!gpio_get(BUTTON_A)) {
            *pressed_button = BUTTON_A;
            break;
        }
        if (!gpio_get(BUTTON_B)) {
            *pressed_button = BUTTON_B;
            break;
        }
        if (!gpio_get(BUTTON_C)) {
            *pressed_button = BUTTON_C;
            break;
        }
    }
    sleep_ms(300); // Debounce
    while (!gpio_get(*pressed_button)); // Espera soltar o botão
    sleep_ms(300);
}

void setup() {
    stdio_init_all();
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(BUTTON_C);
    gpio_set_dir(BUTTON_C, GPIO_IN);
    gpio_pull_up(BUTTON_C);
}

int main() {
    setup();
    printf("Jogo da Memória Iniciado!\n");
    
    while (1) {
        int pressed_button;
        wait_for_button_press(&pressed_button);
        
        if (current_index < sequence_length) {
            if (pressed_button != sequence[current_index]) {
                printf("Sequência errada! Fim de jogo.\n");
                sequence_length = 0;
                current_index = 0;
                continue;
            }
            current_index++;
        } else {
            sequence[sequence_length] = pressed_button;
            sequence_length++;
            current_index = 0;
            printf("Sequência correta! Próxima rodada.\n");
        }
    }
}
