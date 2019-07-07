
#include "terminal.h"
#include "main.h"

USBD_CDC_LineCodingTypeDef LineCoding = {
  115200,                       /* baud rate */
  0x00,                         /* stop bits-1 */
  0x00,                         /* parity - none */
  0x08                          /* nb. of bits 8 */
};

static TERM_tx_handler low_tx_handler;


void TERM_init(TERM_tx_handler handler)
{
    low_tx_handler = handler;
}

void TERM_deinit()
{
}

void TERM_get_config(USBD_CDC_LineCodingTypeDef* cfg)
{
    if(cfg)
    {
        memcpy(cfg, &LineCoding,sizeof(LineCoding));
    }
}

void TERM_set_config(USBD_CDC_LineCodingTypeDef* cfg)
{
    if(cfg)
    {
        memcpy(&LineCoding, cfg,sizeof(LineCoding));
    } 
}
