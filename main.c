#include "string.h"
#include "stdlib.h"

uint8_t Rx_DataArr[12], Rx_Data, arrCount=0, pkgCount=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		//Add the recieved byte to the array and increment the counter afterwards
		Rx_DataArr[arrCount] = Rx_Data;
		arrCount++;

		//Count 0xFF
		if(Rx_Data == 0xFF)
			pkgCount++;
		else
			pkgCount = 0;

		//If the data is recieved correctly;
		if(pkgCount == 3)
		{
			//Send the data to the sender back:
			uint8_t count = 0, FFCount = 0;
			for(uint8_t i = 0; FFCount < 3; i++)
			{
				count++;
				if(Rx_DataArr[i] == 0xFF) FFCount++;
			}
      
      //Create a new buffer same size as the recieved data
			uint8_t *transferBuf = (uint8_t*) malloc(count * sizeof(uint8_t));

			for(uint8_t i = 0; i < count; i++)
			{
				transferBuf[i] = Rx_DataArr[i];
			}

      //Send the final array through UART. 
      //Its possible to expand this code from here:
			HAL_UART_Transmit(&huart1, transferBuf, count, HAL_MAX_DELAY);

      //Free the array buffer memory
			free(transferBuf);

			//Reset the buffer counters
			pkgCount = 0;
			arrCount = 0;
		}

    //Call UART function for continous data recieving 
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&Rx_Data, 1);
	}
}

int main(void)
{
  //Start Interrupt Recieving
  HAL_UART_Receive_IT (&huart1, (uint8_t *)&Rx_Data, 1);

  while (1)
  {
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  HAL_Delay(1000);
  }

}