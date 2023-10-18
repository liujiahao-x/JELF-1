/*!
 * @file        apm32s10x_spi.c
 *
 * @brief       This file provides all the SPI firmware functions
 *
 * @version     V1.0.1
 *
 * @date        2022-12-31
 *
 * @attention
 *
 *  Copyright (C) 2022-2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be usefull and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "apm32s10x_spi.h"
#include "apm32s10x_rcm.h"

/** @addtogroup APM32S10x_StdPeriphDriver
  @{
*/

/** @addtogroup SPI_Driver SPI Driver
  @{
*/

/** @defgroup SPI_Functions Functions
  @{
*/

/*!
 * @brief     Reset the specified SPIx peripheral
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_Reset(SPI_T* spi)
{
    if (spi == SPI1)
    {
        RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_SPI1);
        RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_SPI1);
    }
    else if (spi == SPI2)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_SPI2);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_SPI2);
    }
}

/*!
 * @brief     Configure the SPI peripheral according to the specified parameters in the spiConfig
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     spiConfig: pointer to a SPI_Config_T structure
 *
 * @retval    None
 */
void SPI_Config(SPI_T* spi, SPI_Config_T* spiConfig)
{
    spi->CTRL1 &= 0x3040;
    spi->CTRL1 |= (uint16_t)((uint32_t)spiConfig->direction | spiConfig->mode |
                             spiConfig->length | spiConfig->polarity |
                             spiConfig->phase | spiConfig->nss |
                             spiConfig->baudrateDiv | spiConfig->firstBit);
    spi->CRCPOLY = spiConfig->crcPolynomial;
}

/*!
 * @brief     Fill each SPI_Config_T member with its default value
 *
 * @param     spiConfig: pointer to a SPI_Config_T structure
 *
 * @retval    None
 */
void SPI_ConfigStructInit(SPI_Config_T* spiConfig)
{
    spiConfig->direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    spiConfig->mode = SPI_MODE_SLAVE;
    spiConfig->length = SPI_DATA_LENGTH_8B;
    spiConfig->polarity = SPI_CLKPOL_LOW;
    spiConfig->phase = SPI_CLKPHA_1EDGE;
    spiConfig->nss = SPI_NSS_HARD;
    spiConfig->baudrateDiv = SPI_BAUDRATE_DIV_2;
    spiConfig->firstBit = SPI_FIRSTBIT_MSB;
    spiConfig->crcPolynomial = 7;
}

/*!
 * @brief     Enable the specified SPI peripheral
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_Enable(SPI_T* spi)
{
    spi->CTRL1_B.SPIEN = BIT_SET;
}

/*!
 * @brief     Disable the specified SPI peripheral
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_Disable(SPI_T* spi)
{
    spi->CTRL1_B.SPIEN = BIT_RESET;
}

/*!
 * @brief     Enable the SPIx DMA interface.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     dmaReq: specify the SPI DMA transfer request
 *              The parameter can be one of following values:
 *              @arg SPI_DMA_REQ_TX: Tx buffer DMA transfer request
 *              @arg SPI_DMA_REQ_RX: Rx buffer DMA transfer request
 * @retval    None
 */
void SPI_EnableDMA(SPI_T* spi, SPI_DMA_REQ_T dmaReq)
{
    if (dmaReq == SPI_DMA_REQ_TX)
    {
        spi->CTRL2_B.TXDEN = ENABLE;
    }
    else
    {
        spi->CTRL2_B.RXDEN = ENABLE;
    }
}

/*!
 * @brief     Disable the SPIx DMA interface.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     dmaReq: specify the SPI DMA transfer request
 *              The parameter can be one of following values:
 *              @arg SPI_DMA_REQ_TX: Tx buffer DMA transfer request
 *              @arg SPI_DMA_REQ_RX: Rx buffer DMA transfer request
 * @retval    None
 */
void SPI_DisableDMA(SPI_T* spi, SPI_DMA_REQ_T dmaReq)
{
    if (dmaReq == SPI_DMA_REQ_TX)
    {
        spi->CTRL2_B.TXDEN = DISABLE;
    }
    else
    {
        spi->CTRL2_B.RXDEN = DISABLE;
    }
}

/*!
 * @brief     Transmit a Data through the SPIx peripheral.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     data: Data to be transmitted
 *
 * @retval    None
 */
void SPI_TxData(SPI_T* spi, uint16_t data)
{
    spi->DATA = data;
}

/*!
 * @brief     Return the most recent received data by the SPIx peripheral.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    data :The value of the received data
 *
 * @retval    None
 */
uint16_t SPI_RxData(SPI_T* spi)
{
    return spi->DATA;
}

/*!
 * @brief     Set the SPI NSS internal by Software
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_SetSoftwareNSS(SPI_T* spi)
{
    spi->CTRL1_B.ISSEL = BIT_SET;
}

/*!
 * @brief     Reset the SPI NSS internal by Software
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_ResetSoftwareNSS(SPI_T* spi)
{
    spi->CTRL1_B.ISSEL = BIT_RESET;
}

/*!
 * @brief     Enable the specified SPI SS output
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_EnableSSOutput(SPI_T* spi)
{
    spi->CTRL2_B.SSOEN = BIT_SET;
}

/*!
 * @brief     Disable the specified SPI SS output
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_DisableSSOutput(SPI_T* spi)
{
    spi->CTRL2_B.SSOEN = BIT_RESET;
}

/*!
 * @brief     Configure the specified SPI data size
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     length: specify the SPI data size.
 *              This parameter can be one of the following values:
 *              @arg SPI_DATA_LENGTH_16B: Set data frame format to 16bit
 *              @arg SPI_DATA_LENGTH_8B : Set data frame format to 8bit
 *
 * @retval    None
 */
void SPI_ConfigDataSize(SPI_T* spi, SPI_DATA_LENGTH_T length)
{
    spi->CTRL1_B.DFLSEL = BIT_RESET;
    spi->CTRL1 |= length;
}

/*!
 * @brief     Transmit CRC value
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_TxCRC(SPI_T* spi)
{
    spi->CTRL1_B.CRCNXT = BIT_SET;
}

/*!
 * @brief     Enable the specified SPI CRC value calculation of the transferred bytes
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_EnableCRC(SPI_T* spi)
{
    spi->CTRL1_B.CRCEN = BIT_SET;
}

/*!
 * @brief     Disable the specified SPI CRC value calculation of the transferred bytes
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    None
 */
void SPI_DisableCRC(SPI_T* spi)
{
    spi->CTRL1_B.CRCEN = BIT_RESET;
}

/*!
 * @brief     Read the specified SPI transmit CRC register value
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    The SPI transmit CRC register value
 */
uint16_t SPI_ReadTxCRC(SPI_T* spi)
{
    return spi->TXCRC_B.TXCRC;
}

/*!
 * @brief     Read the specified SPI receive CRC register value
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    The SPI receive CRC register value
 */
uint16_t SPI_ReadRxCRC(SPI_T* spi)
{
    return spi->RXCRC_B.RXCRC;
}

/*!
 * @brief     Read the specified SPI CRC Polynomial register value
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @retval    The SPI CRC Polynomial register value
 */
uint16_t SPI_ReadCRCPolynomial(SPI_T* spi)
{
    return spi->CRCPOLY_B.CRCPOLY;
}

/*!
 * @brief     Configure the specified SPI data transfer direction
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     direction: Select the SPI data transfer direction
 *              The parameter can be one of following values:
 *              @arg SPI_DIRECTION_RX: Selects Rx receive direction
 *              @arg SPI_DIRECTION_TX: Selects Tx transmission direction
 *
 * @retval    None
 */
void SPI_ConfigBiDirectionalLine(SPI_T* spi, SPI_DIRECTION_SELECT_T direction)
{
    if (direction == SPI_DIRECTION_TX)
    {
        spi->CTRL1 |= SPI_DIRECTION_TX;
    }
    else
    {
        spi->CTRL1 &= SPI_DIRECTION_RX;
    }
}

/*!
 * @brief     Enable the specified SPI interrupts.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     interrupt: specify the TMR interrupts sources
 *              The parameter can be one of following values:
 *              @arg SPI_INT_TXBE: Tx buffer empty interrupt
 *              @arg SPI_INT_RXBNE: Rx buffer not empty interrupt
 *              @arg SPI_INT_ERR: Error interrupt
 *
 * @retval    None
 */
void SPI_EnableInterrupt(SPI_T* spi, SPI_INT_T interrupt)
{
    spi->CTRL2 |= (interrupt >> 8);
}

/*!
 * @brief     Disable the specified SPI interrupts.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     interrupt: specify the TMR interrupts sources
 *              The parameter can be one of following values:
 *              @arg SPI_INT_TXBE: Tx buffer empty interrupt
 *              @arg SPI_INT_RXBNE: Rx buffer not empty interrupt
 *              @arg SPI_INT_ERR: Error interrupt
 * @retval    None
 */
void SPI_DisableInterrupt(SPI_T* spi, SPI_INT_T interrupt)
{
    spi->CTRL2 &= ~(interrupt >> 8);
}

/*!
 * @brief     Check whether the specified SPI flag is set or not.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     flag: specify the SPI flag to check
 *              The parameter can be one of following values:
 *              @arg SPI_FLAG_RXBNE: Receive buffer not empty flag
 *              @arg SPI_FLAG_TXBE: Transmit buffer empty flag
 *              @arg SPI_FLAG_SCHDIR: Side Channel flag
 *              @arg SPI_FLAG_UDR: Underrun Error flag
 *              @arg SPI_FLAG_CRCE: CRC Error flag
 *              @arg SPI_FLAG_ME: Mode Error flag
 *              @arg SPI_FLAG_OVR: Overrun flag
 *              @arg SPI_FLAG_BSY: Busy flag
 *
 * @retval     SET or RESET
 */
uint8_t SPI_ReadStatusFlag(SPI_T* spi, SPI_FLAG_T flag)
{
    if ((spi->STS & flag) != RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/*!
 * @brief     Clear the SPIx CRC Error flag
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     flag: only clear SPI_FLAG_CRCE(CRC Error flag)
 *
 * @retval    None
 *
 * @note      1)SPI_FLAG_OVR: (OverRun error) flag is cleared by software sequence:
 *              a read operation to SPI_DATA register (SPI_RxData())
 *              followed by a read operation to SPI_STS register (SPI_ReadStatusFlag()).
 *            2)SPI_FLAG_UDR: (UnderRun error) flag is cleared:
 *              a read operation to SPI_STS register (SPI_ReadStatusFlag()).
 *            3)SPI_FLAG_ME: (Mode Fault) flag is cleared by software sequence:
 *              a read/write operation to SPI_STS register (SPI_ReadStatusFlag())
 *              followed by a write operation to SPI_CTRL1 register (SPI_Enable()).
 */
void SPI_ClearStatusFlag(SPI_T* spi, SPI_FLAG_T flag)
{
    spi->STS_B.CRCEFLG = BIT_RESET;
}

/*!
 * @brief     Check whether the specified SPI interrupt has occurred or not.
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     flag: specify the SPI interrupt flag to check.
 *              The parameter can be one of following values:
 *              @arg SPI_INT_RXBNE : Receive buffer not empty interrupt flag
 *              @arg SPI_INT_TXBE  : Transmit buffer empty interrupt flag
 *              @arg SPI_INT_OVR   : Overrun interrupt flag
 *              @arg SPI_INT_CRCE  : CRC Error interrupt flag
 *              @arg SPI_INT_ME    :  Mode Error interrupt flag
 *              @arg SPI_INT_UDR   : Underrun Error interrupt flag
 *
 * @retval       SET or RESET
 */
uint8_t SPI_ReadIntFlag(SPI_T* spi, SPI_INT_T flag)
{
    uint32_t intEnable;
    uint32_t intStatus;

    intEnable = (uint32_t)(spi->CTRL2 & (flag >> 8));
    intStatus = (uint32_t)(spi->STS & flag);

    if (intEnable && intStatus)
    {
        return SET;
    }

    return RESET;
}

/*!
 * @brief     Clear the SPIx CRC Error interrupt flag
 *
 * @param     spi: The SPIx can be 1,2
 *
 * @param     flag: only clear SPI_INT_CRCE(CRC Error interrupt flag)
 *
 * @retval    None
 *
 * @note      1)SPI_INT_OVR: (OverRun interrupt error) flag is cleared by software sequence:
 *              a read operation to SPI_DATA register (SPI_RxData())
 *              followed by a read operation to SPI_STS register (SPI_ReadIntFlag()).
 *            2)SPI_INT_UDR: (UnderRun interrupt error) flag is cleared:
 *              a read operation to SPI_STS register (SPI_ReadIntFlag()).
 *            3)SPI_INT_ME: (Mode interrupt Fault) flag is cleared by software sequence:
 *              a read/write operation to SPI_STS register (SPI_ReadIntFlag())
 *              followed by a write operation to SPI_CTRL1 register (SPI_Enable()).
 */
void SPI_ClearIntFlag(SPI_T* spi, SPI_INT_T flag)
{
    spi->STS_B.CRCEFLG = BIT_RESET;
}

/**@} end of group SPI_Functions */
/**@} end of group SPI_Driver */
/**@} end of group APM32S10x_StdPeriphDriver */
