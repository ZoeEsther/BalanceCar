# BalanceCar

The project based on STM32 chip design to achieve a small two-wheel self-balancing car.

The car uses a DC reducer motor with encoder, and adopts the feedback adjustment method, and controls the motor through the motor drive chip TB6612FNG. In order to make the car have a more stable sense of balance, PID regulation is used. In hardware, gyroscope MPU6050 is used to collect the rotation Angle and rotation Angle acceleration of the car body, and acceleration sensor is used to measure the rotation Angle of the car body indirectly. stm32f103c8t6 is used as the main control chip to collect the sensor information for filtering, analyzing and other operations, and then enter the closed-loop feedback adjustment to realize the self-balance of the car.

In addition to the normal driving function, the car can also broadcast the corresponding information by voice to achieve a certain degree of human-computer interaction. In the voice control part, it can drive any distance, in the remote control part, it can accelerate and decelerate the car, and in the gravity sensing part, it can turn left and right at any Angle. In addition, it can also intuitively see the battery power display of the car and the switching of the voice-controlled car OLED screen page on the mobile phone APP.

## Code implementation

The main functions of this part of the code are: program initialization; Read and process sensor data and encoder pulse data; Upright control, speed control, direction control; Calculation output PWM output control motor; Bluetooth remote control, ultrasonic reading, display refresh, voice module playback and other auxiliary functions.

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/ecc27f50-4056-4247-922e-81d42855d43a)

After the car is powered on, the microcontroller starts to initialize: each application module is initialized; Initialize the control program, that is, initialize the variable value applied to the car control program. After the initialization, the car enters the vertical balance detection subprogram, which determines whether the car body is in the controllable Angle range. If it is in the controllable range, the car body upright control, speed control, direction control and motor output will be started immediately. If it is not under control, wait until the condition is met.

The upright control, speed control, direction control and motor output of the car are all completed in the timed interrupt program. These closed-loop controls are determined by global flag variables. Lift recognition, landing recognition and fall recognition functions are also placed in the interrupt, once the car is out of control, the motor output is stopped until it meets the upright condition again.
### MPU6050

The Angle information obtained by the acceleration sensor and gyroscope needs to be integrated, but if there is a small deviation in the angular velocity, a large error will be accumulated after integration, and accurate Angle data can be obtained through the data fusion algorithm. In this project, we adopt complementary filtering, the core formula of complementary filtering is:

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/28f07e1a-796c-4959-b95e-42ffd42911e1)

Among them, "Acc" represents: the Angle obtained by the arctangent function of the acceleration sensor part; "Gyro" indicates the angular velocity obtained by the gyroscope part.

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/f67b0eef-d9f0-4deb-9ba3-414b4bf558e5)

### PID control of vertical balance Angle ring

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/d263549c-c24f-40fc-a262-4a60c356eb6b)

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/cf41a7c9-3500-40a3-bcc4-c8f55c82d646)

"g_fCarAngle indicates" the car Angle. "g_fGyroAngleSpeed" indicates the angular speed of the car, which is the value of the gyroscope. "CAR_ANGLE_SET" and "GAR_ANGLE_SPEED_SET" represent the expected Angle value and the expected angular velocity value of the car respectively. "g_tCarAnglePID.P" and "g_tCarAnglePID.D" are the Angle ring P and D parameters.
In the upright balance Angle loop PID control, we choose PD control because there will inevitably be noise signals in the attitude detection signal of the two-wheeled self-balancing car, which will continuously accumulate over time through the integration I link, resulting in the integrator losing the adjustment function of eliminating static errors and generating control errors.

### Closed-loop control of motion speed

![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/32652a6d-6105-4e1a-b613-55399fc17c7f)

In the speed ring, unlike the vertical ring, PI control is used. And the speed control here is positive feedback, that is, when the car is running at a certain speed, if you want to stop the car, the car needs to travel faster to the speed direction, if it is negative feedback, the car will inertial fall.

After the completion of upright ring balance and velocity ring balance, the car needs smooth speed output. Because the PWM change of speed if the output is immediately after the calculation of 25ms(speed loop control period is 25ms), it will cause non-smooth jitter, etc., so the design will allocate the PWM obtained by one cycle to 5 5ms time to output, smoothly and gradually approach the final calculated value of the output.

### Voice broadcast
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/126a676b-b123-4369-bbf5-00502c36d549)

STM32 and SYN6288 voice synthesis chip are connected through UART2 interface. STM32 can send control commands and text to SYN6288 voice synthesis chip through communication interface. SYN6288 voice synthesis chip synthesizes received text into voice signal output. The output signal is amplified by the power amplifier and then connected to the speaker for playback.

## Final object diagram of a two-wheeled self-balancing car
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/0eb326dc-4a67-4a1b-b4b7-300f8a4131bc)
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/802a2511-be29-4830-9b2f-2ce1ae46d4c4)
![image](https://github.com/ZoeEsther/BalanceCar/assets/119051069/2955ca89-053e-4c35-b090-ab82da58abfb)


