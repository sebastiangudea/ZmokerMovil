#include <Event.h>
#include <Timer.h>
#include <Servo.h>

int IN3 = 8;    // Input3 conectada al pin 5
int IN4 = 4;    // Input4 conectada al pin 4 
int ENB = 3;    // ENB conectada al pin 3 de Arduino
int pwm = 0;    //se almacena el valor de la aceleración
int luces = 7; //Luces delanteras
int rojas = 10; //Luces de STOP
Servo servo; //Direccion
int pito = A5; //Pito
int dirDer = 12; //Direccionales derechas
int dirIzq = 13; //Direccionales izquierdas
int rojo = 0, azul = 0, verde = 0; //Luces RGB
int bluetoothEnabled = 0; //0 = 2, 1 = 2 equivalencias en pines
boolean dirDer1 = false, dirIzq1 = false, reversa = false, parqueo = false; //Intermitencia de las luces y pito
Timer t;

void setup()
{
 pinMode (ENB, OUTPUT); 
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);
 pinMode (luces, OUTPUT);
 pinMode (rojas, OUTPUT);
 pinMode (pito, OUTPUT);
 pinMode (dirDer, OUTPUT);
 pinMode (dirIzq, OUTPUT);
 attachInterrupt(bluetoothEnabled, desconectar ,FALLING);
 
 digitalWrite(luces,LOW);
 digitalWrite(rojas,LOW);
 digitalWrite(pito,LOW);
 digitalWrite(dirDer,LOW);
 digitalWrite(dirIzq,LOW);
 servo.attach(9);
 servo.write(90);
 
 //Temporizador de acciones
 t.every(500,acciones); 
 
 Serial.begin(57600);
}

void desconectar(){
   digitalWrite(ENB, 0);
}

void acciones(){
  if(dirDer1 == true){
    digitalWrite(dirDer ,!digitalRead(dirDer));
  }
  if(dirIzq1 == true){
    digitalWrite(dirIzq ,!digitalRead(dirIzq));
  }
  if(parqueo == true){
    digitalWrite(dirDer ,!digitalRead(dirDer));
    digitalWrite(dirIzq ,!digitalRead(dirIzq));
  }
  if(reversa == true){
    if(pwm>0){
      digitalWrite(pito,!digitalRead(pito));
    }
  }
}

void loop()
{
   while(Serial.available() > 0) {
    switch(Serial.read()){
      
      /*case 'a':{//Encender <-------------------Metodo en desUso
        digitalWrite (IN3, HIGH);
        digitalWrite (IN4, LOW);
        analogWrite(ENB,0);
        break;
      }*/
      /*case 'b':{//Apagar <----------------   Metodo en desUso
        digitalWrite (IN3, LOW);
        digitalWrite (IN4, LOW);
        analogWrite(ENB,0);
        break;
      }*/
      case 'c':{//Adelante
        digitalWrite (IN3, HIGH);
        digitalWrite (IN4, LOW);
        reversa = false;
        digitalWrite(pito, LOW);
        if(digitalRead(luces)==HIGH){
          analogWrite(rojas,255);
        }else{
          analogWrite(rojas,0);
        }
        break;
      }
      case 'd':{//Izquierda
        if(servo.read()>75){
          servo.write(servo.read()-1);
        }
        break;
      }
      case 'e':{//Derecha
        if(servo.read()<105){
          servo.write(servo.read()+1);
        }
        break;
      }
      case 'f':{//Atras
        digitalWrite (IN3, LOW);
        digitalWrite (IN4, HIGH);
        analogWrite(rojas,255);
        reversa = true;
        break;
      }
      case 'g':{//Acelerar
        pwm = Serial.parseInt();
        analogWrite(ENB, pwm);
        if(reversa == true && pwm == 0){
          digitalWrite(pito,LOW);
        }
        break;
      }
      case 'h':{//Girar
        int i = Serial.parseInt();
        servo.write(i);
        break;
      }
      case 'i':{//Encender luces delanteras
        analogWrite(rojas, 255);//Enciende las luces de stop mas atenuado
        digitalWrite(luces, HIGH);
        break;
      }
      case 'j':{//Apagar luces delanteras
        analogWrite(rojas,0);//apagamos las luces traseras
        digitalWrite(luces, LOW);
        break;
      }
      /*case 'k':{// <-------------- Metodo en desUso 
        digitalWrite(rojas, HIGH);
        break;
      }*/
      /*case 'l':{// <-------------- Metodo en desUso 
        digitalWrite(rojas, LOW);
        break;
      }*/ 
      case 'm':{//Encender pito
        digitalWrite(pito, HIGH);
        break;
      }
      case 'n':{//Apagar pito
        digitalWrite(pito, LOW);
        break;
      }
      case 'o':{//Encender direccionales izquierdas
        dirIzq1 = true;
        dirDer1 = false;
        digitalWrite(dirDer, LOW);
        break;
      }
      case 'p':{//Apagar direcciones izquierdas
        dirIzq1 = false;
        dirDer1 = false;
        digitalWrite(dirDer, LOW);
        digitalWrite(dirIzq, LOW);
        break;
      }
      case 'q':{//Encendemos direccionales derechas
        dirDer1 = true;
        dirIzq1 = false;
        digitalWrite(dirIzq, LOW);
        break;
      }
      case 'r':{//Apagar direccionales izquierdas
        dirDer1 = false;
        dirIzq1 = false;
        digitalWrite(dirIzq, LOW);
        digitalWrite(dirDer, LOW);
        break;
      }
      case 's':{//Encender luces parqueo
        parqueo = true;
        digitalWrite(dirIzq, LOW);
        digitalWrite(dirDer, LOW);
        break;
      }
      case 't':{
        parqueo = false;
        digitalWrite(dirIzq, LOW);
        digitalWrite(dirDer, LOW);
        break;
      }
      case 'u':{
        rojo = Serial.parseInt();
        analogWrite(6,rojo);
        break;
      }
      case 'v':{
        verde = Serial.parseInt();
        analogWrite(5,verde);
        break;
      }
      case 'w':{
        azul = Serial.parseInt();
        analogWrite(11,azul);
        break;
      }
      
      
    }//Fin del Switch
  }//Fin del While
  t.update();
}//Fin Loop
