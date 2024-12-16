/*
 * 程序清单：这是一个 PWM 设备使用例程
 * 例程导出了 pwm_led_sample 命令到控制终端
 * 命令调用格式：pwm_led_sample
 * 程序功能：通过 PWM 设备控制 LED 灯的亮度，可以看到LED不停的由暗变到亮，然后又从亮变到暗。
 *
 * t = 0.5ms——————-舵机会转动 0 °
t = 1.0ms——————-舵机会转动 45°
t = 1.5ms——————-舵机会转动 90°
t = 2.0ms——————-舵机会转动 135°
t = 2.5ms——————-舵机会转动180°   12.5 %
*
*/

#include <rtthread.h>
#include <sg90s.h>

int sg90s_init(void)
{
    /* 查找设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set_dead_time(pwm_dev, PWM_DEV_CHANNEL, 5000);
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, MAX_PERIOD, 0);
    /* 使能设备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);

}
