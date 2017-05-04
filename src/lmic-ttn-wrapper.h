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
    radio(unsigned char pin_rfm96_nss, unsigned char pin_rfm96_io0, unsigned char pin_rfm96_io1);
    void init();

    void send_data(unsigned char mydata[], int size);
    void set_event_cb(event_cb_t callback);

    unsigned char _pin_rfm96_nss =LMIC_UNUSED_PIN;
    unsigned char _pin_rfm96_io0 =LMIC_UNUSED_PIN;
    unsigned char _pin_rfm96_io1 =LMIC_UNUSED_PIN;
};

#endif
