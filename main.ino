void setup(void)
{
  SPI_MasterInit();
  //Wait 20ms for Power-up 
  delayMicroseconds(20000);
}

void main(void)
{}



void SPI_MasterInit(void)
{
  /* Set MOSI and SCK output, DDB1-->Reset, all others input */
  DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<DDB1);
  PORTB |= 1<<PINB1;
  asm volatile("nop");
  PORTB &= ~(1<<PINB1);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
}

void sendInstruction(unsigned long instruction)
{
  while(instruction!=0){
    SPI_MasterTransmit((instruction&0xFF000000)>>24);
    instruction = instruction<<8;
  }
}


void ProgrammingEnable(void)
{
  sendInstruction(0xAC530000);
}

void ChipErase(void)
{
  sendInstruction(0xAC800000);
  delayMicroseconds(9000);
}

void Poll(void)
{
  sendInstruction(0xF0000000);
}


