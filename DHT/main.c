#include "stdint.h"
#include <board.h>
#include <periph/gpio.h>
#include "periph_conf.h"
#include "xtimer.h"
#include "timex.h"
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"

dht_params_t params[] = {{.pin = UNWD_GPIO_7}};

int main(void)
{
    dht_t dev;
    int16_t temp, hum;
    char temp_s[10];
    char hum_s[10];

    /* initialize sensor */
    printf("Try to initialize DHT sensor:\n\r");
    if (dht_init(&dev, &params[0]) == DHT_OK) {
        puts("[OK]\n\r");
    }
    else {
        puts("[Failed]\n\r");
        return 1;
    }
    
    while(true) 
    {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Values unavialable\n\r");
            continue;
        }

        size_t n = fmt_s16_dfp(temp_s, temp, -1);
        temp_s[n] = '\0';
        n = fmt_s16_dfp(hum_s, hum, -1);
        hum_s[n] = '\0';

        printf("DHT - temp: %s°C - humidity: %s%%\n\r",
                temp_s, hum_s);

        xtimer_usleep(10e6);
    }

    return 0;
}
