# AudioProject

# General Information
This project focuses on reading audio frequencies and utilizing the information read in order to create an audio visualizer wirelessly. This is a project that has been in the making for a year as an introduction to Arduino and ESP in my free time. A timeline of my improvements and steps in this journey can be found [in this twitter thread](https://twitter.com/PunaticGerry/status/1292268597901811712?s=20) and **further information will be provided below**.

 ***Videos of the final product can also be found in the twitter thread ***


## Steps and Resources
 During the development of this project I went through multiple stages, having been influenced by multiple YouTube videos I found online. The first two having been [DIY LED Music Visualizer | Real-Time Animations (Arduino)](https://youtu.be/lU1GVVU9gLU) that focused on utilizing the LMX31 Voltage to Frequency converter, and a circuit he found online, in addition to [Making Wireless MUSIC VISUALIZING Floor Lamps](https://youtu.be/yninmUrl4C0) in which a Wemos WiFi Board is taking input utlizing a microphone and creating a server for another Wemos WiFi Board to receive the information to control LED Lights.
 In the beginning of the project I started by utlizing a microphone to read audio levels to turn on the WS2812 Leds, however after awhile I found about the *MSGEQ7* a 7 band graphic equalizer, that splits the audio input that is read through an analog aux jack into seven different frequencies. From here I used these following circuits to utilize the MSGEQ7.
 
 ![MBED](http://developer.mbed.org/media/uploads/chrisisthefish/equalizerschematic.png)
 
 ---
 
 ![Reddit](https://external-preview.redd.it/k60hzDqywt11Vxf9Lf1IaOPm1nax_M0aXfSNiSU6Z-4.jpg?auto=webp&s=7c847391825ed8cf634db3ca683357c1acca59fb)
 
 ---
 
  After having followed the schematics and getting them to work, I wrote the *visuaulizer* file to display the audio spectrum through a local arduino and began an attempt to create a server and a client using two *ESP8266 boards* to send the information from the input to a server that would light up the lights. After some failed attempts I found out about the *M28 Bluetooth Audio Reciever Module* and implemented it into my circuit as well as the Blynk framework to control the lights wirelessly. Additionally, taking into consideration that my WiFi would perhaps change in the future and wanting to avoid it being hard coded, I followed the tutorial [Enter Blynk Credentials Over the Air in ESP32 or NodeMCU board | ESP32 projects | ESP8266 projects](https://youtu.be/cjGBlEVPGBI) in order to enter the credentials over the air
  
  **Final Schematic**
  ![Untitled_Artwork](https://user-images.githubusercontent.com/81593072/126049060-28d45e6e-e0b2-4aae-a0c5-0ec2d1f44913.png)
  
#### Caveats
   The main issue with this project overall is that with the implemented code it will *require* a connection to WiFi network in order to function at all. Additionally, when being turned on after having already been turned on before, the software my require for the visualizer selection to be selected again in order to fully initiate the desired mode. Additionally, I have decided to leave my server and client attempts here as show of my steps. I have written an improved version of them in which the audio frequency values were being transferred over to the server, however I have refrained from providing the code as I want to modify it to utilize an ESP32 board as the information was being sent at a slow pace utilizing the ESP8266 Board.
   
#### Final Thoughts
   This was a fun first project to have done as an introduction to Arduinos and ESP boards. The completed software can be found in the **blynk** folder containing all of the code I wrote myself as well as the provided code I found with some online examples(credit is given within code for code utilized). This may be a project I improve on in the future and update here, but overall I have gotten a further understanding of circuits, voltage drops, current, classes as well as how servers and clients work.
   
