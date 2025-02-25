// #include <stdio.h>
// #include <string.h>
// #include "pico/stdlib.h"
// #include "hardware/gpio.h"
// #include "hardware/pwm.h"
// #include "hardware/i2c.h"
// #include "lib/ssd1306_i2c.h"
// #include <stdint.h>

// #define BUTTON_A 5
// #define BUTTON_B 6
// #define BUTTON_C 22


// #define I2C_SDA 14
// #define I2C_SDL 15
// #define endereco 0x3C

// #define Led_Verde 11
// #define Led_Vermelho 13
// #define Led_Azul 12

// #define MAX_SEQUENCE 9999

// static uint32_t last_time = 0;  // Variável para controle do debounce

// int sequence[MAX_SEQUENCE];
// int sequence_length = 0;

// // Buffer para manipulação do display
// uint8_t ssd[ssd1306_buffer_length];

// // Estrutura para definir a área de renderização no display
// struct render_area frame_area = {
//     start_column : 0,
//     end_column : ssd1306_width - 1,
//     start_page : 0,
//     end_page : ssd1306_n_pages - 1
// };

// // Declaração de funções externas do display
// extern void calculate_render_area_buffer_length(struct render_area *area);
// extern void ssd1306_send_command(uint8_t cmd);
// extern void ssd1306_send_command_list(uint8_t *ssd, int number);
// extern void ssd1306_send_buffer(uint8_t ssd[], int buffer_length);
// extern void ssd1306_init();
// extern void ssd1306_scroll(bool set);
// extern void render_on_display(uint8_t *ssd, struct render_area *area);
// extern void ssd1306_set_pixel(uint8_t *ssd, int x, int y, bool set);
// extern void ssd1306_draw_line(uint8_t *ssd, int x_0, int y_0, int x_1, int y_1, bool set);
// extern void ssd1306_draw_char(uint8_t *ssd, int16_t x, int16_t y, uint8_t character);
// extern void ssd1306_draw_string(uint8_t *ssd, int16_t x, int16_t y, char *string);
// extern void ssd1306_command(ssd1306_t *ssd, uint8_t command);
// extern void ssd1306_config(ssd1306_t *ssd);
// extern void ssd1306_init_bm(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
// extern void ssd1306_send_data(ssd1306_t *ssd);
// extern void ssd1306_draw_bitmap(ssd1306_t *ssd, const uint8_t *bitmap);

// // Função de debouncing
// bool debounce(uint32_t *last_time) {
//     uint32_t current_time = to_us_since_boot(get_absolute_time());
//     if (current_time - *last_time > 200000) { // 200ms de debounce
//         *last_time = current_time;
//         return true;
//     }
//     return false;
// }

// // Espera até que um botão seja pressionado e retorna qual foi
// void wait_for_button_press(int *pressed_button) {
//     while (true) {
//         if (!gpio_get(BUTTON_A) && debounce(&last_time)) {
//             *pressed_button = BUTTON_A;
//             break;
//         }
//         if (!gpio_get(BUTTON_B) && debounce(&last_time)) {
//             *pressed_button = BUTTON_B;
//             break;
//         }
//         if (!gpio_get(BUTTON_C) && debounce(&last_time)) {
//             *pressed_button = BUTTON_C;
//             break;
//         }
//     }

//     // Aguarda o botão ser solto antes de continuar
//     sleep_ms(50);
//     while (!gpio_get(*pressed_button));
//     sleep_ms(50);
// }

// void setup() {
//     stdio_init_all();
    
//     gpio_init(BUTTON_A);
//     gpio_set_dir(BUTTON_A, GPIO_IN);
//     gpio_pull_up(BUTTON_A);

//     gpio_init(BUTTON_B);
//     gpio_set_dir(BUTTON_B, GPIO_IN);
//     gpio_pull_up(BUTTON_B);

//     gpio_init(BUTTON_C);
//     gpio_set_dir(BUTTON_C, GPIO_IN);
//     gpio_pull_up(BUTTON_C);

//     gpio_init(Led_Verde);
//     gpio_set_dir(Led_Verde, GPIO_OUT);
//     gpio_put(Led_Verde, 0);

//     gpio_init(Led_Vermelho);
//     gpio_set_dir(Led_Vermelho, GPIO_OUT);
//     gpio_put(Led_Vermelho, 0);

//     gpio_init(Led_Azul);
//     gpio_set_dir(Led_Azul, GPIO_OUT);
//     gpio_put(Led_Azul, 1);

//     gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_wrap(slice_num, 255);
//     pwm_set_gpio_level(slice_num, 100);
//     pwm_set_enabled(slice_num, true);
    
//     stdio_init_all();
    
//     // Inicialização do display I2C
//     i2c_init(i2c1, ssd1306_i2c_clock * 1000);
//     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
//     gpio_set_function(I2C_SDL, GPIO_FUNC_I2C);
//     gpio_pull_up(I2C_SDA);
//     gpio_pull_up(I2C_SDL);

//     ssd1306_init();

//     // Configuração da área de renderização
//     calculate_render_area_buffer_length(&frame_area);

//     // Zera o buffer do display
//     memset(ssd, 0, ssd1306_buffer_length);
//     render_on_display(ssd, &frame_area);
// }

// void set_led_brightness(uint8_t brightness) {
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, brightness);
// }

// void pisca_led_verde() {
//     gpio_put(Led_Verde, 1);
//     sleep_ms(500);
//     gpio_put(Led_Verde, 0);
//     sleep_ms(500);
// }

// void liga_led_azul(uint8_t brilho) {
//     gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_enabled(slice_num, true);
//     set_led_brightness(brilho);
// }

// void desliga_led_azul() {
//     pwm_set_enabled(pwm_gpio_to_slice_num(Led_Azul), false);
//     gpio_set_function(Led_Azul, GPIO_FUNC_SIO);
//     gpio_set_dir(Led_Azul, GPIO_OUT);
//     gpio_put(Led_Azul, 0);
// }

// void render_centered_text(char *text[]) {
//     int y = 0;
//     for (uint8_t i = 0; i < count_of(text); i++) {
//         int text_width = strlen(text[i]) * 8; // Cada caractere tem 8 pixels de largura
//         int x_position = (ssd1306_width - text_width) / 2; // Calcula a posição central
//         ssd1306_draw_string(ssd, x_position, y, text[i]); // Renderiza o texto centralizado
//         y += 8; // Avança para a próxima linha
//     }
//     render_on_display(ssd, &frame_area); // Atualiza o display
// }

// int main() {
//     char *text[5] ={ "", "", "", "", "" };
//     stdio_init_all();
//     setup();
    
//     // display_text("Jogo da Memória Iniciado\n");
//     text[0] = "Jogo da Memoria";
//     text[1] = "  Iniciado";

//     // Limpa o buffer do display (apaga o conteúdo anterior)
//     memset(ssd, 0, ssd1306_buffer_length);

//     // Atualiza o display para refletir a tela limpa
//     render_on_display(ssd, &frame_area);

//     // Renderiza o texto no display
//     int y = 0;
//     for(uint8_t i = 0; i < count_of(text); i++) {
//         ssd1306_draw_string(ssd, 5, y, text[i]);
//         y += 8;
//     }
//     render_on_display(ssd, &frame_area);
     
//     set_led_brightness(20);

//     while (1) {
//         liga_led_azul(100);
//         set_led_brightness(20);

//         for (int i = 0; i < sequence_length; i++) {
//             int pressed_button = -1;
//             wait_for_button_press(&pressed_button);

//             if (i == 0) { 
//                 desliga_led_azul();
//             }

//             if (pressed_button != sequence[i]) {

//                 text[0] = " Botao Errado";
//                 text[1] = " Fim de Jogo";
//                 gpio_put(Led_Vermelho, 1);
//                 sleep_ms(1000);
//                 gpio_put(Led_Vermelho, 0);
//                 liga_led_azul(100);
//                 set_led_brightness(20);
//                 sequence_length = 0;

//                 // Limpa o buffer do display (apaga o conteúdo anterior)
//                 memset(ssd, 0, ssd1306_buffer_length);
//                 // Atualiza o display para refletir a tela limpa
//                 render_on_display(ssd, &frame_area);

//                 // Renderiza o texto no display
//                 int y = 0;
//                 for(uint8_t i = 0; i < count_of(text); i++) {
//                     ssd1306_draw_string(ssd, 5, y, text[i]);
//                     y += 8;
//                 }
//                 render_on_display(ssd, &frame_area);
//                 break;
//             } 

//             pisca_led_verde(); 

//         }

//         int pressed_button = -1;
//         wait_for_button_press(&pressed_button);
//         sequence[sequence_length] = pressed_button;
//         sequence_length++;
//         text[0] = " Botao Correto"; 
//         text[1] = "Proxima Rodada";
//          // Limpa o buffer do display (apaga o conteúdo anterior)
//          memset(ssd, 0, ssd1306_buffer_length);
//          // Atualiza o display para refletir a tela limpa
//          render_on_display(ssd, &frame_area);

//          // Renderiza o texto no display
//          int y = 0;
//          for(uint8_t i = 0; i < count_of(text); i++) {
//              ssd1306_draw_string(ssd, 5, y, text[i]);
//              y += 8;
//          }
//          render_on_display(ssd, &frame_area);
//     }
// }

// #include <stdio.h>
// #include <string.h>
// #include <stdint.h>
// #include "pico/stdlib.h"
// #include "hardware/gpio.h"
// #include "hardware/pwm.h"
// #include "hardware/i2c.h"
// #include "lib/ssd1306_i2c.h"
// #include "interruptions_counter.pio.h"

// #define BUTTON_A 5
// #define BUTTON_B 6
// #define BUTTON_C 22

// #define I2C_PORT i2c1
// #define I2C_SDA 14
// #define I2C_SDL 15
// #define endereco 0x3C

// #define Led_Verde 11
// #define Led_Vermelho 13
// #define Led_Azul 12

// #define NUM_PIXELS 25
// #define LED_MATRIX_PIN 7

// #define MAX_SEQUENCE 9999
// ssd1306_t ssd;

// static uint32_t last_time = 0;  // Variável para controle do debounce

// int sequence[MAX_SEQUENCE];
// int sequence_length = 0;

// bool led_number_0[NUM_PIXELS] = {
//     0, 1, 1, 1, 0,
//     1, 1, 0, 0, 1,
//     1, 0, 1, 0, 1,
//     1, 0, 0, 1, 1,
//     0, 1, 1, 1, 0,
// };

// bool led_number_1[NUM_PIXELS] = {
//     0, 0, 1, 0, 0, 
//     0, 0, 1, 0, 0, 
//     0, 0, 1, 0, 0, 
//     0, 1, 1, 0, 0, 
//     0, 0, 1, 0, 0
// };

// bool turn_off_all_leds[NUM_PIXELS] = {
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0,
// };

// // Função de debouncing
// bool debounce(uint32_t *last_time) {
//     uint32_t current_time = to_us_since_boot(get_absolute_time());
//     if (current_time - *last_time > 200000) { // 200ms de debounce
//         *last_time = current_time;
//         return true;
//     }
//     return false;
// }

// // Espera até que um botão seja pressionado e retorna qual foi
// void wait_for_button_press(int *pressed_button) {
//     while (true) {
//         if (!gpio_get(BUTTON_A) && debounce(&last_time)) {
//             *pressed_button = BUTTON_A;
//             break;
//         }
//         if (!gpio_get(BUTTON_B) && debounce(&last_time)) {
//             *pressed_button = BUTTON_B;
//             break;
//         }
//         if (!gpio_get(BUTTON_C) && debounce(&last_time)) {
//             *pressed_button = BUTTON_C;
//             break;
//         }
//     }

//     // Aguarda o botão ser solto antes de continuar
//     sleep_ms(50);
//     while (!gpio_get(*pressed_button));
//     sleep_ms(50);
// }

// void setup() {
//     stdio_init_all();
    
//     gpio_init(BUTTON_A);
//     gpio_set_dir(BUTTON_A, GPIO_IN);
//     gpio_pull_up(BUTTON_A);

//     gpio_init(BUTTON_B);
//     gpio_set_dir(BUTTON_B, GPIO_IN);
//     gpio_pull_up(BUTTON_B);

//     gpio_init(BUTTON_C);
//     gpio_set_dir(BUTTON_C, GPIO_IN);
//     gpio_pull_up(BUTTON_C);

//     gpio_init(Led_Verde);
//     gpio_set_dir(Led_Verde, GPIO_OUT);
//     gpio_put(Led_Verde, 0);

//     gpio_init(Led_Vermelho);
//     gpio_set_dir(Led_Vermelho, GPIO_OUT);
//     gpio_put(Led_Vermelho, 0);

//     gpio_init(Led_Azul);
//     gpio_set_dir(Led_Azul, GPIO_OUT);
//     gpio_put(Led_Azul, 1);

//     gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_wrap(slice_num, 255);
//     pwm_set_gpio_level(slice_num, 100);
//     pwm_set_enabled(slice_num, true);

//     // I2C Initialisation. Using it at 400Khz.
//     i2c_init(I2C_PORT, 400 * 1000);

//     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
//     gpio_set_function(I2C_SDL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
//     gpio_pull_up(I2C_SDA); // Pull up the data line
//     gpio_pull_up(I2C_SDL); // Pull up the clock line
//     ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
//     ssd1306_config(&ssd); // Configura o display
//     ssd1306_send_data(&ssd); // Envia os dados para o display

//     // Limpa o display. O display inicia com todos os pixels apagados.
//     ssd1306_fill(&ssd, false);
//     ssd1306_send_data(&ssd);
    
// }

// void set_led_brightness(uint8_t brightness) {
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_chan_level(slice_num, PWM_CHAN_A, brightness);
// }

// void pisca_led_verde() {
//     gpio_put(Led_Verde, 1);
//     sleep_ms(500);
//     gpio_put(Led_Verde, 0);
//     sleep_ms(500);
// }

// void liga_led_azul(uint8_t brilho) {
//     gpio_set_function(Led_Azul, GPIO_FUNC_PWM);
//     uint slice_num = pwm_gpio_to_slice_num(Led_Azul);
//     pwm_set_enabled(slice_num, true);
//     set_led_brightness(brilho);
// }

// void desliga_led_azul() {
//     pwm_set_enabled(pwm_gpio_to_slice_num(Led_Azul), false);
//     gpio_set_function(Led_Azul, GPIO_FUNC_SIO);
//     gpio_set_dir(Led_Azul, GPIO_OUT);
//     gpio_put(Led_Azul, 0);
// }

// bool* draw_number(int number)
// {
//     switch (number)
//     {
//     case 0:
//         return led_number_0; // Retorna o padrão de LEDs para o número 0.
//         break;
//     case 1:
//         return led_number_1; // Retorna o padrão de LEDs para o número 1.
//         break;
//     default:
//         return turn_off_all_leds; // Retorna um padrão que desliga todos os LEDs se o número for inválido.
//         break;
//     }
// }

// static inline void put_pixel(uint32_t pixel_grb)
// {
//     // Envia o valor do pixel para a máquina de estado do PIO, deslocando 8 bits para a esquerda.
//     pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
// }


// int main() {
//     stdio_init_all();
//     setup();

//     PIO pio = pio0;
//     int sm = 0;

//     int aux = 0;
//     bool cor = true;
     
//     set_led_brightness(20);

//     ssd1306_fill(&ssd, !cor); // Limpa o display
//     ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
//     ssd1306_draw_string(&ssd, "Inicio de jogo", 9, 25); // Desenha uma string
//     ssd1306_send_data(&ssd); // Atualiza o display

//     while (1) {
//         liga_led_azul(100);
//         set_led_brightness(20);

//         if (aux == 1)
//         {
//             cor = !cor;
//             // Atualiza o conteúdo do display com animações
//             ssd1306_fill(&ssd, !cor); // Limpa o display
//             ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
//             ssd1306_draw_string(&ssd, "Botao Correto", 13, 25); // Desenha uma string
//             ssd1306_send_data(&ssd); // Atualiza o display
//         }
//         aux = 1;

//         for (int i = 0; i < sequence_length; i++) {
//             int pressed_button = -1;
//             wait_for_button_press(&pressed_button);

//             if (i == 0) { 
//                 desliga_led_azul();
//             }

//             if (pressed_button != sequence[i]) {
//             cor = !cor;
//             // Atualiza o conteúdo do display com animações
//             ssd1306_fill(&ssd, !cor); // Limpa o display
//             ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
//             ssd1306_draw_string(&ssd, "Botao Errado", 8, 10); // Desenha uma string
//             ssd1306_draw_string(&ssd, "Fim de jogo", 20, 30); // Desenha uma string
//             ssd1306_draw_string(&ssd, "Tente Novamente", 5, 48); // Desenha uma string
//             ssd1306_send_data(&ssd); // Atualiza o display
//             gpio_put(Led_Vermelho, 1);
//             sleep_ms(1000);
//             gpio_put(Led_Vermelho, 0);
//             liga_led_azul(100);
//             set_led_brightness(20);
//             sequence_length = 0;
//             break;
//             } 

//             pisca_led_verde(); 

//         }

//         int pressed_button = -1;
//         wait_for_button_press(&pressed_button);
//         sequence[sequence_length] = pressed_button;
//         sequence_length++;
//         cor = !cor;
//         // Atualiza o conteúdo do display com animações
//         ssd1306_fill(&ssd, !cor); // Limpa o display
//         ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
//         ssd1306_draw_string(&ssd, "Proxima rodada", 8, 10); // Desenha uma string
//         ssd1306_send_data(&ssd); // Atualiza o display
//         sleep_ms(2000);

//     }
//     return 0;
// }




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
