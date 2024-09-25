/*
* Arquitetura e Aplicação de sistemas - exercicio 03 - Alarme por interrupção
* Aluno: Victor Hugo de Toledo Nunes
* Prof.: Gustavo Ferreira Palma 
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_PIN 17
#define HANDLER_PIN 2
#define DEBOUNCE_TIME_MS 200  // Tempo de debounce de 200 ms

absolute_time_t last_interrupt_time;

bool is_debounce_time_elapsed() { // Função de debouncing
    absolute_time_t current_time = get_absolute_time();
    int64_t diff = absolute_time_diff_us(last_interrupt_time, current_time) / 1000;
    if (diff > DEBOUNCE_TIME_MS) {
        last_interrupt_time = current_time;
        return true;
    }
    return false;
}

void alarme_led(int time, int pulse){
    for (int i = 0; i < pulse; i++) {
        gpio_put(LED_PIN, 1);
        sleep_ms(time);
        gpio_put(LED_PIN, 0);
        sleep_ms(time);
    }
}

void gpio_callback(uint gpio, uint32_t events) { // Função de callback para a interrupção
    if (is_debounce_time_elapsed()) {
        printf("ALARME DETECTED !\n");
        alarme_led(10,50);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("Hello GPIO IRQ\n");

    last_interrupt_time = get_absolute_time();  // Inicializa o tempo da última interrupção

    // Configuração do pino de interrupção
    gpio_init(HANDLER_PIN);
    gpio_set_dir(HANDLER_PIN, GPIO_IN);
    gpio_set_pulls(HANDLER_PIN, true, false); // Configura como entrada com pull-up
    gpio_set_irq_enabled_with_callback(HANDLER_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Configuração do pino do LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        printf("Hello World\n");
        sleep_ms(1000);
    }
}