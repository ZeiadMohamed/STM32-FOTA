FOTA for STM32F103




📚 Project Overview:

FOTA ( firmware over the air ) , is a feature that  allows flashing the Microcontroller remotely over the air. 
I applied it on STM32F103C8T6 MCU. 


📝 Project Structure :

The Project is divided into three main parts:
1- The Website :- allow the user to upload code (.hex) file and flash it remotely on the Microcontroller.
		- It's implemented using html,css,php .
		- I hosted it locally because Free Hosting is not stable
2- The ESP8266 ESP-01 : a Wi-Fi module that allows microcontrollers access to a Wi-Fi network to receive the uploaded code line by line.
3- STM32F103C8T6 : - flash the uploaded code into app area in flash memory and wait for 5 secs then jump to the application.
		   - if no code received it waits for 5 secs and jumps to the latest flashed application

📝 Project Testing :
I tested the project with uploading an application which blink a LED every 500 ms


👉 Hardware Components:

-------------------------------

🔴 STM32 BlueBill.

🔵 ESP8266 ESP-01.

⚫ LEDS.





👉Implemented drivers:

-------------------------------

🔴 DIO - STK - FPEC - RCC - USART - ESP8266 .






