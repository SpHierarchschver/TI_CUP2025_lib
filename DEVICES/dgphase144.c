#include "dgphase144.h"

static void set_dataA (uint8_t val);
static void set_dataB (uint8_t val);

void
dgphase_set_phase (float32_t phase)
{
  for (int i = 100; i < 256; ++i)
  {
    set_dataB (i);

    for (int j = 100; j < 256; ++j)
    {
      set_dataA (j);
      // HAL_Delay (300);
    }
  }
}

static void
set_dataA (uint8_t val)
{
  uchar i, command = 0x11;

  /* CSB - PE2
     CSA - PE4
     SCK - PE5
     SI  - PE6 */
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_Delay (1);

  for (i = 0; i < 8; ++i) 
	{ 
		if (command & 0x80)
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
		else
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay (1); 
		command <<= 1;
	}

  for (i = 0; i < 8; ++i) 
	{ 
		if (val & 0x80)
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
		else
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
		HAL_Delay (1);
		val <<= 1;
	}

	HAL_GPIO_WritePin (GPIOE, GPIO_PIN_4, GPIO_PIN_SET); 
	HAL_Delay (1);
}

static void
set_dataB (uint8_t val)
{
  uchar i, command = 0x11;

  /* CSB - PE2
     CSA - PE4
     SCK - PE5
     SI  - PE6 */
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin (GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
  HAL_Delay (1);

  for (i = 0; i < 8; ++i) 
	{ 
		if (command & 0x80)
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
		else
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay (1); 
		command <<= 1;
	}

  for (i = 0; i < 8; ++i) 
	{ 
		if (val & 0x80)
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
		else
      HAL_GPIO_WritePin (GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); 
		HAL_Delay (1);
		HAL_GPIO_WritePin (GPIOE, GPIO_PIN_5, GPIO_PIN_SET); 
		HAL_Delay (1);
		val <<= 1;
	}

	HAL_GPIO_WritePin (GPIOE, GPIO_PIN_2, GPIO_PIN_SET); 
	HAL_Delay (1);
}
