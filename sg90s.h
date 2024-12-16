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
#include <rtdevice.h>

#define PWM_DEV_NAME        "pwm2"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1      /* PWM通道 */
#define MAX_PERIOD  20000000

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */

int sg90s_init(void) ;
