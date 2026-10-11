// EXECÍCIOS:
// 1) Configurar os oito pinos de GPIO como saídas utilizando a API do ESP-IDF.
// 2) Desenvolver uma função unsigned char codifica_display(int numero); que converta
// um número hexadecimal (0x0 a 0xF) no padrão de bits correspondente ao display
// de 7-segmentos. Lembre-se que o segmento acende quando o valor é definido
// como nível lógico baixo.
//  Dica: utilize a sintaxe de switch do C.
// 3) Alterar o programa da Listagem 1 para que os 8 primeiros pinos GPIO sejam
// inicializados como saídas e mantenham os níveis lógicos em ALTO, evitando assim
// que os segmentos acendam na inicialização.
// 4) Alterar o laço infinito do programa para que sejam exibidos os valores
// correspondentes no display de 7-segmentos, de forma que apareçam
// sequencialmente os valores de 0x0 a 0xF.
// 5) Alterar a função criada no Exercício 2 para que receba uma segunda variável,
// correspondente ao ponto do display. A variável deve ser definida como 1 quando
// existe a intenção de acender o ponto, e 0 quando o ponto deve permanecer
// apagado.
// 6) Alterar o programa para que o ponto acenda em números ímpares e apague em
// números pares.
//  Dica: utilize a sintaxe operador de módulo (%) em C.

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

unsigned char codifica_Display(int numero);
void mostra_Display(unsigned char codificado, int contagem);

int hexa_num [16] = {
  0x0,
  0x1,
  0x2,
  0x3,
  0x4,
  0x5,
  0x6,
  0x7,
  0x8,
  0x9,
  0xA,
  0xB,
  0xC,
  0xD,
  0xE,
  0xF
};

int segmentos [8] = {
  GPIO_NUM_0 ,
  GPIO_NUM_1 ,
  GPIO_NUM_2 ,
  GPIO_NUM_3 ,
  GPIO_NUM_4 ,
  GPIO_NUM_5 ,
  GPIO_NUM_6 ,
  GPIO_NUM_7 
};

void app_main(void){

  gpio_config_t io_config_pinos = {
    .pin_bit_mask = (1ULL << GPIO_NUM_0) |
                    (1ULL << GPIO_NUM_1) |
                    (1ULL << GPIO_NUM_2) |
                    (1ULL << GPIO_NUM_3) |
                    (1ULL << GPIO_NUM_4) |
                    (1ULL << GPIO_NUM_5) |
                    (1ULL << GPIO_NUM_6) |
                    (1ULL << GPIO_NUM_7) ,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
  };

  gpio_config(&io_config_pinos);

  for(int i=0;i<8;i++) {
    gpio_set_level(i,1);
  }

  while (1) {
    int contagem = 0;
    for (int i=0 ; i<=15 ; i++) {
      unsigned char codificado = codifica_Display(hexa_num[i]);
      mostra_Display(codificado, contagem);
      contagem ++;
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }
}

unsigned char codifica_Display(int numero) {

  switch(numero) {
    case 0x0: return 0b11000000;
    case 0x1: return 0b11111001;
    case 0x2: return 0b10100100;
    case 0x3: return 0b10110000;
    case 0x4: return 0b10011001;
    case 0x5: return 0b10010010;
    case 0x6: return 0b10000010;
    case 0x7: return 0b11111000;
    case 0x8: return 0b10000000;
    case 0x9: return 0b10010000;
    case 0xA: return 0b10001000;
    case 0xB: return 0b10000011;
    case 0xC: return 0b11000110;
    case 0xD: return 0b10100001;
    case 0xE: return 0b10000110;
    case 0xF: return 0b10001110;
    default : return 0b11111111;
  }
}

void mostra_Display(unsigned char codificado, int contagem) {

  for (int i=0 ; i<8 ; i++) {
    gpio_set_level(segmentos[i] , (codificado >> i) & 1);
  }
  if (contagem % 2 == 1) {
    gpio_set_level(segmentos[7] , 0);
  }
}
