## Measuring evaporation rate on a modified Oskar Big humidifier by Stadler Form without filters

Oskar Big is an evaporative humidifier. It works by soaking up water into some filters and then a fan (in the top part of the case) sucks air out. It's pretty simple and quite effective. The issue is that the filters turn disgusting pretty quickly - can we use Oskar without filters? 
![enter image description here](https://www.stadlerform.ru/upload/sotbit.htmleditoraddition/0eb/PYL_NA_FILTRAKH_OSKAR_1000_.jpg)

Without the filters it's basically just a water container with fans and it doesn't work well. So we need a way to speed up the rate of evaporation. Adding a small aquarium pump and water heater does the trick: 
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230217_201415.jpg)
The pump moves water around and the heater goes up to 40 degrees. Having a water heater can be a fire hazard when there is no water left. And we are using considerably more energy but hopefully that's offset by the price of buying new filters every two months. 

Oskar has a water level indicator inconveniently placed the side of the panel. But I needed a more visible indicator to alert when water level is low. Especially now when we have a pump and heater that could break if there is no water left. 

The circuit diagram:
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/project_hum_schema.png)

We use an ultrasonic sensor to measure distance from the top of the container to the water level. As the water level goes down, distance increases. If we subtract the total height of the container without water we can get the real water level. For a more accurate reading we make the calculation by taking into account the temperature and humidity inside the container (which significantly impacts the speed of sound). 

Breadboard wiring:
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230216_200321.jpg)


Perfboard/prototype (front and back - soldering not the best lol but it works):

![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230217_214229.jpg)
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230217_214234.jpg)

display module:

![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230217_205012.jpg)

Fitting it inside the humidifier:
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230217_224531.jpg)

final product:
![enter image description here](https://raw.githubusercontent.com/jonathanlal/project_hum/master/20230218_012640.jpg)

The display has two different views, which can be toggled by the button. The first display shows temp, humidity and water level (in cm) and the second view shows the remaining water level, hours remaining and the rate of evaporation (cm/h). When water is too low the red light turns on, the display changes to a warning view and a sound plays. 


