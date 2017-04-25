#ifndef _LMIC_TTN_WRAPPER_H_
#define _LMIC_TTN_WRAPPER_H_

#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

typedef void (*event_cb_t)(void *data);

static event_cb_t event_callback;

void onEvent (ev_t ev);

class radio{
public:
    radio(uint8_t pin_rfm96_nss, uint8_t pin_rfm96_io0, uint8_t pin_rfm96_io1);
    void init();

    void send_data(uint8_t mydata[], int size);
    void set_event_cb(event_cb_t callback);

    int _pin_rfm96_nss =LMIC_UNUSED_PIN;
    int _pin_rfm96_io0 =LMIC_UNUSED_PIN;
    int _pin_rfm96_io1 =LMIC_UNUSED_PIN;
};

#endif
