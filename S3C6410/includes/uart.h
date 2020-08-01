#ifndef __UART_H__
#define __UART_H__
#ifdef __cplusplus
extern "C" {
#endif

void Uart_init(void);
void Uart_putc(const char c);
void Uart_SendString(char *s);
void Uart_SendHex(unsigned int dat);
void Uart_Printf(const char *fmt,...);

#endif /*__UART_H__*/