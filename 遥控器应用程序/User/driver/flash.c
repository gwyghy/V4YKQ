 /*************************************************************
** �ļ���:  flash.c
** Copyright (c) 1998-1999 ��������Ӽ������޹�˾����������
** ������:  ��΢��
** �ա���:  2013.03.01
** �޸���:	
** �ա���:	
** �衡��:  ��Һ����ĿUCOS_IIϵͳӦ��֮flashоƬ(W25QXX)������
**
** �桡��:	V1.0A
**--------------------------------------------------------------------------
*************************************************************/
#include "includes.h"
#ifdef INIT_FLASH1
extern const uint8_t  GB2312_A[20*96][32];
extern const uint8_t  GB2312_B[20*96][32];
extern const uint8_t  GB2312_C[20*96][32];
extern const uint8_t  GB2312_D[20*96][32];
extern const uint8_t  GB2312_E[7*96][32];
extern const uint8_t  ASCII[100][16];
#endif
#ifdef INIT_FLASH2
extern const uint8_t  GB2312_D[20*96][32];	
extern const uint8_t  GB2312_E[7*96][32];
extern const uint8_t  ASCII[100][16];
#endif
/*********************************************************
** ��������W25QXX_LowLevel_DeInit
** �䡡�룺None  
** �䡡����None
** ����������Destruciton of spi API. 
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Disable the sFLASH_SPI  */
  SPI_Cmd(W25QXX_SPI, DISABLE);
  
  /*!< DeInitializes the sFLASH_SPI */
  SPI_I2S_DeInit(W25QXX_SPI);
  
  /*!< sFLASH_SPI Periph clock disable */
  RCC_APB1PeriphClockCmd(W25QXX_SPI_CLK, DISABLE);
  
  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(W25QXX_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_MISO_PIN;
  GPIO_Init(W25QXX_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_MOSI_PIN;
  GPIO_Init(W25QXX_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = W25QXX_CS_PIN;
  GPIO_Init(W25QXX_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************
** ��������W25QXX_LowLevel_Init
** �䡡�룺None 
** �䡡����None
** ����������Constrution of spi API, and initialize it.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO 
       and sFLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(W25QXX_WP_GPIO_CLK | W25QXX_CS_GPIO_CLK | W25QXX_SPI_MOSI_GPIO_CLK | W25QXX_SPI_MISO_GPIO_CLK |
                         W25QXX_SPI_SCK_GPIO_CLK, ENABLE );

  /*!< sFLASH_SPI Periph clock enable */
  RCC_APB1PeriphClockCmd(W25QXX_SPI_CLK, ENABLE); 



  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(W25QXX_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_MOSI_PIN;
  GPIO_Init(W25QXX_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = W25QXX_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
  GPIO_Init(W25QXX_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = W25QXX_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(W25QXX_CS_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: WP */
  GPIO_InitStructure.GPIO_Pin = W25QXX_WP_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(W25QXX_WP_GPIO_PORT, &GPIO_InitStructure);

//  GPIO_PinAFConfig(GPIO_Remap_SPI3 , ENABLE);
}
/*********************************************************
** ��������W25QXX_DeInit
** �䡡�룺None  
** �䡡����None
** ����������Destrution of flash API.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_DeInit(void)
{
  W25QXX_LowLevel_DeInit();
}

/*********************************************************
** ��������W25QXX_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
//uint8_t  tmp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void W25QXX_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
#if defined(INIT_FLASH1)||defined(INIT_FLASH2)
	uint32_t i;
	uint8_t  ttp[16]={0};
#endif

  W25QXX_LowLevel_Init();
    
  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_WP_HIGH();
  W25QXX_CS_HIGH();

  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
#endif

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(W25QXX_SPI, &SPI_InitStructure);

  /*!< Enable the W25QXX_SPI  */
  SPI_Cmd(W25QXX_SPI, ENABLE);

#ifdef INIT_FLASH1
    if(FLASH_ID == W25QXX_ReadID())                                                  //Read the ID of flash to avoid the wrong instrument operation.  
    {	
		W25QXX_ReadBuffer(ttp, 0, 16);
		//if(ttp[0]!=0)
		//{
		for(i=0;i<0x400000;i+=0x10000)
		{W25QXX_EraseBulk(i);}   
		W25QXX_ReadBuffer(ttp, 0, 16);                                                       //Erasing the sector which is located from 0x100000
		W25QXX_WriteBuffer((uint8_t *)GB2312_A, 0x000000, 1920*32);						 //Writing to 0x100000
		W25QXX_WriteBuffer((uint8_t *)GB2312_B, 0x010000, 1920*32);
		W25QXX_WriteBuffer((uint8_t *)GB2312_C, 0x020000, 1920*32);  
		//}
    }
#endif
#ifdef INIT_FLASH2
	if(FLASH_ID == W25QXX_ReadID())                                                  //Read the ID of flash to avoid the wrong instrument operation.  
    {
		//uint8_t  tmp;
		//uint8_t  ttp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		W25QXX_ReadBuffer(ttp, 0x030000,16);
		//if(ttp[0]!=0){ 
		W25QXX_WriteBuffer((uint8_t *)GB2312_D, 0x030000, 1920*32); 
		W25QXX_WriteBuffer((uint8_t *)GB2312_E, 0x040000, 672*32); 
		W25QXX_WriteBuffer((uint8_t *)ASCII, 0x050000, 100*16);
		//} 
	}
#endif
}

/*********************************************************
** ��������W25QXX_EraseSector
** �䡡�룺SectorAddr ---Easer the adress of sector.  
** �䡡����Result
** ����������Easer a complete Sector to 0xff.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_EraseSector(uint32_t SectorAddr)
{
  if( SectorAddr >FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;
  
  /*!< Send write enable instruction */
  W25QXX_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();
  /*!< Send Sector Erase instruction */
  W25QXX_SendByte(W25QXX_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  W25QXX_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  W25QXX_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  W25QXX_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  /*!< Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** ��������W25QXX_EraseBulk
** �䡡�룺None  
** �䡡����Result
** ����������Easer a complete Bulk(64KB) to 0xff.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_EraseBulk(uint32_t BulkAddr)
{
  if( BulkAddr >FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;
  W25QXX_WriteEnable();
  W25QXX_CS_LOW();
  W25QXX_SendByte(W25QXX_CMD_BE_64K);
  W25QXX_SendByte((BulkAddr & 0xFF0000) >> 16);
  W25QXX_SendByte((BulkAddr & 0xFF00) >> 8);
  W25QXX_SendByte(BulkAddr & 0xFF);
  W25QXX_CS_HIGH();
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** ��������W25QXX_WritePage
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write an entire page of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  if( WriteAddr >FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;
  /*!< Enable the write access to the FLASH */
  W25QXX_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  W25QXX_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  W25QXX_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  W25QXX_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    W25QXX_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  /*!< Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** ��������W25QXX_WriteBuffer
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write a certain mount of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  //if((WriteAddr+NumByteToWrite) > FLASH_ADDRESS_MAX) return FALSE;
  if((WriteAddr+NumByteToWrite)+NumByteToWrite > FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;
  Addr = WriteAddr % W25QXX_SPI_PAGESIZE;
  count = W25QXX_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is W25QXX_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25QXX_PAGESIZE */
    {
      W25QXX_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > W25QXX_PAGESIZE */
    {
      while (NumOfPage--)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      W25QXX_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not W25QXX_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25QXX_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > W25QXX_PAGESIZE */
      {
        temp = NumOfSingle - count;

        W25QXX_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25QXX_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25QXX_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > W25QXX_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

      W25QXX_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  return SUCCESS;
}

/*********************************************************
** ��������W25QXX_ReadBuffer
** �䡡�룺pBuffer ---The buffer containing the data read from flash.
**         ReadAddr ---Address in flash for paunching the instrution of read.
**         NumByteToRead ---The mount of bytes to be read  
** �䡡����None
** ����������Read a certain mount of content in flash and write them to a fixed memory.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  if((ReadAddr+NumByteToRead) + NumByteToRead > FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  W25QXX_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  W25QXX_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  W25QXX_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = W25QXX_SendByte(W25QXX_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
  return SUCCESS;
}

/*********************************************************
** ��������W25QXX_ReadID
** �䡡�룺pBuffer1 ---None  
** �䡡����JEDEC ID
** ����������Read the JEDEC ID(manufacturer id and device id)
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint32_t W25QXX_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "RDID " instruction */
  W25QXX_SendByte(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
/*********************************************************
** ��������W25QXX_StartReadSequence
** �䡡�룺ReadAddr ---The start adress to be reading  
** �䡡����1
** ����������Read a seri of data from a start adress continuously.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  W25QXX_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  W25QXX_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  W25QXX_SendByte(ReadAddr & 0xFF);
}

/*********************************************************
** ��������W25QXX_ReadByte
** �䡡�룺None  
** �䡡����Byte which is read.
** ����������Read a byte data from the spi bus.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_ReadByte(void)
{
  return (W25QXX_SendByte(W25QXX_DUMMY_BYTE));
}

/*********************************************************
** ��������W25QXX_SendByte
** �䡡�룺byte --- 8-bit data being sent. 
** �䡡����8-bit data being received.
** ����������Sending the 8-bit data.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint8_t W25QXX_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(W25QXX_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(W25QXX_SPI);
}

/*********************************************************
** ��������W25QXX_SendHalfWord
** �䡡�룺HalfWord --- 16-bit data being sent.  
** �䡡����16-bit data being received.
** ����������Sending the 16-bit data.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
uint16_t W25QXX_SendHalfWord(uint16_t HalfWord)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send Half Word through the sFLASH peripheral */
  SPI_I2S_SendData(W25QXX_SPI, HalfWord);

  /*!< Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(W25QXX_SPI);
}

/*********************************************************
** ��������W25QXX_WriteEnable
** �䡡�룺None  
** �䡡����None
** ����������Send the writing command byte to prepare for writing.(Demonstrated in the protocol of SPI standard in Manual of W25QXX)
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Write Enable" instruction */
  W25QXX_SendByte(W25QXX_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
}

/*********************************************************
** ��������W25QXX_WaitForWriteEnd
** �䡡�룺none  
** �䡡����none
** ����������Waiting for the last writing operation finished.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
void W25QXX_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  W25QXX_SendByte(W25QXX_CMD_RDSR1);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  }
  while ((flashstatus & W25QXX_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
}
