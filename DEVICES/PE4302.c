#include "PE4302.h"

void
PE4302_damp (float32_t db)
{
  uint8_t dbWord = (uint8_t) (db * 2);
  
  if (dbWord & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);	// 0.5 dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  if ((dbWord >> 1) & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3, GPIO_PIN_SET);	// 1   dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  if ((dbWord >> 2) & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_2, GPIO_PIN_SET);	// 2   dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
  if ((dbWord >> 3) & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, GPIO_PIN_SET);	// 4   dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  if ((dbWord >> 4) & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_0, GPIO_PIN_SET);  // 8   dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  if ((dbWord >> 5) & 0x1)
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	// 16  dB
  else
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}
