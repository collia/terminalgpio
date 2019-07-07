
#ifndef __TERMINAL_H
#define __TERMINAL_H

#include "usbd_cdc.h"

typedef void (*TERM_tx_handler)(const uint8_t* buff, uint32_t len);

void TERM_init(TERM_tx_handler handler);
void TERM_deinit(void);
void TERM_get_config(USBD_CDC_LineCodingTypeDef* cfg);
void TERM_set_config(USBD_CDC_LineCodingTypeDef* cfg);
void TERM_debug_print(const char *line);

#endif
