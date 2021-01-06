# StationMeteo

Ce code en Arduino permet d'utiliser un capteur de température, de pression, d'humidité, de luminosité et l'heure et d'enregistrer ces données dans un fichier .txt sur une carte SD.
Il y a 4 modes : 
- mode standard : quand vous allumez la carte arduino, il permet de récupérer les données des capteurs toutes les x minutes
- mode eco : quand vous appuyez 5 secondes sur le bouton vert pendant que l'arduino est en mode standard, il permet d'augmenter le temps entre 2 mesures (2*x minutes)
- mode maintenance : quand vous appuyez 5 secondes sur le bouton bleu pendant que l'arduino est en mode standard ou eco, il permet de retirer la carte sd en toute sécurité.
- mode config : quand vous appuyez sur le bouton rouge pendant qu'on allume l'arduino, il permet de modifier certain réglage (temps entre 2 mesures, lumière min et max, ...)
Ce code a été créé par https://github.com/BaptisteBurguiere, https://github.com/ByJfMarie et https://github.com/FranciscoDF.


This Arduino code allows to use a temperature, pressure, humidity, luminosity captor and a clock and save this datas in a .txt file on an SD card.
There are 4 modes :
- standard mode : when you turn on the Arduino board, it allows to collect the datas from the sensor in an interval of x minutes.
- eco mode : when you push 5 secondes on the green button while the Arduino is in standard mode, it allows 
