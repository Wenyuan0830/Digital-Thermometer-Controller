# Digital-Thermometer-Controller
Basic Function Description:
Digital thermometer has three buttons. Two of them are used to adjust the desired temperature. If users want to increase the desired temperature, they are supposed to press the left button. And if users want to decrease the desired temperature, they are supposed to press the middle button.
Besides, there is an LCD screen to present all the information. The format of the information is the first line is ii.i°C aa ee.e°C, and the second line is dd°C ccc, where ii.i reperesents internal temperature, aa represents the character arrow to indicate which temperature is used for control, ee.e represents the external temperature, dd represents user desired temperature and ccc indicates whether the furnace is on or off.
The function of the digital thermometer is firstly selecting which temperature is the compare temperature. Then if the temperature is higher than the desired temperature or in the deadband, the furnace would not be turned on. If the temperature is lower than the desired temperature or in the deadband, the furnace would be turned on.

Operating Conditions:
1. Using 5V to supply the power.
2. ATmega328P U should be operated at range of -40 to 85 degree Celsius and buttons should be operated between -25 to 70 degree Celsius. LCD should be used on 25 degree Celsius.
    Note: Exceed voltage would damage the product.
    
Operating Instruction:
1. Button from left to right represent decrease temperature,increase temperature and toggle compared temperature.
2. All the information would be presented on the LCD screen.

Results of Testing:
1. Testing temperature:room temperature which is 25 degree Celsius. 
2. Testing voltage and current:5V and 0.1A
3. Testing results:

Pressing the first button:
The desired temperature is increasing until it reaches the maximum(35 °C),and the mode of the furnace is OFF all the time.

Pressing the second button:
The desired temperature is decreasing until it reaches the minimum(10 °C), and the mode of the furnace became ON when the compared temperature is lower than the desired temperature.

Pressing the third button:
The compared temperature toggled, which can be seen from the direction of the arrow.

