
#include "stm32f10x.h"
#include <stdlib.h>
#include "MyProject.h"

/************************************************
SimpleMotor开发板
开环速度控制和开环位置控制  演示
=================================================
本程序仅供学习，引用代码请标明出处
使用教程：https://blog.csdn.net/loop222/article/details/119220638
创建日期：20230208
作    者：loop222 @郑州
************************************************/
/******************************************************************************/
#define LED_blink    GPIOC->ODR^=(1<<13)
/******************************************************************************/
float target;
/******************************************************************************/
void commander_run(void);
/******************************************************************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA,GPIOB,GPIOC,AFIO;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;         //PC13是LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  //速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //对选中管脚初始化
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);                 //上电点亮LED
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          //PB9是motor1的使能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);                  //低电平解除
}
/******************************************************************************/
//开环控制最重要的参数就是voltage_limit
//1、电机抖动转不起来把voltage_limit设置的大一点，
//2、电机发热严重的把voltage_limit设置的小一点，
//3、串口发送指令“T10”，后面要有回车换行符
//4、电机能转就表示一切正常，可以学习下一章了，开环不是电机控制的常态，不要纠结太久。
int main(void)
{
	GPIO_Config();
	uart_init(115200);
	TIM2_PWM_Init();
	TIM4_1ms_Init();           //interrupt per 1ms
	
	delay_ms(1000);            //Wait for the system to stabilize
	
	voltage_power_supply=12;   //V
	voltage_limit=2.5;         //V，最大值需小于12/1.732=6.9。大功率航模电机设置的小一点0.5-1；小功率云台电机设置的大一点1-3
	velocity_limit=20;         //rad/s angleOpenloop() use it
	controller=Type_velocity_openloop;  //Type_angle_openloop;  //
	pole_pairs=7;              //极对数
	
	M1_Enable;
  printf("Motor ready.\r\n");
	
	systick_CountMode();   //不能再调用delay_us()和delay_ms()函数
	target = 6.28;         //上电后以6.28rad/s的转速转动（1圈/秒）
	
	while(1)
	{
		if(time1_cntr>=200)  //0.2s
		{
			time1_cntr=0;
			LED_blink;
		}
		move(target);
		commander_run();
	}
}
/******************************************************************************/
void commander_run(void)
{
	if((USART_RX_STA&0x8000)!=0)
	{
		switch(USART_RX_BUF[0])
		{
			case 'H':
				printf("Hello World!\r\n");
				break;
			case 'T':   //T6.28
				target=atof((const char *)(USART_RX_BUF+1));
				printf("RX=%.4f\r\n", target);
				break;
		}
		USART_RX_STA=0;
	}
}
/******************************************************************************/




