#include <avr/io.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "timeout.h"
#include "net.h"
#include "usart.h"

#include "websrv_help_functions.h"
#include "avr/interrupt.h"
volatile int led_rgb_red=255;
volatile int led_rgb_green=255;
volatile int led_rgb_blue=255;
volatile int stepper_motor_pan=255;
volatile int stepper_motor_tilt=255;

static uint8_t mymac[6] = {0x00,0xE0,0x4C,0x00,0x02,0x30};
static uint8_t myip[4] = {192,168,0,20};

#define MYWWWPORT 80
#define BUFFER_SIZE 850
static uint8_t buf[BUFFER_SIZE+1];
uint8_t tryb=0;

// global string buffer
#define STR_BUFFER_SIZE 24
static char strbuf[STR_BUFFER_SIZE+1];

int8_t analyse_get_url(char *str)
{
        uint8_t port=0;
		uint8_t sport=0;
		uint8_t dport=0;
        if (find_key_val(str,strbuf,STR_BUFFER_SIZE,"colour")){
		  if (strbuf[0] < 0x47 && strbuf[0] > 0x2f){
                    if(strbuf[0] < 0x47 && strbuf[0] >0x40)
					sport=(strbuf[0]-55)*16;
					else sport=(strbuf[0]-0X30)*16;
                }
				
				
			 if (strbuf[1] < 0x47 && strbuf[1] > 0x2f){
					if(strbuf[1] < 0x47 && strbuf[1] >0x40)
					dport=(strbuf[1]-55);
					else dport=(strbuf[1]-0X30);
					led_rgb_red=dport+sport;
					//OCR0A=dport+sport;
					
				}
				
               
                if (strbuf[2] < 0x47 && strbuf[2] > 0x2f){
                 	if(strbuf[2] < 0x47 && strbuf[2] >0x40)
					sport=(strbuf[2]-55)*16;
					else sport=(strbuf[2]-0X30)*16;
				}
				
				
			 if (strbuf[3] < 0x47 && strbuf[3] > 0x2f){
                   	if(strbuf[3] < 0x47 && strbuf[3] >0x40)
					dport=(strbuf[3]-55);
					else dport=(strbuf[3]-0X30);
					led_rgb_green=dport+sport;
					//OCR0B=dport+sport;
				}
				
								
                if (strbuf[4] < 0x47 && strbuf[4] > 0x2f){
					if(strbuf[4] < 0x47 && strbuf[4] >0x40)
					sport=(strbuf[4]-55)*16;
					else sport=(strbuf[4]-0X30)*16;
                }
				
				
			 if (strbuf[5] < 0x47 && strbuf[5] > 0x2f){
                  	if(strbuf[5] < 0x47 && strbuf[5] >0x40)
					dport=(strbuf[5]-55);
					else dport=(strbuf[5]-0X30);
					led_rgb_blue=dport+sport;
					//OCR0B=dport+sport;
				}
				
		}
        if (find_key_val(str,strbuf,STR_BUFFER_SIZE,"sw")){
                if (strbuf[0] == 'c'){
                       tryb=0;
						}
						
				if (strbuf[0] == 'p'){
                        
						tryb=1;
						}
               if (strbuf[0] == 't'){
						tryb=2;
						}

			if (tryb==0){    //tryb 0 RGB
			
			if (strbuf[1] != 'd'){
                        return(0);
			}
				
                if (strbuf[2] < 0x47 && strbuf[2] > 0x2f){
                    if(strbuf[2] < 0x47 && strbuf[2] >0x40)
					sport=(strbuf[2]-55)*16;
					else sport=(strbuf[2]-0X30)*16;
                }
				
				
			 if (strbuf[3] < 0x47 && strbuf[3] > 0x2f){
					if(strbuf[3] < 0x47 && strbuf[3] >0x40)
					dport=(strbuf[3]-55);
					else dport=(strbuf[3]-0X30);
					led_rgb_red=dport+sport;
					//OCR0A=dport+sport;
					
				}
				
               
                if (strbuf[4] < 0x47 && strbuf[4] > 0x2f){
                 	if(strbuf[4] < 0x47 && strbuf[4] >0x40)
					sport=(strbuf[4]-55)*16;
					else sport=(strbuf[4]-0X30)*16;
				}
				
				
			 if (strbuf[5] < 0x47 && strbuf[5] > 0x2f){
                   	if(strbuf[5] < 0x47 && strbuf[5] >0x40)
					dport=(strbuf[5]-55);
					else dport=(strbuf[5]-0X30);
					led_rgb_green=dport+sport;
					//OCR0B=dport+sport;
				}
				
								
                if (strbuf[6] < 0x47 && strbuf[6] > 0x2f){
					if(strbuf[6] < 0x47 && strbuf[6] >0x40)
					sport=(strbuf[6]-55)*16;
					else sport=(strbuf[6]-0X30)*16;
                }
				
				
			 if (strbuf[7] < 0x47 && strbuf[7] > 0x2f){
                  	if(strbuf[7] < 0x47 && strbuf[7] >0x40)
					dport=(strbuf[7]-55);
					else dport=(strbuf[7]-0X30);
					led_rgb_blue=dport+sport;
					//OCR0B=dport+sport;
				}
				
			} //koniec if
			
		if (tryb==1){ if (strbuf[1] != 'd'){
                        return(0);
			}
				
                if (strbuf[2] < 0x3A && strbuf[2] > 0x2f){
					sport=(strbuf[2]-0x30)*100;
                }
				
				
			 if (strbuf[3] < 0x3A && strbuf[3] > 0x2f){
					
					dport=(strbuf[3]-0x30)*10;
					
				}
				
               
                if (strbuf[4] < 0x3A && strbuf[4] > 0x2f){
                 	port=(strbuf[4]-0x30)+dport+sport;
					stepper_motor_pan=port;
				}
		}
		
		
		if (tryb==2){ if (strbuf[1] != 'd'){
                        return(0);
			}
				
                if (strbuf[2] < 0x3A && strbuf[2] > 0x2f){
					sport=(strbuf[2]-0x30)*100;
                }
				
				
			 if (strbuf[3] < 0x3A && strbuf[3] > 0x2f){
					
					dport=(strbuf[3]-0x30)*10;
					
				}
				
               
                if (strbuf[4] < 0x3A && strbuf[4] > 0x2f){
                 	port=(strbuf[4]-0x30)+dport+sport;
					stepper_motor_tilt=port;
				}
		}
		
		

						
        }
		USART_Transmit(stepper_motor_pan);	
		USART_Transmit(stepper_motor_tilt);	
		USART_Transmit(led_rgb_red);		
		USART_Transmit(led_rgb_green);
		USART_Transmit(led_rgb_blue);
        
        return(1);
}




uint16_t http200ok(void)
{
        return(fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n")));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
        uint16_t plen;
        plen=http200ok();
		
		plen=fill_tcp_data_p(buf,plen,PSTR("<html> \n<head>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n <script type=\"text/javascript\" src=\"http://slaher.ugu.pl/avr/jquery.js\"></script>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n  <script type=\"text/javascript\" src=\"http://slaher.ugu.pl/avr/farbtastic.js\"></script> "));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n  <link rel=\"stylesheet\" href=\"http://slaher.ugu.pl/avr/farbtastic.css\" type=\"text/css\" />"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n <script type=\"text/javascript\" charset=\"utf-8\">"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n  $(document).ready(function() {  \n   $('#demo').hide();"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n $('#picker').farbtastic('#color'); \n });"));	
		plen=fill_tcp_data_p(buf,plen,PSTR("\n </script> \n </head> \n <body>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n <form action=\"\" style=\"width: 400px;\">"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n  <div class=\"form-item\"><label for=\"color\">Color: #</label>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("<input id=\"color\" name=\"colour\" value=\"123456\" />  </div>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n<div id=\"picker\"></div> \n </form>"));
		plen=fill_tcp_data_p(buf,plen,PSTR("\n </body> \n </html>"));			
			
        return(plen);
}


int main(void)
{
	DDRD|=(1<<PD6);
	DDRD|=(1<<PD5);
	TCCR0A = 0b10100001;
	TCCR0B = 0b00000011;
	OCR0A=100;
	OCR0B=127;
	USART_Init(3);	
	
	sei();
    uint16_t plen;
    uint16_t dat_p;
    int8_t cmd;
    // set the clock speed to "no pre-scaler" (8MHz with internal osc or 
    // full external speed)
    // set the clock prescaler. First write CLKPCE to enable setting of clock the
    // next four instructions.
    CLKPR=(1<<CLKPCE); // change enable
    CLKPR=0; // "no pre-scaler"
    _delay_loop_1(0); // 60us

        /* enable PD2/INT0, as input */
        DDRD&= ~(1<<DDD2);

        // test PD3=reset to default 
        DDRD&= ~(1<<PIND3);
        PORTD|=1<<PIND3; // internal pullup resistor on
        // test PD2=change IP
        DDRD&= ~(1<<PIND2);
        PORTD|=1<<PIND2; // internal pullup resistor on

        _delay_loop_1(0); 
        _delay_loop_1(0); 
  
        _delay_loop_1(0); // 60us
        if (eeprom_read_byte((uint8_t *)0x0) == 19){
                // ok magic number matches accept values
                eeprom_read_block((uint8_t *)myip,(void *)1,sizeof(myip));
        }
        /*initialize enc28j60*/
        enc28j60Init(mymac);
        enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
        _delay_loop_1(0); // 60us
        
        adc_ref_pin_init();

        // the transistor on PD7
        DDRD|= (1<<DDD7);
        PORTD &= ~(1<<PORTD7);// transistor off
        // the transistor on PD6
        DDRD|= (1<<DDD6);
        PORTD &= ~(1<<PORTD6);// transistor off
        // the transistor on PD5
        DDRD|= (1<<DDD5);
        PORTD &= ~(1<<PORTD5);// transistor off
        // the transistor on PD4
        DDRD|= (1<<DDD4);
        PORTD &= ~(1<<PORTD4);// transistor off
        // digital input:
        DDRD&= ~(1<<DDD0);
        // pullup on
        PORTD|= (1<<PIND0);
        // digital input:
        DDRD&= ~(1<<DDD1);
        // pullup on
        PORTD|= (1<<PIND1);
        
        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
        enc28j60PhyWrite(PHLCON,0x476);
        _delay_loop_1(0); // 60us
        

        //init the ethernet/ip layer:
        init_ip_arp_udp_tcp(mymac,myip,MYWWWPORT);

        while(1){
		
                // handle ping and wait for a tcp packet:
                plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
                dat_p=packetloop_icmp_tcp(buf,plen);

                /* dat_p will ne unequal to zero if there is a valid
                 * http get */
                if(dat_p==0){
                        continue;
                }
                if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
                        // head, post and other methods:
                        //
                        // for possible status codes see:
                        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
                        goto SENDTCP;
                }
                
                // analyse the url and do possible port changes:
                // move one char ahead:
                cmd=analyse_get_url((char *)&(buf[dat_p+5]));
                // for possible status codes see:
                // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                if (cmd==-1){
                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
                        goto SENDTCP;
                }
                if (cmd==0){
                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>"));
                        goto SENDTCP;
                }
                if (cmd==2){
                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 406 Not Acceptable\r\nContent-Type: text/html\r\n\r\n<h1>406 IP addr. format wrong</h1>"));
                        goto SENDTCP;
                }
				else{
                        plen=print_webpage(buf);
                }
               
                
                //
SENDTCP:
                www_server_reply(buf,plen); // send web page data
                // tcp port www end
                //
        }
        return (0);
}
