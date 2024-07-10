#include "MT6835Sensor.h"
#include "FOCMotor.h"
// void Enable_MT6835(void)
// {
//     HAL_GPIO_WritePin(GPIOA,GPIO_PIN15,GPIO_PIN_RESET);
// }
// void Disable_MT6835(void)
// {
//     HAL_GPIO_WritePin(GPIOA,GPIO_PIN15,GPIO_PIN_SET);
// }

uint32_t Angle;
uint16_t TxData;
uint16_t RxData[3];

void Read_AngleReg(void)
{
    TxData = ContinuousRead|0x0003;
    HAL_SPI_TransmitReceive(&hspi1,(unsigned char *)&TxData,(unsigned char *)&RxData,3,100);

    Angle = (RxData[1]<<5)|(RxData[2]>>11);

    printf("%d\n",Angle);
}

