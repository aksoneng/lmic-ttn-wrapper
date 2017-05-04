#include <lmic-ttn-wrapper.h>

radio::radio(unsigned char pin_rfm96_nss, unsigned char pin_rfm96_io0, unsigned char pin_rfm96_io1){
    _pin_rfm96_nss =pin_rfm96_nss;
    _pin_rfm96_io0 =pin_rfm96_io0;
    _pin_rfm96_io1 =pin_rfm96_io1;
}

void radio::init(){
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100); //Fix for OTAA within seconds instead of minutes: https://www.thethingsnetwork.org/forum/t/over-the-air-activation-otaa-with-lmic/1921/37

    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    // Use DR_SF12 for maximal range, DR_SF7 for maximal bandwidth
    LMIC_setDrTxpow(DR_SF7,14);
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
        Serial.println(F("EV_SCAN_TIMEOUT"));
        break;
        case EV_BEACON_FOUND:
        Serial.println(F("EV_BEACON_FOUND"));
        break;
        case EV_BEACON_MISSED:
        Serial.println(F("EV_BEACON_MISSED"));
        break;
        case EV_BEACON_TRACKED:
        Serial.println(F("EV_BEACON_TRACKED"));
        break;
        case EV_JOINING:
        Serial.println(F("EV_JOINING"));
        break;
        case EV_JOINED:
        Serial.println(F("EV_JOINED"));
        break;
        case EV_RFU1:
        Serial.println(F("EV_RFU1"));
        break;
        case EV_JOIN_FAILED:
        Serial.println(F("EV_JOIN_FAILED"));
        break;
        case EV_REJOIN_FAILED:
        Serial.println(F("EV_REJOIN_FAILED"));
        break;
        case EV_TXCOMPLETE:
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
        if (LMIC.dataLen) {
            Serial.println(F("Received "));
            Serial.println(LMIC.dataLen);
            Serial.println(F(" bytes of payload"));
        }
        break;
        case EV_LOST_TSYNC:
        Serial.println(F("EV_LOST_TSYNC"));
        break;
        case EV_RESET:
        Serial.println(F("EV_RESET"));
        break;
        case EV_RXCOMPLETE:
        Serial.println(F("EV_RXCOMPLETE"));
        break;
        case EV_LINK_DEAD:
        Serial.println(F("EV_LINK_DEAD"));
        break;
        case EV_LINK_ALIVE:
        Serial.println(F("EV_LINK_ALIVE"));
        break;
        default:
        Serial.println(F("Unknown event"));
        break;
    }
    event_callback((void *)ev);
}

void radio::send_data(unsigned char mydata[], int size){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, size, 0);
        Serial.println(F("Packet queued"));
    }
}

void radio::set_event_cb(event_cb_t callback)
{
    event_callback =callback;
}
