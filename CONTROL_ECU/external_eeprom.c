 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Shahd Hamad
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"
#include <util/delay.h>

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 
		 
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* write byte to eeprom */
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
	
    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

/*
 * Write String in EEPROM at a specific address
 */
uint8 EEPROM_writeString(uint16 u16addr, const uint8 *str)
{
    while (*str)
    {
        /* Write each character in the string to EEPROM */
        EEPROM_writeByte(u16addr++, *str++);
        _delay_ms(10);
    }
    /* Write null terminator to indicate end of string */
    EEPROM_writeByte(u16addr, '\0');
    _delay_ms(10);
    return SUCCESS;
}

/*
 * Read String from EEPROM at a specific address
 */
uint8 EEPROM_readString(uint16 u16addr, uint8 *str, uint8 maxLen)
{
    uint8 i = 0;
    uint8 data;

    /* Read each character from EEPROM until null terminator or maxLen */
    for (i = 0; i < maxLen - 1; i++)
    {
        if (EEPROM_readByte(u16addr++, &data) != SUCCESS)
        {
            return ERROR;  // Return error if read operation fails
        }
        str[i] = data;
        if (data == '\0')
        {
            return SUCCESS;  // Stop if null terminator is found
        }
    }
    str[i] = '\0';  // Ensure null termination if maxLen limit is reached
    return SUCCESS;
}
