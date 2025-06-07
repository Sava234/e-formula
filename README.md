# e-formula
a simple electronic machine based on arduino nano, which will move along the QTR-8A sensor and use motors with gearboxes
![изображение](https://github.com/user-attachments/assets/1971b3dd-a46e-4293-9eb4-aef188fa7ecc)
![photo_2025-05-28_18-37-57](https://github.com/user-attachments/assets/9f530898-c2f1-41e3-84dd-847ea5008c6a)

Line-Following Robot on Arduino Nano
This project is a simple electronic robot that follows a black (or white) line using a QTR-8A sensor and two  motors
 Components:
 Arduino Nano
 QTR-8A line sensor (iarduino_I2C_Bumper)
 Two servo motors
 Motor driver (e.g., DRV8833)
 18650 battery
 2 wheels with geared motors
 Voltage regulator (5V converter)
 Micromotor with gearbox 6V 200 RPM
 
  Assembly & Setup:
 Connect all components to the Arduino Nano according to your wiring diagram. Double-check power and GND connections.
 Install required libraries in the Arduino IDE:
 QTR-8A
 
 Upload the code:
 Open Arduino IDE.
 Copy the code into a new sketch.
 Upload it to your Arduino Nano.
 
 Check output:
 Open the Serial Monitor to view sensor data and servo angles.

 Test the robot:
 Place the robot on a line-following track.
 If needed, adjust parameters in the code: lineThreshold, centerAngle, minAngle, maxAngle, steeringSensitivity.

How It Works:
8 infrared sensors detect the line position.
The code calculates the deviation from the center.
The servo motors steer the robot to follow the line accordingly.









































![photo_2025-05-24_16-29-23](https://github.com/user-attachments/assets/90442d29-e759-4aa3-afb2-36a5e168d706) 
![photo_2025-05-14_18-55-42](https://github.com/user-attachments/assets/7fad7c2a-d17f-4973-a6a1-c2c23c02af96)
