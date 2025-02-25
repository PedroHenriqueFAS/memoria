#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lib/ssd1306_i2c.h"
#include "interruptions_counter.pio.h"

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 22

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SDL 15
#define ENDERECO 0x3C

#define LED_VERDE 11
#define LED_VERMELHO 13
#define LED_AZUL 12

#define NUM_PIXELS 25
#define LED_MATRIX_PIN 7

#define IS_RGBW false

#define MAX_SEQUENCE 9999
ssd1306_t ssd;

static uint32_t last_time = 0;  // Para debouncing

int sequence[MAX_SEQUENCE];
int sequence_length = 0;

uint8_t led_r = 0;   // Intensidade de vermelho
uint8_t led_g = 0;   // Intensidade de verde
uint8_t led_b = 5;  // Intensidade de azul

// Padrão para o dígito 1 na matriz de LEDs
bool led_number_O[NUM_PIXELS] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

// Novo padrão (exemplo: dígito 2 ou outro desenho)
// Ajuste os valores conforme o desenho desejado.
bool led_number_X[NUM_PIXELS] = {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
};

bool led_number_P[NUM_PIXELS] = {
    0, 0, 0, 1, 0,
    0, 1, 1, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 1, 0, 0,
    0, 0, 0, 1, 0,
};

bool turn_off_all_leds[NUM_PIXELS] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

//
// Constrói um valor de 32 bits a partir de componentes RGB (ordem GRB)
//
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 8) | ((uint32_t)g << 16) | (uint32_t)(b);
}

//
// Envia um pixel para o PIO (deslocando 8 bits à esquerda)
//
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

//
// Função que "desenha" um padrão na matriz de LEDs
//
void print_digit(int digit, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = urgb_u32(r, g, b);
    bool *pattern;
    // Seleciona o padrão com base no parâmetro 'digit'
    switch(digit) {
        case 1:
            pattern = led_number_O;
            break;
        case 2:
            pattern = led_number_X;
            break;
        case 3:
            pattern = led_number_P;
            break;
        default:
            pattern = turn_off_all_leds;
            break;
    }
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (pattern[i])
            put_pixel(color);  // Liga o LED com a cor definida
        else
            put_pixel(0);      // Desliga o LED
    }
}

//
// Função de debouncing
//
bool debounce(uint32_t *last_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > 200000) { // 200ms de debounce
        *last_time = current_time;
        return true;
    }
    return false;
}

//
// Aguarda até que um botão seja pressionado e retorna qual foi
//
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
    sleep_ms(50);
    while (!gpio_get(*pressed_button));
    sleep_ms(50);
}

//
// Setup: Inicializa I2C, display, botões e LEDs
//
void setup() {
    stdio_init_all();
    
    // Botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(BUTTON_C);
    gpio_set_dir(BUTTON_C, GPIO_IN);
    gpio_pull_up(BUTTON_C);

    // LEDs de indicação
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_put(LED_AZUL, 1);

    gpio_set_function(LED_AZUL, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_AZUL);
    pwm_set_wrap(slice_num, 255);
    pwm_set_gpio_level(slice_num, 100);
    pwm_set_enabled(slice_num, true);
    
    // I2C e display SSD1306
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SDL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SDL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

//
// Funções auxiliares para controle de LEDs (brilho, piscar, ligar/desligar azul)
//
void set_led_brightness(uint8_t brightness) {
    uint slice_num = pwm_gpio_to_slice_num(LED_AZUL);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, brightness);
}

void pisca_led_verde() {
    gpio_put(LED_VERDE, 1);
    sleep_ms(500);
    gpio_put(LED_VERDE, 0);
    sleep_ms(500);
}

void liga_led_azul(uint8_t brilho) {
    gpio_set_function(LED_AZUL, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_AZUL);
    pwm_set_enabled(slice_num, true);
    set_led_brightness(brilho);
}

void desliga_led_azul() {
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_AZUL), false);
    gpio_set_function(LED_AZUL, GPIO_FUNC_SIO);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_put(LED_AZUL, 0);
}

//
// main(): Inicializa o sistema, configura a matriz via PIO e imprime o padrão desejado
//
int main() {
    stdio_init_all();
    setup();
    
    // Configura a matriz de LEDs via PIO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &interruptions_counter_program);
    interruptions_counter_program_init(pio, sm, offset, LED_MATRIX_PIN, 800000, IS_RGBW);
    
    // O restante do código (display, botões, etc.) segue normalmente
    int aux = 0;
    bool cor = true;
     
    set_led_brightness(20);
    
    ssd1306_fill(&ssd, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
    ssd1306_draw_string(&ssd, "Inicio de jogo", 9, 25);
    ssd1306_send_data(&ssd);
    
    print_digit(3, led_r, led_g, led_b);

    while (1) {
        liga_led_azul(100);
        set_led_brightness(20);
    
        if (aux == 1) {
            cor = !cor;
            ssd1306_fill(&ssd, !cor);
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
            ssd1306_draw_string(&ssd, "Botao Correto", 13, 25);
            ssd1306_send_data(&ssd);
        }
        aux = 1;
    
        for (int i = 0; i < sequence_length; i++) {
            int pressed_button = -1;
            wait_for_button_press(&pressed_button);
    
            if (i == 0) { 
                desliga_led_azul();
            }
    
            if (pressed_button != sequence[i]) {
                cor = !cor;
                ssd1306_fill(&ssd, !cor);
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
                ssd1306_draw_string(&ssd, "Botao Errado", 8, 10);
                ssd1306_draw_string(&ssd, "Fim de jogo", 20, 30);
                ssd1306_draw_string(&ssd, "Tente Novamente", 5, 48);
                ssd1306_send_data(&ssd);
                gpio_put(LED_VERMELHO, 1);
                print_digit(2, 5, led_g, 0);
                sleep_ms(1000);
                gpio_put(LED_VERMELHO, 0);
                print_digit(4, led_r, led_g, led_b);
                liga_led_azul(100);
                set_led_brightness(20);
                sequence_length = 0;
                break;
            } 
            pisca_led_verde(); 
        }
    
        int pressed_button = -1;
        wait_for_button_press(&pressed_button);
        sequence[sequence_length] = pressed_button;
        sequence_length++;
        cor = !cor;
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        ssd1306_draw_string(&ssd, "Proxima rodada", 8, 10);
        ssd1306_send_data(&ssd);
        print_digit(1, led_r, 5, 0);
        sleep_ms(2000);
        print_digit(4, led_r, led_g, led_b);
    }
    return 0;
}

