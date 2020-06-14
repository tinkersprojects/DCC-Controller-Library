#include "Arduino.h"
#include "Dcc.h"

/**************************** SET UP ****************************/

Dcc::Dcc(uint8_t _OutputA, uint8_t _OutputB)
{
  OutputA = _OutputA;
  OutputB = _OutputB;

  pinMode(OutputA, OUTPUT);
  digitalWrite(OutputA, HIGH);
  OutputABitMask = digitalPinToBitMask(OutputA);
  uint8_t OutputAport = digitalPinToPort(OutputA);
  OutputAPortRegister = portOutputRegister(OutputAport);

  pinMode(OutputB, OUTPUT);
  digitalWrite(OutputB, HIGH);
  OutputBBitMask = digitalPinToBitMask(OutputB);
  uint8_t OutputBport = digitalPinToPort(OutputB);
  OutputBPortRegister = portOutputRegister(OutputBport);
}

void Dcc::begin()
{
  flush();
  writeOutput(HIGH);
}







/**************************** SET ****************************/

size_t Dcc::set(uint8_t address, uint8_t Direction, uint8_t speed)
{
  if(speed >= 32)
    return 0;

  uint8_t data = speed;
  bitWrite(data,5, (Direction>0));
  bitWrite(data,6, 1);

  return set(address, data);
}

size_t Dcc::set(uint8_t address, uint8_t data)
{
  if(address >= 127)
    return 0;

  for (uint8_t i = 0; i < DCC_Buffer_size; i++)
  {
    if(OutputBufferArray[i].send == 0)
    {
      OutputBufferArray[i].send = 1;
      OutputBufferArray[i].address = address;
      OutputBufferArray[i].data = data;
      return 1;
    }
  }
  return 0; // buffer is full
}

void Dcc::Send()
{
  if(OutputBufferArray[0].send == 1)
    output(OutputBufferArray[0].address, OutputBufferArray[0].data);

  for (uint8_t i = 1; i < DCC_Buffer_size; i++)
  {
    OutputBufferArray[i-1] = OutputBufferArray[i];
    if(OutputBufferArray[i].send == 0)
      return;
  }
  OutputBufferArray[DCC_Buffer_size-1].send = 0;
}

void Dcc::SendAll()
{
  for (uint8_t i = 1; i < DCC_Buffer_size; i++)
  {
    if(OutputBufferArray[i].send)
      output(OutputBufferArray[i].address, OutputBufferArray[i].data);
    OutputBufferArray[i].send = 0;
  }
}

void Dcc::flush()
{
  for (uint8_t i = 0; i < DCC_Buffer_size; i++)
    OutputBufferArray[i].send = 0;
}







/**************************** OUTPUT ****************************/

// 1111111111 0 0AAAAAAA 0 01DUSSSS 0 EEEEEEEE 1
// 1111111111	0	AAAAAAA0 0 01DUSSSS 0	EEEEEEEE 1

void Dcc::output(uint8_t address, uint8_t data)
{
  uint8_t error = address ^ data;
  
  for (int i=0; i<14; i++)
    writeBit(1);

  writeBit(0);

  writeBit(0);

  for (byte mask = 64; mask>0; mask >>= 1)
    writeBit(address & mask);
  
  writeBit(0);
  writeBit(0);
  writeBit(1);
  
  for (byte mask = 32; mask>0; mask >>= 1)
    writeBit(data & mask);
  
  writeBit(0);
  
  for (byte mask = 128; mask>0; mask >>= 1)
    writeBit(error & mask);
  
  writeBit(1);
}

void Dcc::writeBit(uint8_t bit)
{
  if (bit) // choose bit
    delayMicroseconds(58); // send 1
  else
    delayMicroseconds(100); // send 0

  writeOutput(LOW);

  if (bit) // choose bit
    delayMicroseconds(58); // send 1
  else
    delayMicroseconds(100); // send 0

  writeOutput(HIGH);
}

void Dcc::writeOutput(uint8_t pinState)
{
  if (pinState == LOW)
  {
    *OutputBPortRegister &= ~OutputBBitMask;
    *OutputAPortRegister |= OutputABitMask;
  }
  else
  {
    *OutputAPortRegister &= ~OutputABitMask;
    *OutputBPortRegister |= OutputBBitMask;
  }
}