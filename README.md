# ZephyrRTOS_MDBT50Q-DB-ATMS
MDBT50Q-DB-ATMS Board for Zephyr RTOS<br>

> [!WARNING]
> The use of AT commands will become unavailable.<br>

## PINOUT
| pin no | pin name | pin label       | function                          | note          |
| ------ | -------- | --------------- | --------------------------------- | ------------- |
| 26     | P0.15    | Connect/ADV LED | Low Active output                 | D1(Red Led)   |
| 25     | P0.13    | Indicater       | Low Active output                 | D2(Green Led) |
| 8      | P0.31    | Wakeup          | Low Active input                  | SW1           |
| 9      | P0.31    | ADC             | 820k / (820k + 180k) x 3.3V input |               |

## Sample
basic/thread app based on the zephyrproject.<br>
cf. <br>
    https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-6-analog-to-digital-converter-adc/topic/exercise-1-interfacing-with-adc-using-zephyr-api/ <br>
    https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/basic/threads <br>