#include "pico/stdlib.h"
#define BROCHE_GPIO 0 //on définit la gpio 0 en sortie
#define BOUTON22 22 //on définit la gpio22 en entrée

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

// Perform initialisation
int pico_led_init(void)
{
#if defined(BROCHE_GPIO)
    //initialise le bouton gpio 22 du maker cytron
    gpio_init(BOUTON22);
    gpio_set_dir(BOUTON22, GPIO_IN);
    gpio_pull_up(BOUTON22);// on le met en mode pull up car bonton non activé = 3.3V
    // A device like Pico that uses a GPIO for the LED will define BROCHE_GPIO
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(BROCHE_GPIO);
    gpio_set_dir(BROCHE_GPIO, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on)
{
#if defined(BROCHE_GPIO)
    // Just set the GPIO on or off
    gpio_put(BROCHE_GPIO, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main()
{
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    while (true)
    {
        if (!gpio_get(BOUTON22))//si le bouton est appuyé soit à 0V en entée clignote rapidement
        {
            pico_set_led(true);
            sleep_ms(100);
            pico_set_led(false);
            sleep_ms(100);
        }
        else
        { // si le bouton est relaché clignote doucement
            pico_set_led(true);
            sleep_ms(LED_DELAY_MS);
            pico_set_led(false);
            sleep_ms(LED_DELAY_MS);
        }
    }
}
