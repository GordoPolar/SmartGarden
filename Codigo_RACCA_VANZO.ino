#include <SoftwareSerial.h>


SoftwareSerial BTSerial(5, 6);  // RX, TX
////////////////////// A1-Sensor de lluvia ; A2-Nivel d agua ; A5-Sensor humedad

float humedadPorcentaje = 0;
int SenHumedad = 0;
int myChar = 0;
int SensorLluvia = A1;
int SensorAgua = A2;
int SensorHumedad = A5;
int lluvia = 0;
const int relayPin = 12;
const int motor1A = 3;  // Conectar a IN1
const int motor1B = 4;  // Conectar a IN2


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(SensorLluvia, INPUT);
  pinMode(SensorAgua, INPUT);
  pinMode(SenHumedad, INPUT);  // Se corrigió el nombre del pin
  pinMode(relayPin, OUTPUT);
}
void avanzar() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
}

void parar() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
}

void retroceder() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
}
void loop() {

  //////////////////////////////// SENSOR LLUVIA OK FUNCIONA MAMAGUEVO

  int sensorValue = analogRead(SensorLluvia);
  static bool llueveAnterior = false;

  if (sensorValue < 700) {
    Serial.println("Esta lloviendo");
    if (!llueveAnterior) {
      avanzar();
      delay(5000);
      parar();
      llueveAnterior = true;
    }
  } else {
    Serial.println("No hay lluvia");
    if (llueveAnterior) {
      retroceder();

      delay(5000);
      parar();
      llueveAnterior = false;
    }
  }

  //////////////////////////////// VALORES NIVEL DE AGUA OK

  int waterLevel = digitalRead(SensorAgua);

  if (waterLevel == LOW) {
    Serial.println("Nivel de agua alcanzado");
    digitalWrite(relayPin, HIGH);
  }
  if (waterLevel == HIGH) {
    Serial.println("Nivel de agua insuficiente");
    digitalWrite(relayPin, LOW);
  }

  //////////////////////////////// VALORES HUMEDAD 

  int valorHumedad = analogRead(SensorHumedad);
  int porcentajeHumedad = map(valorHumedad, 0, 1023, 100, 0);

  
  char buffer[10];
  itoa(porcentajeHumedad, buffer, 10);
  
  Serial.print("Humedad: ");
  Serial.print(porcentajeHumedad);
  Serial.println("%");

  BTSerial.print("Humedad: ");
  BTSerial.print(porcentajeHumedad);
  BTSerial.println("%");

  delay(1500); 

  while (BTSerial.available()) {
    myChar = BTSerial.read();
    Serial.print("Recibido desde Bluetooth: ");
    Serial.println(myChar);
  }
}
