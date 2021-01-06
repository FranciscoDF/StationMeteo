#include <DS1307.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <ChainableLED.h>
#include <EEPROM.h>


#define PUSHPINV 2 // Bouton vert sur le PIN 2
#define PUSHPINR 3 // Bouton rouge sur le PIN 3



typedef struct BME BME;
struct BME
{
    String temp;
    String hum;
    String pres;
};

typedef struct light_sensor light_sensor; // Définition des valeurs par défaut dans le config .c
struct light_sensor{
    bool LUMIN;
    int LUMIN_LOW;
    int LUMIN_HIGH;

};

typedef struct temperature_sensor // Définition des valeurs par défaut dans le config .c
{
    bool TEMP_AIR;
    int MIN_TEMP_AIR;
    int MAX_TEMP_AIR;

}temperature_sensor;

typedef struct humidity_sensor // Définition des valeurs par défaut dans le config .c
{
    bool HYGR;
    int HYGR_MINT;
    int HYGR_MAXT;

}humidity_sensor;

typedef struct pressure_sensor // Définition des valeurs par défaut dans le config .c
{
    bool PRESSURE;
    int PRESSURE_MIN;
    int PRESSURE_MAX;
}pressure_sensor;

typedef struct other other;
struct other
{
    int LOG_INTERVAL;
    int FILE_MAX_SIZE;
    int TIMEOUT;
};






ChainableLED LED(7, 8, 1);
BME280I2C bme;
DS1307 clock;
int mode = 0;
int prevMode;
int prevMode2;
BME bme_value;
light_sensor config_l;
temperature_sensor config_t;
humidity_sensor config_h;
pressure_sensor config_p;
other config_o;
int adress;





// Interruption du bouton vert
void init_inter_v(){
    attachInterrupt(digitalPinToInterrupt(PUSHPINV), switchStandardEco, CHANGE);
}

// Interruption du bouton rouge
void init_inter_r(){
    attachInterrupt(digitalPinToInterrupt(PUSHPINR), switchMaintenance, CHANGE);
}

// Désactivation de l'interruption du bouton vert
void disable_inter_v() {
    detachInterrupt(digitalPinToInterrupt(PUSHPINV));
}

// Désactivation de l'interruption du bouton rouge
void disable_inter_r() {
    detachInterrupt(digitalPinToInterrupt(PUSHPINR));
}

// permet de passer du mode éco au mode standard et vice versa
void switchStandardEco() {

    static bool bascule = 0;
    static unsigned long t1;
    unsigned long t2;

    if (bascule == 0) {

        t1 = millis();
    } else {

        t2 = millis();

        if (t2 - t1 > 2000) {

            if (mode == 0) {

                mode = 1;
            }else {

                mode = 0;
            }
            // LED clignote pour signifier changement de mode
            ledRouge();

        }
    }

    bascule = !bascule;
}

// Permet de passer du mode en mode maintenance et retenir le mode précédent
void switchMaintenance() {

    static bool bascule = 0;
    static unsigned long t1;
    unsigned long t2;

    if (bascule == 0) {

        t1 = millis();
    } else {

        t2 = millis();

        if (t2 - t1 > 2000) {

            if (mode == 2) {
              
              mode = prevMode;   
            }else {

              prevMode = mode;
              mode = 2;
            }
            // LED clignote pour signifier changement de mode
            ledRouge();
        }
    }

    bascule = !bascule;
}






void ledBleu() {
  LED.setColorRGB(0, 0, 0, 255);
}

void ledOrange() {
  LED.setColorRGB(0, 255, 100, 0);
}

void ledVert() {
  LED.setColorRGB(0, 0, 255, 0);
}

void ledJaune() {
  LED.setColorRGB(0, 255, 255, 0);
}

void ledRouge() {
    LED.setColorRGB(0, 255, 0, 0);
}

void ledBlanc() {
  LED.setColorRGB(0, 255, 255, 255);
}

void ledErreurRTC() {
  ledRouge();
  delay(1000);
  ledBleu();
  delay(1000);
}

void ledErreurCapteur() {
  ledRouge();
  delay(1000);
  ledVert();
  delay(1000);
}

void ledErreurDonneeCapteur() {
  ledRouge();
  delay(1000);
  ledVert();
  delay(2000);
}

void ledErreurSDPlein() {
  ledRouge();
  delay(1000);
  ledBlanc();
  delay(1000);
}

void ledErreurSD() {
  ledRouge();
  delay(1000);
  ledBlanc();
  delay(2000);
}









void recup_values(int mode) {

    int x;
    if (mode == 0 || mode == 2) {

      x = 1;
    }else {

      x = 2;
    }

    prevMode2 = mode;

    while (mode == prevMode2) {

        capteur_bme();

        if (mode == 0 || mode == 1) {

          save_write_values(capteur_lumin(), bme_value.temp, bme_value.pres, bme_value.hum);
        }else {

          save_write_values_maintenance(capteur_lumin(), bme_value.temp, bme_value.pres, bme_value.hum);
        }

    delay(1000*x);
    }
}




void save_write_values(String lumin, String temp, String pres, String hum) {

    int revision = 0;
    unsigned long taille = 0;
    int size_max = 4096; // a voir comment on récup
    prevMode2 = mode;

    while (mode == prevMode2) {

        clock.getTime();
        String date = String(clock.year) + String(clock.month) + String(clock.dayOfMonth);
        String nom = date + "_" + revision + ".txt";

        while (!SD.begin(4)) {
          ledErreurSD();
        }

        File fichier = SD.open(nom, FILE_WRITE);

        while (true) {

            taille = fichier.size();

            if (taille + 100 > size_max) {

                revision++;
                break;
            }

            clock.getTime();
            String data = String(clock.hour) + ":" + String(clock.minute) + ":" + String(clock.second) + " | Lum=" + lumin + " Temp=" + temp + " Hum=" + hum + " Pres=" + pres;   
            fichier.println(data);

            fichier.close();
            
            break;
            
       } 

        if (taille + 100 < size_max) {
            break;
        }
    }

}





void save_write_values_maintenance(String lumin, String temp, String pres, String hum){

    prevMode2 = mode;
    while (mode == prevMode2) {
      
      clock.getTime();

      String data = String(clock.hour) + ":" + String(clock.minute) + ":" + String(clock.second) + " | Lum=" + lumin + " Temp=" + temp + " Hum=" + hum + " Pres=" + pres;
      Serial.println(data);

      break;
    }

    

}





String capteur_lumin() {

    float value;

    if (config_l.LUMIN)
    {

        value = analogRead(0);

        if (config_l.LUMIN_LOW <= value && value <= config_l.LUMIN_HIGH)
        {
          return "Moyen";
        }

        if (value < config_l.LUMIN_LOW) {

          return "Faible";
        }

        if (value > config_l.LUMIN_HIGH) {

          return "Fort";
        }
        
        
    }
    return "DIS";
}







void capteur_bme(){

   if (!config_t.TEMP_AIR && !config_h.HYGR && !config_p.PRESSURE) {

    bme_value.temp = "DIS";
    bme_value.hum = "DIS";
    bme_value.pres = "DIS";
   }else {

    while (!bme.begin()) {

      ledErreurCapteur();
    }

     float temp(NAN), hum(NAN), pres(NAN);

     BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
     BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  
     bme.read(pres, temp, hum, tempUnit, presUnit);
     pres = pres*0.01;
  
     if (!config_p.PRESSURE) {
      
      bme_value.pres = "DIS";
     }else 
     
     if (config_p.PRESSURE_MIN <= pres && pres <= PRESSURE_MAX) {
     
         bme_value.pres = String(pres);
     }else {
     
         bme_value.pres = "ERROR";
     }

     if (!config_t.TEMP_AIR) {

      bme_value.temp = "DIS";
     }else
  
     if (MIN_TEMP_AIR <= temp && temp <= MAX_TEMP_AIR) {
     
         bme_value.temp = String(temp);
     }else {
     
         bme_value.temp = "ERROR";
     }

     if (!config_h.HYGR) {

      bme_value.hum = "DIS";
     }else
  
     if (HYGR_MINT <= hum && hum <= HYGR_MAXT) {
     
         bme_value.hum = String(hum);
     }else {
     
         bme_value.hum = "ERROR";

     }
   }
   

}





void INIT() {

  adress = 0;
  EEPROM.get(adress, config_l);

  adress += sizeof(light_sensor);
  EEPROM.get(adress, config_t);

  adress += sizeof(temperature_sensor);
  EEPROM.get(adress, config_h);

  adress += sizeof(humidity_sensor);
  EEPROM.get(adress, config_p);

  adress += sizeof(pressure_sensor);
  EEPROM.get(adress, config_o);
}












void setup() {

  Serial.begin(9600);
  pinMode(PUSHPINR, INPUT);
  
  clock.begin();
  LED.init();

  delay(500);

  if (!digitalRead(PUSHPINR)) {

    ledOrange();
    Serial.println("mode config");
    delay(5000);
  }

  Wire.begin();

  init_inter_v();
  init_inter_r();

  INIT();
}

void loop() {

  if (mode == 0) {

    ledVert();
    init_inter_v();
    recup_values(mode);
  }

  if (mode == 1) {

    ledBleu();
    init_inter_v();
    recup_values(mode);
  }

  if (mode == 2) {

    ledJaune();
    disable_inter_v();
    recup_values(mode);
  }


}
