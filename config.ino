#include <EEPROM.h>
#include <DS1307.h>
#include <ChainableLED.h>



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




light_sensor config_l;
temperature_sensor config_t;
humidity_sensor config_h;
pressure_sensor config_p;
other config_o;
int adress;
DS1307 clock;
ChainableLED LED(7, 8, 1);




void RESET() {

  adress = 0;

  config_l.LUMIN = 1;
  config_l.LUMIN_LOW = 255;
  config_l.LUMIN_HIGH = 768;

  EEPROM.put(adress, config_l);
  adress += sizeof(light_sensor);

  config_t.TEMP_AIR = 1;
  config_t.MIN_TEMP_AIR = -10;                // A définir en tant que pointeur
  config_t.MAX_TEMP_AIR = 60;

  EEPROM.put(adress, config_t);
  adress += sizeof(temperature_sensor);
  
  config_h.HYGR = 1;
  config_h.HYGR_MINT = 0;
  config_h.HYGR_MAXT = 50;

  EEPROM.put(adress, config_h);
  adress += sizeof(humidity_sensor);
  
  config_p.PRESSURE = 1;
  config_p.PRESSURE_MIN = 850;
  config_p.PRESSURE_MAX = 1080;

  EEPROM.put(adress, config_p);
  adress += sizeof(pressure_sensor);

  config_o.LOG_INTERVAL = 10;     // En minutes (à convertir en ms dans les fonctions)
  config_o.FILE_MAX_SIZE = 4096;
  config_o.TIMEOUT = 30;

  EEPROM.put(adress, config_o);
  
}





void Configuration() {

  String answer = "";
  while (answer != "Y" || answer != "N") {
    
    Serial.println("Modifier paramètres? (Y/N)");

    delay(500);

    answer = "";
    
    while (answer == "") {

      answer = userMessage();
    }

    if (answer != "Y" && answer != "N") {

       Serial.println("Entrez une réponse valide");
    }

    if (answer == "Y") {
      
      Serial.println("Entrez numéro :");
      Serial.println("1:LUMIN");
      Serial.println("2:LUMIN_LOW");
      Serial.println("3:LUMIN_HIGH");
      Serial.println("4:TEMP_AIR");
      Serial.println("5:MIN_TEMP_AIR");
      Serial.println("6:MAX_TEMP_AIR");
      Serial.println("7:HYGR");
      Serial.println("8:HYGR_MINT");
      Serial.println("9:HYGR_MAXT");
      Serial.println("10:PRESSURE");
      Serial.println("11:PRESSURE_MIN");
      Serial.println("12:PRESSURE_MAX");
      Serial.println("13:LOG_INTERVAL");
      Serial.println("14:FILE_MAX_SIZE");
      Serial.println("15:TIMEOUT");
      Serial.println("");
      Serial.println("16:RESET");
      Serial.println("17:CLOCK");
      Serial.println("18:DATE");
      Serial.println("19:DAY");
      Serial.println("");
      Serial.println("20:EXIT");

      answer = "";

      while (answer == "") {

        answer = userMessage();
        
      }

      if (answer == "1") {

        Serial.println("LUMIN [Défaut:1 ; Plage:{0 - 1}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_l.LUMIN = answer.toInt();
        adress = 0;
        EEPROM.put(adress, config_l);
      }else

      if (answer == "2") {

        Serial.println("LUMIN_LOW [Défaut:255 ; Plage:{0 - 1023}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_l.LUMIN_LOW = answer.toInt();
        adress = 0;
        EEPROM.put(adress, config_l);
      }else

      if (answer == "3") {

        Serial.println("LUMIN_HIGH [Défaut:768 ; Plage:{0 - 1023}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_l.LUMIN_HIGH = answer.toInt();
        adress = 0;
        EEPROM.put(adress, config_l);
      }else

      if (answer == "4") {

        Serial.println("TEMP_AIR [Défaut 1 ; Plage:{0 - 1}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_t.TEMP_AIR = answer.toInt();
        adress = sizeof(light_sensor);
        EEPROM.put(adress, config_t);
      }else

      if (answer == "5") {

        Serial.println("MIN_TEMP_AIR [Défaut:-10 ; Plage:{-40 - 85}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_t.MIN_TEMP_AIR = answer.toInt();
        adress = sizeof(light_sensor);
        EEPROM.put(adress, config_t);
      }else

      if (answer == "6") {

        Serial.println("MAX_TEMP_AIR [Défaut:-10 ; Plage:{-40 - 85}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_t.MAX_TEMP_AIR = answer.toInt();
        adress = sizeof(light_sensor);
        EEPROM.put(adress, config_t);
      }else

      if (answer == "7") {

        Serial.println("HYGR [Défaut:1 ; Plage:{0 - 1}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_h.HYGR = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor);
        EEPROM.put(adress, config_h);
      }else

      if (answer == "8") {

        Serial.println("HYGR_MINT [Défaut:50 ; Plage:{-40 - 85}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_h.HYGR_MINT = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor);
        EEPROM.put(adress, config_h);
      }else

      if (answer == "9") {

        Serial.println("HYGR_MAXT [Défaut:50 ; Plage:{-40 - 85}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_h.HYGR_MAXT = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor);
        EEPROM.put(adress, config_h);
      }else

      if (answer == "10") {

        Serial.println("PRESSURE [Défaut:1 ; Plage:{0 - 1}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_p.PRESSURE = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor) + sizeof(humidity_sensor);
        EEPROM.put(adress, config_p);
      }else

      if (answer == "11") {

        Serial.println("PRESSURE_MIN [Défaut:850 ; Plage:{300 - 1500}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_p.PRESSURE_MIN = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor) + sizeof(humidity_sensor);
        EEPROM.put(adress, config_p);
      }else

      if (answer == "12") {

        Serial.println("PRESSURE_MAX [Ddéfaut:1080 ; Plage:{300 - 1500}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_p.PRESSURE_MAX = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor) + sizeof(humidity_sensor);
        EEPROM.put(adress, config_p);
      }else

      if (answer == "13") {

        Serial.println("LOG_INTERVAL [Défaut:10 ; Plage:{1 - 360}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_o.LOG_INTERVAL = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor) + sizeof(humidity_sensor) + sizeof(pressure_sensor);
        EEPROM.put(adress, config_o);
      }else

      if (answer == "14") {

        Serial.println("FILE_MAX_SIZE [Défaut : 4096 ; Plage : {100 - 32000}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_o.FILE_MAX_SIZE = answer.toInt();
        adress = sizeof(light_sensor) + sizeof(temperature_sensor) + sizeof(humidity_sensor) + sizeof(pressure_sensor);
        EEPROM.put(adress, config_o);
      }else

      if (answer == "15") {

        Serial.println("TIMEOUT [Défaut:30 ; Plage:{5 - 300}]");
        answer = "";

        while (answer == "") {

          answer = userMessage();
        }

        config_o.TIMEOUT = answer.toInt();
      }else

      if (answer == "16") {

        RESET();
      }else

      if (answer == "17") {

        
        CLOCK();
      }else

      if (answer == "18") {

        DATE();
      }else

      if (answer == "19") {

        DAY();
      }else

      if (answer == "20") {

        break;
      }else {

        Serial.println("Invalide");
      }
      
    
    }

    if (answer == "N") {

      break;
    }
  }
  
  Serial.println("retour au standard");
  
}




void CLOCK() {

    String chour = "";
    Serial.println("Heure [0 - 23]");
    while (chour == "")
    {
        chour = userMessage();
    }

    String cminute = "";
    Serial.println("Minutes [0 - 59]");
    while (cminute == "")
    {
        cminute = userMessage();
    }

    String csecond = "";
    Serial.println("Secondes [0 - 59]");
    while (csecond == "")
    {
        csecond = userMessage();
    }
    Serial.print("\n");

    clock.fillByHMS(chour.toInt(), cminute.toInt(), csecond.toInt());
    clock.setTime();
}

void DATE() {

    String cyear = "";
    Serial.println("Année [2000 - 2099]");
    while (cyear == "")
    {
        cyear = userMessage();
    }

    String cmonth = "";
    Serial.println("Mois [1 - 12]");
    while (cmonth == "")
    {
        cmonth = userMessage();
    }

    String cday = "";
    Serial.println("Jour [1 - 31]");
    while (cday == "")
    {
        cday = userMessage();
    }

    clock.fillByYMD(cyear.toInt(), cmonth.toInt(), cday.toInt());
    clock.setTime();
}

void DAY() {

    Serial.println("Entrez le numéro\n1 : Lundi\n2 : Mardi\n3 : Mercredi\n4 : Jeudi\n5 : Vendredi\n6 : Samedi\n7 : Dimanche\n");
    Serial.println("1:Lundi");
    Serial.println("2:Mardi");
    Serial.println("3:Mercredi");
    Serial.println("4:Jeudi");
    Serial.println("5:Vendredi");
    Serial.println("6:Samedi");
    Serial.println("7:Dimanche");
    String cday = "";
    while (cday == "") {

        cday = userMessage();
    }

    switch (cday.toInt())
    {
    case 1:
        clock.fillDayOfWeek(MON);
        clock.setTime();
        break;

    case 2:
        clock.fillDayOfWeek(TUE);
        clock.setTime();
        break;

    case 3:
        clock.fillDayOfWeek(WED);
        clock.setTime();
        break;

    case 4:
        clock.fillDayOfWeek(THU);
        clock.setTime();
        break;

    case 5:
        clock.fillDayOfWeek(FRI);
        clock.setTime();
        break;

    case 6:
        clock.fillDayOfWeek(SAT);
        clock.setTime();
        break;

    case 7:
        clock.fillDayOfWeek(SUN);
        clock.setTime();
        break;                        
    
    default:
        Serial.println("ERROR");
        break;
    }
}




String userMessage() {

  String reponse = "";
  char charac = '0';
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {

      charac = Serial.read();
      reponse += charac;
      delay(2);
    }

    return reponse;
  }

  return "";
}




void ledOrange() {
  LED.setColorRGB(0, 255, 100, 0);
}

void ledVert() {
  LED.setColorRGB(0, 0, 255, 0);
}




void setup() {

  Serial.begin(9600);
  clock.begin();
  LED.init();
  RESET();
  pinMode (3, INPUT);
  delay(500);
  

  if (!digitalRead(3)) {
    ledOrange();
    Configuration();
    
  }
}

void loop() {
  ledVert();
  Serial.println("mode standard");
  delay(1000);
}
