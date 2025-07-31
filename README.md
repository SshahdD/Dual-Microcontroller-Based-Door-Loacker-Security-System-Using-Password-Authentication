# Dual-Microcontroller-Based-Door-Loacker-Security-System-Using-Password-Authentication
This project is a password-protected embedded system designed to control door access using an LCD, keypad, PIR sensor, motor, and microcontroller (ATmega32). ​ It follows a layered model architecture with two main components: HMI_ECU (Human-Machine Interface) and Control_ECU. ​ The system operates at a frequency of 8 MHz and utilizes EEPROM for password storage. ​
Key Features:

Password Creation: Users create a 5-digit password, confirmed by re-entering it. ​ The password is sent to Control_ECU via UART and stored in EEPROM if both entries match. ​
Main Options: The LCD displays options to open the door or change the password. ​
Door Operation:

Users enter the saved password to unlock the door. ​ If matched, the motor rotates clockwise for 15 seconds, unlocking the door. ​ The PIR sensor detects motion and holds the motor until people enter. ​
Once no motion is detected, the motor rotates counterclockwise for 15 seconds, locking the door. ​


Password Change: Users can change the password by entering the current password and repeating the password creation process.
Error Handling: If passwords do not match during door operation or password change, the system prompts the user to re-enter the password up to three times.

System Architecture:

HMI_ECU: Handles user interaction via LCD and keypad.
Control_ECU: Manages motor, buzzer, PIR sensor, and EEPROM for password validation and storage. ​

Technologies:

Microcontroller: ATmega32 ​
Communication: UART ​
Storage: EEPROM ​
Sensors: PIR for motion detection ​
Actuators: Motor for door locking/unlocking ​

This project can be implemented as a GitHub repository with the following structure:

Code: Firmware for HMI_ECU and Control_ECU.
Documentation: System requirements, operation steps, and architecture details. ​
Hardware: Circuit diagrams and component specifications.
