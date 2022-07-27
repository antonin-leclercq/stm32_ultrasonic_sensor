# stm32_ultrasonic_sensor

## Pin connections:
| Nucleo board pin | HC-SR04 pin |
| ---------------- | ----------- |
| 5V               | Vcc         |
| GND              | GND         |
| PA8              | Echo        |
| PB4              | Trig        |

<b>NOTE</b>: Though most IO pins on STM32 microcontrollers are 5V tolerant, it's safer to add a voltage divider between the `Echo` pin and the `PA8` pin. A 5K / 3.3K divider should do the job just fine and the voltage going to pin `PA8` should not exceed 3V.
