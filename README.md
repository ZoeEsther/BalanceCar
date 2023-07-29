# BalanceCar

The project based on STM32 chip design to achieve a small two-wheel self-balancing car.

The car uses a DC reducer motor with encoder, and adopts the feedback adjustment method, and controls the motor through the motor drive chip TB6612FNG. In order to make the car have a more stable sense of balance, PID regulation is used. In hardware, gyroscope MPU6050 is used to collect the rotation Angle and rotation Angle acceleration of the car body, and acceleration sensor is used to measure the rotation Angle of the car body indirectly. stm32f103c8t6 is used as the main control chip to collect the sensor information for filtering, analyzing and other operations, and then enter the closed-loop feedback adjustment to realize the self-balance of the car.

In addition to the normal driving function, the car can also broadcast the corresponding information by voice to achieve a certain degree of human-computer interaction. In the voice control part, it can drive any distance, in the remote control part, it can accelerate and decelerate the car, and in the gravity sensing part, it can turn left and right at any Angle. In addition, it can also intuitively see the battery power display of the car and the switching of the voice-controlled car OLED screen page on the mobile phone APP.

## Code implementation

The main functions of this part of the code are: program initialization; Read and process sensor data and encoder pulse data; Upright control, speed control, direction control; Calculation output PWM output control motor; Bluetooth remote control, ultrasonic reading, display refresh, voice module playback and other auxiliary functions.

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/ecc27f50-4056-4247-922e-81d42855d43a)

## Final object diagram of a two-wheeled self-balancing car
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/0eb326dc-4a67-4a1b-b4b7-300f8a4131bc)
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/802a2511-be29-4830-9b2f-2ce1ae46d4c4)
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/2955ca89-053e-4c35-b090-ab82da58abfb)


