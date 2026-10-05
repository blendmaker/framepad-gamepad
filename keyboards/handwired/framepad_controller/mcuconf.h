#pragma once

#include_next <mcuconf.h>

/* Enable the RP2040 ADC driver (used for joystick axes) */
#undef RP_ADC_USE_ADC1
#define RP_ADC_USE_ADC1 TRUE
