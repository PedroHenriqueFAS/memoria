#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 22

#define Led_Verde 11
#define Led_Vermelho 13
#define Led_Azul 12

#define MAX_SEQUENCE 9999

static uint32_t last_time = 0;  // Variável para controle do debounce

int sequence[MAX_SEQUENCE];
int sequence_length = 0;

// Função de debouncing
bool debounce(uint32_t *last_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > 200000) { // 200ms de debounce
        *last_time = current_time;
        return true;
    }
    return false;
}

// Espera até que um botão seja pressionado e retorna qual foi
void wait_for_button_press(int *pressed_button) {
    while (true) {
        if (!gpio_get(BUTTON_A) && debounce(&last_time)) {
            *pressed_button = BUTTON_A;
            break;
        }
        if (!gpio_get(BUTTON_B) && debounce(&last_time)) {
            *pressed_button = BUTTON_B;
            break;
        }
        if (!gpio_get(BUTTON_C) && debounce(&last_time)) {
            *pressed_button = BUTTON_C;
            break;
        }
    }

    // Aguarda o botão ser solto antes de continuar
    sleep_ms(50);
    while (!gpio_get(*pressed_button));
    sleep_ms(50);
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

    gpio_init(Led_Verde);
    gpio_set_dir(Led_Verde, GPIO_OUT);
    gpio_put(Led_Verde, 0);

    gpio_init(Led_Vermelho);
    gpio_set_dir(Led_Vermelho, GPIO_OUT);
    gpio_put(Led_Vermelho, 0);

    gpio_init(Led_Azul);
    gpio_set_dir(Led_Azul, GPIO_OUT);
    gpio_put(Led_Azul, 1);

    gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
    pwm_set_wrap(slice_num, 255);
    pwm_set_gpio_level(slice_num, 100);
    pwm_set_enabled(slice_num, true);
}

void set_led_brightness(uint8_t brightness) {
    uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, brightness);
}

void pisca_led_verde() {
    gpio_put(Led_Verde, 1);
    sleep_ms(500);
    gpio_put(Led_Verde, 0);
    sleep_ms(500);
}

void liga_led_azul(uint8_t brilho) {
    gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
    pwm_set_enabled(slice_num, true);
    set_led_brightness(brilho);
}

void desliga_led_azul() {
    pwm_set_enabled(pwm_gpio_to_slice_num(Led_Azul), false);
    gpio_set_function(Led_Azul, GPIO_FUNC_SIO);
    gpio_set_dir(Led_Azul, GPIO_OUT);
    gpio_put(Led_Azul, 0);
}

int main() {
    setup();
    printf("Jogo da Memória Iniciado!\n");

    set_led_brightness(20);

    while (1) {
        liga_led_azul(100);
        set_led_brightness(20);

        for (int i = 0; i < sequence_length; i++) {
            int pressed_button = -1;
            wait_for_button_press(&pressed_button);

            if (i == 0) { 
                desliga_led_azul();
            }

            printf("Esperado: %d, Pressionado: %d\n", sequence[i], pressed_button);

            if (pressed_button != sequence[i]) {
                printf("Sequência errada! Fim de jogo.\n");
                gpio_put(Led_Vermelho, 1);
                sleep_ms(1000);
                gpio_put(Led_Vermelho, 0);
                liga_led_azul(100);
                set_led_brightness(20);
                sequence_length = 0;
                break;
            } 

            // Verifica se vai piscar corretamente
            printf("Botão correto! Piscando LED verde...\n");
            pisca_led_verde(); 
            printf("LED verde piscou!\n");
        }

        int pressed_button = -1;
        wait_for_button_press(&pressed_button);
        sequence[sequence_length] = pressed_button;
        sequence_length++;
        printf("Sequência correta! Próxima rodada.\n");
    }
}
