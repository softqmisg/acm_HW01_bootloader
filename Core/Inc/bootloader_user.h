/*
 * bootloader_user.h
 *
 *  Created on: Jul 11, 2020
 *      Author: mehdi
 */

#ifndef INC_BOOTLOADER_USER_H_
#define INC_BOOTLOADER_USER_H_

void Enter_Bootloader(char *filename);
uint8_t SD_Init(void);
void    SD_DeInit(void);
void    SD_Eject(void);
void    GPIO_Init(void);
void    GPIO_DeInit(void);


#endif /* INC_BOOTLOADER_USER_H_ */
