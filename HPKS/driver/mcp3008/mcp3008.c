
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 * 
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   mcp3008.c
 * Author: Christian Rost  <rost at entronix.de>
 *
 * Created on 2. Dezember 2012, 11:52
 */


#include <defines/used_includes.h>
#include "mcp3008.h"

/*
static  SPI_CONFIG_struct mcp3008_spi_config = {
  SPI_MODE_3|SPI_NO_CS,
  8,
  100000,
  0,
  tx,
  rx,
  3};
*/

#define MCP3008_SPI_START_BYTE    0x01


int mcp3008_adc_read(uint8_t single_diff, uint8_t channel, uint16_t *adcout) {

  if(single_diff != MCP3008_ADC_SINGLE && single_diff != MCP3008_ADC_DIFF )
    return -1;
  
  if(channel > ADC_7 )
    return -1;
 
bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); // The default
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); // The default
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024); // ~ 4 MHz
  bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE); // 

// Active the CS pin
  
  MCP3008_CS_CLR;
 // SleepMs(1);
  
  
  uint8_t tx[10] = { MCP3008_SPI_START_BYTE, (single_diff|channel)<<4 };
	uint8_t rx[10] = { 0 };
  bcm2835_spi_transfernb(tx, rx, 3);
  

  bcm2835_spi_end();

  
          
  // 0100 aaa r/w 
  
  
	
  
  MCP3008_CS_SET;
  
  *adcout = ((rx[1]&0x03) << 8) | rx[2];
          
  return (EXIT_SUCCESS);
}
