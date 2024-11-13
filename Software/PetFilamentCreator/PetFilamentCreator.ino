#include <AccelStepper.h>
#include <LiquidCrystal.h>

//Pines de dirección y de pasos para el controlador (DRV)
const int DIR_PIN = 2;
const int STEP_PIN = 3;

//Datos para el medidor de temperatura
const int thermistorPin = A0;  // Pin analógico donde está conectado el termistor
const float nominalResistance = 100000;  // Resistencia nominal del termistor a 25°C (100 kΩ)
const float nominalTemperature = 25.0;  // Temperatura nominal del termistor (25°C)
const float bCoefficient = 3950;  // Coeficiente Beta del termistor
const float seriesResistorValue = 6800;  // Resistencia en serie (10 kΩ)

//Botones de control
const int botonArriba = 10;
const int botonAbajo = 11;
const int botonDerecha = 12;
const int botonIzquierda = 13;
const int botonCentro = 2;

//Variable de la lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Variables de control
int opcionMenu;
boolean procesoEnMarcha;
boolean salirMenu;
boolean arrancarProceso;

// Crear una instancia de AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

//Variables propias del programa
float temperaturaObjetivo;
float temperaturaReal;
float velocidadPAP;
float filamentoExtruido;

void setup() {
  // Configurar la velocidad máxima y aceleración
  stepper.setMaxSpeed(1000);     // Velocidad máxima en pasos por segundo
  stepper.setAcceleration(500);  // Aceleración en pasos por segundo^2

  // Establecer la velocidad a la que se moverá el motor
  stepper.setSpeed(200);  // Velocidad en pasos por segundo

  //Configuramos la pantalla LCD
  lcd.begin(20, 4);

  //Se definen los valores por defecto de las variables
  opcionMenu = 0;
  procesoEnMarcha = false;
  temperaturaObjetivo = 0;
  velocidadPAP = 0; //IMPORTANTE, EN RPM
  filamentoExtruido = 0; //IMPORTANTE, EN METROS

}

void loop() {
  int adcValue = analogRead(thermistorPin);  // Lee el valor del ADC
  float resistance = seriesResistorValue / (1023.0 / adcValue - 1);  // Calcula la resistencia del termistor
  
  // Calcula la temperatura usando la ecuación de Steinhart-Hart
  float temperatura;
  temperatura = resistance / nominalResistance;  // (R/Ro)
  temperatura = log(temperatura);  // ln(R/Ro)
  temperatura /= bCoefficient;  // 1/B * ln(R/Ro)
  temperatura += 1.0 / (nominalTemperature + 273.15);  // + (1/To)
  temperatura = 1.0 / temperatura;  // Invertir
  temperatura -= 273.15;  // Convierte de Kelvin a Celsius
  temperaturaReal = temperatura;
  
  //Pintamos la información de la pantalla de inicio
  lcd.clear();
  lcd.println("Temperatura: " + String(temperaturaReal) + "º/" + String(temperaturaObjetivo) + "º");
  lcd.println("Velocidad del PAP: " + String(velocidadPAP));
  lcd.println("Filamento extruido: " + String(filamentoExtruido) + " metros");
  lcd.println("Estado: ");
  
  // Se controla si se accede o no al menú (botonCentro)
  if (digitalRead(botonCentro) == HIGH) {

    if(salirMenu == false){
      salirMenu = true;
    }

    while (salirMenu) {
    //Se ha pulsado el botón del centro, entramos en la configuración
    //Apareceran diferentes opciones, dependiendo de la elegida se realizará una acción u otra.
    printMenu(opcionMenu);

    delay(50);

      if (digitalRead(botonAbajo)) {

        if (opcionMenu == 3) {
          opcionMenu = 0;
        } else {
          opcionMenu++;
        }

      } else if (digitalRead(botonArriba)) {

        if (opcionMenu == 0) {
          opcionMenu = 3;
        } else {
          opcionMenu--;
        }
      }

      if (digitalRead(botonCentro) == HIGH) {
        switch (opcionMenu) {
          case 0: 
            salirMenu = false;
            break;
          case 1:
            configureTemp();
            break;
          case 2:
            configureStepperSpeed();
            break;
          case 3:
            arrancarProceso = true;
            break;
        }
      }

    }
  }

  //En caso de proceso en marcha se arranca este proceso
  if(arrancarProceso){
    procesoEnMarcha = true;
    //Encendemos el hotend y esperamos hasta llegar a temperatura objetivo
    turnOnHotend();
    arrancarProceso = false;
  }

  if(procesoEnMarcha){
    stepper.runSpeed();
  }


  
}

void turnOnHotend(){

}


void configureTemp(){

  lcd.clear();
  while (true) {
    lcd.print("Temperatura objetivo: " + String(temperaturaObjetivo));
    if (digitalRead(botonDerecha) && temperaturaObjetivo < 260) {
      temperaturaObjetivo++;
    }else if (digitalRead(botonIzquierda) && temperaturaObjetivo > 0) {
      temperaturaObjetivo--;
    }else if (digitalRead(botonCentro)) {
      break;
    }
  }

}

void configureStepperSpeed(){
  
  lcd.clear();
  while (true) {
    lcd.print("Velocidad motor: " + String(velocidadPAP));
    if (digitalRead(botonDerecha) && velocidadPAP < 260) {
      velocidadPAP++;
    }else if (digitalRead(botonIzquierda) && velocidadPAP > 0) {
      velocidadPAP--;
    }else if (digitalRead(botonCentro)) {
      break;
    }
  }

}


void printMenu(int arrayPosition){
  if(arrayPosition == 0){
    lcd.println("=> Volver a pantalla de información");
    lcd.println("Configurar temperatura objetivo");
    lcd.println("Configurar velocidad del PAP");
    lcd.println("Iniciar proceso");
  }else if (arrayPosition == 1) {
    lcd.println("Volver a pantalla de información");
    lcd.println("=> Configurar temperatura objetivo");
    lcd.println("Configurar velocidad del PAP");
    lcd.println("Iniciar proceso");
  }else if (arrayPosition == 2) {
    lcd.println("Volver a pantalla de información");
    lcd.println("Configurar temperatura objetivo");
    lcd.println("=>Configurar velocidad del PAP");
    lcd.println("Iniciar proceso");
  }else if(arrayPosition == 3){
    lcd.println("Volver a pantalla de información");
    lcd.println("Configurar temperatura objetivo");
    lcd.println("Configurar velocidad del PAP");
    lcd.println("=>Iniciar proceso");
  }
}


//Metodo para limpiar lineas solas de la LCD
void clearLCDLine(int line) {
  lcd.setCursor(0, line);
  for (int n = 0; n < 20; n++) {
    lcd.print(" ");
  }
}
