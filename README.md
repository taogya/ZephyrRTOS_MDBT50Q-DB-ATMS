# ZephyrRTOS_MDBT50Q-DB-ATMS
MDBT50Q-DB-ATMS Board for Zephyr RTOS<br>
Please copy [board](boards/raytac/mdbt50q_db_atms) to `/path/to/your/zephyrproject/zephyr/boards/raytac`.<br>


> [!WARNING]
> The use of AT commands will become unavailable.<br>

## PINOUT
| md[^1] pin no | md pin name | db[^2] symbol | db label          | function                                |
| ------------- | ----------- | ------------- | ----------------- | --------------------------------------- |
| 26            | P0.15       | D1            | Connect/ADV LED   | Red LED (Low Active)                    |
| 25            | P0.13       | D2            | Indicater         | Green LED (Low Active)                  |
| 8             | P0.31       | SW1           | Wakeup            | Push is Low.                            |
| 9             | P0.30       | R30           | ADC               | 180k / (820k + 180k) x (J11 3.3V input) |
| 15            | P0.06       | J10 (2 pin)   | UART CTS/SPIS CSN |                                         |
| 17            | P1.09       | J10 (3 pin)   | UART TX/SPIS MISO |                                         |
| 16            | P0.08       | J10 (4 pin)   | UART RX/SPIS MOSI |                                         |
| 14            | P0.04       | J10 (5 pin)   | UART RTS/SPIS CLK |                                         |
| 5             | P1.05       | J11 (1 pin)   | SPI               |                                         |
| 6             | P0.19       | J11 (2 pin)   | USB               |                                         |
| 7             | P0.29       | J11 (6 pin)   | Flash default     |                                         |
| 8             | P0.31       | J11 (7 pin)   | Wakeup            |                                         |
| 11            | P0.27       | J11 (8 pin)   | UART PD           |                                         |
| 27            | P0.18       | J11 (9 pin)   | RESET             |                                         |
| 25            | P0.13       | J11 (10 pin)  | Indicater         |                                         |
| 31            | P0.24       | J12 (2 pin)   | P0.24/SPI_INT     |                                         |
| 30            | P0.22       | J12 (4 pin)   | P0.22/Full        |                                         |
| 29            | P0.20       | J12 (6 pin)   | P0.20/RW          |                                         |
| 28            | P0.17       | J12 (8 pin)   | P0.17/DRDY        |                                         |
| 33            | SWDCLK      | J9 (3 pin)    | SWCLK             |                                         |
| 32            | SWDIO       | J9 (4 pin)    | SWDIO             |                                         |
| 33            | SWDCLK      | J1 (4 pin)    | SWCLK             | J-Link                                  |
| 32            | SWDIO       | J1 (2 pin)    | SWDIO             | J-Link                                  |

[^1]: MDBT50Q-ATMS module<br>
[^2]: MDBT50Q-DB-ATMS (MDBT50Q-ATMS DK Board)<br>

## Sample
basic/thread app based on the zephyrproject.<br>
cf. <br>
    https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-6-analog-to-digital-converter-adc/topic/exercise-1-interfacing-with-adc-using-zephyr-api/ <br>
    https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/basic/threads <br>
