// #include "gpio.h"

// uint8_t key_scan(void)
// {
//   static uint8_t key_flag;
//   uint8_t ret = 0;
//   keyx_check(G, 0, &key_flag, &ret);
//   keyx_check(G, 1, &key_flag, &ret);
// }

// void keyx_check(GPIO_PinState sta, uint8_t num, uint8_t *flag, uint8_t *result) {
//   if (sta == GPIO_Pin_RESET) {
//     //按键按下
//     *flag |= 1 << num;
//     key_cnt[num]++;
//   } else {
//     if (*flag & (1 << num)) 
//     // 处理短按
//     {
//       *flag &= ~(1 << num);  // 清空标志位
//       if (key_cnt[num] < LONG_CNT)
//       {
//         *result = num + 1;
//       } 
//       else 
//       {
//         key_cnt[num] = 0;
//       }

//     }
//   }
//   if(key_cnt[num] > LONG_CNT)
//   // 处理长按
//   {
//     *result = num + 1;
//   }
// }
