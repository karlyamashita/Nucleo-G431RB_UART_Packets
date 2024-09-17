/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 *      The object of this PollingRoutine.c/h files is to not have to write code in main.c which already has a lot of generated code.
 *      It is cumbersome having to scroll through all the generated code for your own code and having to find a USER CODE section so your code is not erased when CubeMX re-generates code.
 *      
 *      Direction: Call PollingInit before the main while loop. Call PollingRoutine from within the main while loop
 * 
 *      Example;
        // USER CODE BEGIN WHILE
        PollingInit();
        while (1)
        {
            PollingRoutine();
            // USER CODE END WHILE

            // USER CODE BEGIN 3
        }
        // USER CODE END 3

 */


#include "main.h"

extern UART_HandleTypeDef huart2;


UartBufferStruct uart2_msg =
{
	.huart = &huart2,
	.rx.uartType = UART_BINARY,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.bytePtrSize = UART_RX_BYTE_BUFFER_SIZE,
	.rx.checksumType = CHECKSUM_MOD256,
	.rx.packetSize = 8
};


void PollingInit(void)
{
	UART_EnableRxInterrupt(&uart2_msg);
	UART_NotifyUser(&uart2_msg, "STM32 Ready", strlen("STM32 Ready"), true);
}

void PollingRoutine(void)
{
	UART_SortPacketBuffer(&uart2_msg);
	UART_ParsePacket(&uart2_msg);
	UART_CheckRxIntError(&uart2_msg);
}

void UART_ParsePacket(UartBufferStruct *msg)
{
	if(UART_RxMsgRdy(msg))
	{
		UART_TX_AddDataToBuffer(msg, msg->rx.msgToParse->data, msg->rx.packetSize);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2_msg.huart)
	{
		UART_AddByteToBuffer(&uart2_msg); // add byte to buffer.
		UART_EnableRxInterrupt(&uart2_msg); // enable interrupt again
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart2_msg.huart)
	{
		UART_TxMessage_IT(&uart2_msg); // try and transmit if more messages in queue
	}
}







