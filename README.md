# rt-at32-device
设备模块测试，代码整理学习测试

### 增量编码器

### S90s 舵机

t = 0.5ms——————-舵机会转动 0 °
t = 1.0ms——————-舵机会转动 45°
t = 1.5ms——————-舵机会转动 90°
t = 2.0ms——————-舵机会转动 135°
t = 2.5ms——————-舵机会转动180°   

```
MAX_PERIOD = 20ms
sg90s_init();
rt_int32_t count, duty;
while (1)
{
   rt_thread_mdelay(500);
   /* 读取脉冲编码器计数值 */
   rt_device_read(pulse_encoder_dev, 0, &count, 1);
   if (count > 60000)
   {
       count  = 0 ;
       rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
    }else if (count > 20) {
        count = 20 ;
   }
   rt_kprintf("get count %d\n",count);
  // 2.5% ~12.5% 占空比
   duty = (10 *count /20 + 2.5 ) / 100 * MAX_PERIOD;
   rt_kprintf("get duty %d\n",duty);
   rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, MAX_PERIOD, duty);
 }    
```


### DRV8833 和 电机
实现通过增量编码器， 调解电机正反转速
接线注意VM 接电， STBY 接电(高电平)?
in1 in2 /ou1 ou2 顺序无关, 电机转速方向参照手册

```
    DRV8833_Init();

    rt_int32_t count, speed;
    while (1)
    {
         rt_thread_mdelay(2000);
        /* 读取脉冲编码器计数值 */
        rt_device_read(pulse_encoder_dev, 0, &count, 1);
        if (count > 60000)
        {
            count  = 0 ;
            rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
        }else if (count > COUNT_MID * 2) {
            count = COUNT_MID * 2 ;
        }
        rt_kprintf("get count %d\n",count);
        if (count < COUNT_MID) {
            speed = (COUNT_MID - count) * 100  / COUNT_MID  ;
            rt_kprintf("get speed %d\n",speed);
            DRV8833_Forward(speed);
        }else{
            speed = (count  - COUNT_MID )  * 100  / COUNT_MID ;
            rt_kprintf("get speed %d\n",speed);
            DRV8833_Forward(speed);
        }
    }

```

