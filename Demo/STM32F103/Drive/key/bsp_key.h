#ifndef __BSP_KEY_H
#define __BSP_KEY_H 

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP)

/* 按键口对应的RCC时钟 */
#define RCC_ALL_KEY 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE)

#define GPIO_PORT_K1    GPIOE
#define GPIO_PIN_K1	    GPIO_Pin_4

#define GPIO_PORT_K2    GPIOE
#define GPIO_PIN_K2	    GPIO_Pin_3

#define GPIO_PORT_K3    GPIOE
#define GPIO_PIN_K3	    GPIO_Pin_2

#define GPIO_PORT_K4    GPIOA
#define GPIO_PIN_K4	    GPIO_Pin_0

/*
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_SHIF            0  //0不使用组合键（shift键），1使用组合键（shift）
#define KEY_COUNT           4  /* 按键个数, 6个独立建 + 4个组合键 */
#define KEY_FIFO_SIZE	    10  /* 按键FIFO用到变量 */
#define KEY_FILTER_TIME     2   /* 滤波时间 */
#define KEY_LONG_TIME       100 /* 单位10ms， 持续1秒，认为长按事件 */
#define KEY_CLEAR           50  /* 定时清除按键，以10ms为单位，不定义则不使用；
                                   按键fifo带来一个问题:按键具有记忆功能，如果应用程序和按键（优先级高一些）产生了异步，会产生连锁响应*/

/* 根据应用程序的功能重命名按键宏 */
#define KEY_LEFT_PRESS	KEY_1_DOWN //【→】
//#define KEY_LEFT_UNDO	KEY_1_UP
//#define KEY_LEFT_LONG	KEY_1_LONG

#define KEY_RIGHT_PRESS	KEY_2_DOWN //【↓】
//#define KEY_RIGHT_UNDO	KEY_2_UP
//#define KEY_RIGHT_LONG	KEY_2_LONG

#define KEY_UP_PRESS	KEY_3_DOWN //【←】
//#define KEY_UP_UNDO	    KEY_3_UP
//#define KEY_UP_LONG	    KEY_3_LONG

#define KEY_DOWN_PRESS	KEY_4_DOWN //【↑】
//#define KEY_DOWN_UNDO	KEY_4_UP
//#define KEY_DOWN_LONG	KEY_4_LONG

/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_K4,
}KEY_ID_E;

#if KEY_SHIF //组合键模式
    #define KID_SHIFT KID_K6 //指定shift键
#endif

/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
    uint8_t  KeyCodeDown;	/* 按键按下的键值代码 */
    uint8_t  KeyCodeUp;	    /* 按键弹起的键值代码 */
    uint8_t  KeyCodeLong;	/* 按键被持续按下的键值代码 */    
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
    uint8_t  attribute;     //属性，0，通用按键（独立按键）；1，组合按键；2，shift键
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				/* 1键弹起 */
	KEY_1_LONG,				/* 1键长按 */

	KEY_2_DOWN,				/* 2键按下 */
	KEY_2_UP,				/* 2键弹起 */
	KEY_2_LONG,				/* 2键长按 */

	KEY_3_DOWN,				/* 3键按下 */
	KEY_3_UP,				/* 3键弹起 */
	KEY_3_LONG,				/* 3键长按 */

	KEY_4_DOWN,				/* 4键按下 */
	KEY_4_UP,				/* 4键弹起 */
	KEY_4_LONG,				/* 4键长按 */
}KEY_ENUM;

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;

/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t Key_GetState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif //__BSP_KEY_H
