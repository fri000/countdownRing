/*
  SoftwareSerial.cpp - Software serial library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "AttinySoftwareSerial.h"
#include "utils.h"
#include <avr/io.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

//AttinySoftwareSerial::AttinySoftwareSerial(uint8_t receivePin, uint8_t transmitPin)
//{
//  _receivePin = receivePin;
//  _transmitPin = transmitPin;
//  _baudRate = 0;
//}


AttinySoftwareSerial::AttinySoftwareSerial(uint8_t transmitPin)
{
  _transmitPin = transmitPin;
  _baudRate = 0;
}


/******************************************************************************
 * User API
 ******************************************************************************/

//void AttinySoftwareSerial::begin(long speed)
//{
//  _baudRate = speed;
//  _bitPeriod = 1000000 / _baudRate;
//
//  digitalWrite(_transmitPin, HIGH);
//  delayMicroseconds( _bitPeriod); // if we were low this establishes the end
//}


//int AttinySoftwareSerial::read()
//{
//  int val = 0;
//  int bitDelay = _bitPeriod - clockCyclesToMicroseconds(50);
//
//  // one byte of serial data (LSB first)
//  // ...--\    /--\/--\/--\/--\/--\/--\/--\/--\/--...
//  //     \--/\--/\--/\--/\--/\--/\--/\--/\--/
//  //    start  0   1   2   3   4   5   6   7 stop
//
//  while (digitalRead(_receivePin));
//
//  // confirm that this is a real start bit, not line noise
//  if (digitalRead(_receivePin) == LOW) {
//    // frame start indicated by a falling edge and low start bit
//    // jump to the middle of the low start bit
//    delayMicroseconds(bitDelay / 2 - clockCyclesToMicroseconds(50));
//
//    // offset of the bit in the byte: from 0 (LSB) to 7 (MSB)
//    for (int offset = 0; offset < 8; offset++) {
//    // jump to middle of next bit
//    delayMicroseconds(bitDelay);
//
//    // read bit
//    val |= digitalRead(_receivePin) << offset;
//    }
//
//    delayMicroseconds(_bitPeriod);
//
//    return val;
//  }
//
//  return -1;
//}

//for now, this function is hardcoded for 9600 baud
//and to output on pin 1 of port B
void AttinySoftwareSerial::print(uint8_t data)
{
    uint8_t bit9600Delay = 104;
    uint8_t mask;
    //startbit
    //set PORTB.1 low
    PORTB &= ~(1<<1);

    delayMicroseconds(bit9600Delay);

    for (mask = 0x01; mask>0; mask <<= 1)
    {
        if (data & mask)    // choose bit
        {
            //set PORTB.1 high
            PORTB |= 1<<1;    // send 1
        }
        else
        {
            //set PORTB.1 low
            PORTB &= ~(1<<1); // send 0
        }
        delayMicroseconds(bit9600Delay);
    }

    //stop bit
    //set PORTB.1 high
    PORTB |= 1<<1;
    delayMicroseconds(bit9600Delay);
}







void AttinySoftwareSerial::print(const char *s)
{
  while (*s)
    print(*s++);
}

void AttinySoftwareSerial::print(char c)
{
  print((uint8_t) c);
}

void AttinySoftwareSerial::print(int n)
{
  print((long) n);
}

void AttinySoftwareSerial::print(unsigned int n)
{
  print((unsigned long) n);
}

void AttinySoftwareSerial::print(long n)
{
  if (n < 0) {
    print('-');
    n = -n;
  }
  printNumber(n, 10);
}

void AttinySoftwareSerial::print(unsigned long n)
{
  printNumber(n, 10);
}

void AttinySoftwareSerial::print(long n, int base)
{
  if (base == 0)
    print((char) n);
  else if (base == 10)
    print(n);
  else
    printNumber(n, base);
}

void AttinySoftwareSerial::println(void)
{
  print('\r');
  print('\n');
}

void AttinySoftwareSerial::println(char c)
{
  print(c);
  println();
}

void AttinySoftwareSerial::println(const char c[])
{
  print(c);
  println();
}

void AttinySoftwareSerial::println(uint8_t b)
{
  print(b);
  println();
}

void AttinySoftwareSerial::println(int n)
{
  print(n);
  println();
}

void AttinySoftwareSerial::println(long n)
{
  print(n);
  println();
}

void AttinySoftwareSerial::println(unsigned long n)
{
  print(n);
  println();
}

void AttinySoftwareSerial::println(long n, int base)
{
  print(n, base);
  println();
}

// Private Methods /////////////////////////////////////////////////////////////

void AttinySoftwareSerial::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}
