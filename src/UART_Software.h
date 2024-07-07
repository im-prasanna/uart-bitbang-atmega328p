static volatile char *timer_TCCR1A = 0x80, *timer_TCCR1B = 0x81, *timer_TIMSK1 = 0x6F, *timer_TIFR1 = 0x36;
static volatile uint16_t *timer_TCNT1 = 0x84, *timer_OCR1A = 0x88;
static volatile char *PORTD_DIR = 0x2A, *PORTD_DATA = 0x2B, *PORTD_IN = 0x29;

int n, writing=1;
uint16_t data;

void uart_init(){
  *PORTD_DIR |= (1<<1);

  *PORTD_DIR &= ~1;
  *PORTD_DATA |= ((1<<1)|(1));

  *timer_TCCR1A = 0x00;
  *timer_TCCR1B = 0x08;
  *timer_TIMSK1 = 0x02;
}
void uart_writeByte(uint8_t data_byte){
  data = 1<<9;
  data |= ((uint16_t)data_byte << 1);
  data &= ~1;
  writing=1;

  *timer_TCNT1 = 0;
  *timer_OCR1A = 1666;
  *timer_TCCR1B = 0x09;
  while(*timer_TCCR1B!=0x08);
}
void uart_write(char *s){
  for(int i=0;s[i]!=0;i++){
    uart_writeByte(s[i]);
  }
}

uint8_t uart_readByte(){
  while(*PORTD_IN & 0x01);
  writing = 0;
  n=0;
  data = 0;
  *timer_TCNT1 = 0;
 // *timer_OCR1A = 1666; //for baud=9600
  *timer_OCR1A = 138; //for baud=115200
  *timer_TCCR1B = 0x09;
  while(*timer_TCCR1B!=0x08);
  return (uint8_t)data;
}
ISR(TIMER1_COMPA_vect){
  if(writing){
    if(data&0x01) *PORTD_DATA |= 1<<1;
    else *PORTD_DATA &= ~(1<<1);
    data = data >> 1;
    if(data==0){
      *timer_TCCR1B=0x08;
    }
  }
  else{
    data |= (*PORTD_IN & 1)<<n;
    n++;
    if(n>=10)
      *timer_TCCR1B=0x08;
    
  }
}
