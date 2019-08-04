
#include "string.h"
//#include "stdbool.h"
#include "terminal.h"
#include "main.h"
#include "termgpio.h"

USBD_CDC_LineCodingTypeDef LineCoding = {
  115200,                       /* baud rate */
  0x00,                         /* stop bits-1 */
  0x00,                         /* parity - none */
  0x08                          /* nb. of bits 8 */
};

static TERM_tx_handler low_tx_handler;
static TERM_rx_handler low_rx_handler;

static volatile bool TERM_inited = false;

void TERM_init(TERM_tx_handler tx_handler, TERM_rx_handler rx_handler)
{
    low_tx_handler = tx_handler;
    low_rx_handler = rx_handler;
    TERM_inited = true;
}

void TERM_deinit()
{
    if(!TERM_inited)
        return;
}

void TERM_get_config(USBD_CDC_LineCodingTypeDef* cfg)
{
    if(!TERM_inited)
        return;
    if(cfg)
    {
        memcpy(cfg, &LineCoding,sizeof(LineCoding));
    }
}

void TERM_set_config(USBD_CDC_LineCodingTypeDef* cfg)
{
    if(!TERM_inited)
        return;
    if(cfg)
    {
        memcpy(&LineCoding, cfg,sizeof(LineCoding));
    } 
}

void TERM_debug_print(const char *line)
{
    if(!TERM_inited)
        return;
    low_tx_handler((uint8_t*)line, strlen(line));
}

void TERM_debug_print_int(const int value)
{
    char buffer[50];
    char *bp = buffer;
    if(!TERM_inited)
        return;

    bp += TERM_gpio_itona(value, bp, &buffer[50]-bp-2);
    bp += '\n';
    bp += 0;
    TERM_debug_print(buffer);
}

int TERM_get_input_buf(unsigned char* buf, int max_size)
{
    while(!TERM_inited);

    int rc;
    do
    {
        rc = low_rx_handler(buf, max_size);
    }
    while(rc == 0);
    return rc;
}
