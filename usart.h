/*
 * usart.h
 *
 *  Created on: 27-jan-2009
 *      Author: Jan
 */

#ifndef USART_H_
#define USART_H_

void USART_Init(unsigned int baudrate)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baudrate>>8);
	UBRR0L = (unsigned char)baudrate;

	/* Enable receiver and interrupt*/
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);

	/* Set frame format: 9data UCSZ0 = 7, 8data UCSZ0 = 3, 1stop bit */
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(1<<USBS0);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) );
/* Put data into buffer, sends the data */
UDR0 = data;
}

unsigned char USART_Receive( void )
{
unsigned char status, resl;
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get status and 9th bit, then data */
/* from buffer */
status = UCSR0A;
resl = UDR0;
/* If error, return -1 */
if ( status & (1<<FE0))
return -1;
/* Filter the 9th bit, then return */
return (resl);
}

#endif /* USART_H_ */
