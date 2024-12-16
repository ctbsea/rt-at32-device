/**
 * @file drv8833.c
 * @brief DRV8833双H桥电机驱动器控制实现
 */

#include "drv8833.h"
#include "rtdevice.h"
#include "at32f423_tmr.h"

#define PWM_DEV_NAME        "pwm2"  /* PWM设备名称 */
#define IN1_CH 1
#define IN2_CH 2

/** @brief 最大速度值 */
#define MAX_SPEED 100

/** @brief 频率 10000HZ 01ms */
#define PERIOD 1000000

/** @brief 当前衰减模式 */
static DecayMode currentDecayMode = SLOW_DECAY;

struct rt_device_pwm *pwm_dev;
/**
 * @brief 设置IN1引脚的PWM占空比
 * @param duty 占空比值
 */
static inline void __SetIn1PWM(uint8_t duty)
{
    rt_kprintf("1 %d %d \n", duty,   duty  * PERIOD / MAX_SPEED  );
    rt_pwm_set(pwm_dev, IN1_CH, PERIOD , duty * PERIOD / MAX_SPEED );
}

/**
 * @brief 设置IN2引脚的PWM占空比
 * @param duty 占空比值
 */
static inline void __SetIn2PWM(uint8_t duty)
{
    rt_kprintf("2 %d %d \n", duty, duty  * PERIOD / MAX_SPEED );
    rt_pwm_set(pwm_dev, IN2_CH, PERIOD , duty  * PERIOD / MAX_SPEED );
}

/**
 * @brief 初始化DRV8833
 */
rt_err_t DRV8833_Init(void)
{
    /* 查找设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    rt_pwm_set(pwm_dev, IN1_CH, PERIOD, 0);
    rt_pwm_enable(pwm_dev, IN1_CH);
    rt_pwm_set(pwm_dev, IN2_CH, PERIOD, 0);
    rt_pwm_enable(pwm_dev, IN2_CH);
}

/**
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
void DRV8833_SetDecayMode(DecayMode mode)
{
    currentDecayMode = mode;
}

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
 */
void DRV8833_Forward(uint8_t speed)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(speed);
        __SetIn2PWM(0);
    } else {
        __SetIn1PWM(MAX_SPEED);
        __SetIn2PWM(MAX_SPEED - speed);
    }
}

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void DRV8833_Backward(uint8_t speed)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(0);
        __SetIn2PWM(speed);
    } else {
        __SetIn1PWM(MAX_SPEED - speed);
        __SetIn2PWM(MAX_SPEED);
    }
}

/**
 * @brief 电机刹车
 */
void DRV8833_Brake(void)
{
    __SetIn1PWM(MAX_SPEED);
    __SetIn2PWM(MAX_SPEED);
}

/**
 * @brief 电机滑行
 */
void DRV8833_Coast(void)
{
    __SetIn1PWM(0);
    __SetIn2PWM(0);
}
