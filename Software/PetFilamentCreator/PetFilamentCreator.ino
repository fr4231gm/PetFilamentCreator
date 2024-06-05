#include <AccelStepper.h>

//Pines de dirección y de pasos para el controlador (DRV)
const int DIR_PIN = 2;
const int STEP_PIN = 3;

//Botones de control
const int botonArriba = 4;
const int botonAbajo = 5;
const int botonDerehca = 6;
const int botonIzquierda = 7;
const int botonCentro = 8;

// Crear una instancia de AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Configurar la velocidad máxima y aceleración
  stepper.setMaxSpeed(1000); // Velocidad máxima en pasos por segundo
  stepper.setAcceleration(500); // Aceleración en pasos por segundo^2

  // Establecer la velocidad a la que se moverá el motor
  stepper.setSpeed(200); // Velocidad en pasos por segundo
}

void loop() {
  // Se controla si se accede o no al menú (botonCentro)

  if(digitalRead(botonCentro) == HIGH){
    //Se ha pulsado el botón del centro, entramos en la configuración
    //Apareceran diferentes opciones, dependiendo de la elegida se realizará una acción u otra.

    
  }

  stepper.runSpeed();
}
