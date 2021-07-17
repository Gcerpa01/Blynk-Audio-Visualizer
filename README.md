# AudioProject

# General Information
This project focuses on reading audio frequencies and utilizing the information read in order to create an audio visualizer. This is a project that has been in the making for a year as an introduction to Arduino and ESP in my free time. A timeline of my improvements and steps in this journey can be found [in this twitter thread](https://twitter.com/PunaticGerry/status/1292268597901811712?s=20) and **further information will be provided below**.


## Inspired and utilized Resouces
 During the development of this project I went through multiple stages, having been influenced by multiple YouTube videos I found online. The first two having been [DIY LED Music Visualizer | Real-Time Animations (Arduino)](https://youtu.be/lU1GVVU9gLU) that focused on utilizing the LMX31 Voltage to Frequency converter, and a circuit he found online, in addition to [Making Wireless MUSIC VISUALIZING Floor Lamps](https://youtu.be/yninmUrl4C0) in which a Wemos WiFi Board is taking input utlizing a microphone and creating a server for another Wemos WiFi Board to receive the information to control LED Lights.
 In the beginning of the project I started by utlizing a microphone to read audio levels to turn on the WS2812 Leds, however after awhile I found about the *MSGEQ7* a 7 band graphic equalizer, that splits the audio input that is read through an analog aux jack into seven different frequencies. From here I used these following circuits to utilize the MSGEQ7.
 
 ![MBED](http://developer.mbed.org/media/uploads/chrisisthefish/equalizerschematic.png)
 
 ---
 
 ![Reddit](https://external-preview.redd.it/k60hzDqywt11Vxf9Lf1IaOPm1nax_M0aXfSNiSU6Z-4.jpg?auto=webp&s=7c847391825ed8cf634db3ca683357c1acca59fb)
 
 ---
