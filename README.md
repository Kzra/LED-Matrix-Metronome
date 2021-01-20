# LED-Matrix-Metronome
## Using Arduino to help you stay in time!

I'm an Arduino fan and a musician. Practicing with a metronome is a great way to improve your musicianship, but most metronome apps are a little boring and don't have an engaging visual component. I'm a visual person so wanted to use Arduino and an LED matrix to make practicing more engaging. 
The project is very simple. There is an LED Matrix that displays patterns depending on the time signature.  For 4/4 it lights up in a square, for 3/4 it displays a triangle and for 6/8 time it displays a rectangle and two squares. The piezo buzzer beeps every beat. The bpm can be changed by turning the potentiometer, and the time signature can be changed by pressing the switch. 

![Metronome image](https://github.com/Kzra/LED-Matrix-Metronome/blob/main/image_2021-01-20_135442.png)

## How to make the metronome
Assemble the components, wire the circuit as shown on the breadboard schematic. Download the code from the link below and open it in the Arduino IDE. Check that your circuit wiring is correct and that you have the relevant libraries installed and then upload the code onto your Arduino. 
If you want to practice in other time signatures, the code can be tailored to make new patterns. You can also change the code of the existing patterns to make them a little more interesting.

[Metronome in action (video)](https://www.youtube.com/watch?v=7WuJovUj4kk&ab_channel=Afrotechmods)

[Link to full tutorial](https://create.arduino.cc/projecthub/kzra/led-matrix-metronome-767e3c)
