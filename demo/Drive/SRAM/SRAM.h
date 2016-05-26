#ifndef _SRAM_H_
#define _SRAM_H_

void FSMC_SRAM_Init(void);//初始化外部SRAM
void SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n);
void SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n);

#endif//_SRAM_H_
