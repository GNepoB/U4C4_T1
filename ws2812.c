#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define tempo 400
#define botao_A 5
#define botao_B 6
#define LED_red 13

static void gpio_irq_handler(uint gpio, uint32_t events);



volatile uint num = 0; //Inicializando a variavel num=0 
                      //(A matriz e LED inicializa exibindo o número zero)
uint32_t last_time_A = 0; // Armazenar o tempo da última interrupção do botão A
uint32_t last_time_B = 0; // Armazenar o tempo da última interrupção do botão B
#define TEMPO_DEBOUNCING 200 // Tempo de debounce em milissegundos

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 100; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

// Buffer para armazenar quais LEDs estão ligados matriz 5x5
bool num_0[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};

bool num_1[NUM_PIXELS] = {
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 1, 1, 0, 0, 
    0, 0, 1, 0, 0
};

bool num_2[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};

bool num_3[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};

bool num_4[NUM_PIXELS] = {
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0
};

bool num_5[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
};

bool num_6[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
};

bool num_7[NUM_PIXELS] = {
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};

bool num_8[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};

bool num_9[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};


static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_num_0(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_0[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_1(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_1[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_2(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_2[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_3(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_3[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_4(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_4[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_5(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_5[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_6(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_6[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_7(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_7[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_8(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_8[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void set_num_9(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (num_9[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

int main()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    //Configurando o LED vermelho
    gpio_init(LED_red);
    gpio_set_dir(LED_red, GPIO_OUT);
    //Configurando o botão A
    gpio_init(botao_A);
    gpio_set_dir(botao_A, GPIO_IN);
    gpio_pull_up(botao_A);
    //Configurando o botão B
    gpio_init(botao_B);
    gpio_set_dir(botao_B, GPIO_IN);
    gpio_pull_up(botao_B);

    gpio_set_irq_callback(&gpio_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);

    gpio_set_irq_enabled_with_callback(botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1)
    {
        //Switch case para apontar o número a ser exibido na matriz de LED
        switch (num)
        {
        case 0:
            set_num_0(led_r, led_g, led_b);
            break;
        case 1:
            set_num_1(led_r, led_g, led_b);
            break;
        case 2:
            set_num_2(led_r, led_g, led_b);
            break;
        case 3:
            set_num_3(led_r, led_g, led_b);
            break;
        case 4:
            set_num_4(led_r, led_g, led_b);
            break;
        case 5:
            set_num_5(led_r, led_g, led_b);
            break;
        case 6:
            set_num_6(led_r, led_g, led_b);
            break;
        case 7:
            set_num_7(led_r, led_g, led_b);
            break;
        case 8:
            set_num_8(led_r, led_g, led_b);
            break;
        case 9:
            set_num_9(led_r, led_g, led_b);
            break;
        default:
            break;
        }

        //Comando para que o LED vermelho pisque 5x por segundo
        gpio_put(LED_red, true);
        sleep_ms(200);
        gpio_put(LED_red, false);
        sleep_ms(200);

    }

    return 0;
}

void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Verificar se o tempo de debounce passou para o botão A
    if (gpio == botao_A && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_time_A > TEMPO_DEBOUNCING) {
            if (num < 9) {
                num++;
            }
            last_time_A = current_time; // Atualiza o tempo da última interrupção
        }
    }

    // Verificar se o tempo de debounce passou para o botão B
    if (gpio == botao_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (current_time - last_time_B > TEMPO_DEBOUNCING) {
            if (num > 0) {
                num--;
            }
            last_time_B = current_time; // Atualiza o tempo da última interrupção
        }
    }
}
