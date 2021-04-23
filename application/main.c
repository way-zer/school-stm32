/**
 * ***********************************************************
 * 
 * STM32F4xx LL library quickstart
 * 
 * FUNCTION: Blink, Pin: GPIOA1
 * CPU: STM32F407VGT6
 * 
 * CUBEMX PROJECT FOLDER: ./periph_config
 * 
 * ***********************************************************
*/

#include "main.h"

#define LED_Toggle() LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin)

/* peripheral init functions */
extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);

int main()
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* init clock */
    SystemClock_Config();

    /* init peripheral */
    MX_GPIO_Init();

    while (1)
    {
        LED_Toggle();
        HAL_Delay(100);
    }
}
