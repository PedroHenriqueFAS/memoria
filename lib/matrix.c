// #include <stdint.h>
// #include "matriz_led.h"

// Matriz_leds_config limpa = {
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
// };

// Matriz_leds_config zero =
// {
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}}
// };

// Matriz_leds_config um =
// {
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
//     {{0.0, 0.0, 0.0}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.02, 0.02, 0.02}, {0.0, 0.0, 0.0}}
// };


// Matriz_leds_config *numeros[] = {&zero, &um, &limpa};