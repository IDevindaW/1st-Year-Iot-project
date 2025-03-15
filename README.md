This project is a wearable smart navigation system designed to assist visually impaired individuals in detecting obstacles and navigating their environment safely. The system consists of two primary components: a pair of smart glasses and a handheld control unit.

System Components:

Smart Glasses:

Equipped with ultrasonic sensors placed at eye level and lower angles to detect obstacles at different heights.
Designed to be lightweight and comfortable for everyday use.
Communicates with the handheld control unit to provide real-time feedback.

Handheld Control Unit:

Built around an Arduino Nano microcontroller, which processes sensor data and triggers feedback mechanisms.
Uses an IR sensor to detect obstacles at close range, such as low-hanging objects or ground-level obstructions.
Integrates a DFPlayer Mini module for audio feedback, allowing users to receive voice alerts when obstacles are detected.
Includes a vibration motor to provide haptic feedback, ensuring users receive alerts even in noisy environments.
Powered by a 9V battery, regulated by an LM2596S Buck Converter to ensure stable voltage supply for all components.

Functionality & Features:

The ultrasonic sensors continuously scan the surroundings for obstacles.
When an obstacle is detected, the Arduino Nano processes the data and triggers appropriate feedback:
Audio alerts through the DFPlayer Mini.
Vibration feedback via the handheld unit.
The device allows visually impaired users to navigate independently by providing real-time sensory feedback about their environment.

Outcome & Impact:

Successfully enhances mobility and safety for visually impaired individuals.
Provides a cost-effective, lightweight, and user-friendly solution for navigation assistance.
Demonstrates the potential of IoT-based assistive technology in improving accessibility.
