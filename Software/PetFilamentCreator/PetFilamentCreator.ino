#include <AccelStepper.h>
#include <LiquidCrystal.h>

//Pines de dirección y de pasos para el controlador (DRV)
const int DIR_PIN = 2;
const int STEP_PIN = 3;

//Botones de control
const int botonArriba = 4;
const int botonAbajo = 5;
const int botonDerecha = 6;
const int botonIzquierda = 7;
const int botonCentro = 8;

//Variable de la lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Crear una instancia de AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Configurar la velocidad máxima y aceleración
  stepper.setMaxSpeed(1000);     // Velocidad máxima en pasos por segundo
  stepper.setAcceleration(500);  // Aceleración en pasos por segundo^2

  // Establecer la velocidad a la que se moverá el motor
  stepper.setSpeed(200);  // Velocidad en pasos por segundo

  //Configuramos la pantalla LCD
  lcd.begin(20, 4);

  //Se definen los valores por defecto de las variables
  int opcionMenu = 0;
  boolean procesoEnMarcha = false;
}

void loop() {
  // Se controla si se accede o no al menú (botonCentro)

  if (digitalRead(botonCentro) == HIGH) {
    //Se ha pulsado el botón del centro, entramos en la configuración
    //Apareceran diferentes opciones, dependiendo de la elegida se realizará una acción u otra.
    lcd.print("Seleccione opción:");

    delay(100);

    while (digitalRead(botonCentro == LOW)) {



      if (digitalRead(botonDerecha)) {

        if (opcionMenu == 2) {
          opcionMenu = 0;
        } else {
          opcionMenu++;
        }

      } else if (digitalRead(botonIzquierda)) {

        if (opcionMenu == 0) {
          opcionMenu = 2;
        } else {
          opcionMenu--;
        }
      }

      switch (opcionMenu) {
        case 0:
          lcd.setCursor(0, 2);
          lcd.print("Ajustar velocidad de");
          lcd.setCursor(0, 3);
          lcd.print("motor");

        case 1:
          lcd.setCursor(0, 2);
          lcd.print("Ajustar temperatura de");
          lcd.setCursor(0, 3);
          lcd.print("hotend");

        case 2:
          lcd.setCursor(0, 2);
          lcd.print("Empezar proceso");
      }
    }
  }

  stepper.runSpeed();
}



//Metodo para limpiar lineas solas de la LCD
void clearLCDLine(int line) {
  lcd.setCursor(0, line);
  for (int n = 0; n < 20; n++) {
    lcd.print(" ");
  }
}
