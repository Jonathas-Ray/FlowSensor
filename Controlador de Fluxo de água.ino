#define MaxStorage 483        // 0,9817 L == 483.12 Tiques, onde cada 500 ML são 246;

int flowSensorInc = 2;  // Sensor 1
int flowSensorDec = 3;  // Sensor 2
int ledPins[] = {4, 5, 6, 7, 8, 9, 10};  //Facilita pra não ter que inicializar LED por LED em setup

volatile int increasePulse = 0;  
volatile int decreasePulse = 0; //Volatile porque vai ser manipulado fora do Loop pelo attachInterrupt
int totalStored = 0; // Volume total em Tiques

//Funções para contar os pulsos, necessário para o método attachInterrupt
void increaseStorage() {
  increasePulse++;
}
void decreaseStorage() {
  decreasePulse++;
}

void setup() {

  pinMode(flowSensorInc, INPUT);  // Sensor 1
  pinMode(flowSensorDec, INPUT);  // Sensor 2

  for (int i = 0; i < 7; i++) { //inicializa os LEDs
    pinMode(ledPins[i], OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(flowSensorInc), increaseStorage, RISING); // método para configurar interrupções contando os pulsos dos sensores
  attachInterrupt(digitalPinToInterrupt(flowSensorDec), decreaseStorage, RISING); // (entrada, função a ser executada, comportamento a ser observado no sensor)
}

void loop() {
  //Dada a ausência da necessidade de controlar melhor os dados de entrada e saída de pulsos, os fiz diretamente sem uma variável intermediária
  totalStored += increasePulse;
  increasePulse = 0;

  totalStored -= decreasePulse;
  decreasePulse = 0;

  controLEDs(totalStored);
}

void controLEDs(int totalStored) {
    
  float percentage = ((totalStored) / MaxStorage) * 100;

  if (percentage = 0) {
    turnTheLEDOn(0); // Primeiro LED vai piscar alertando que está zerado ou proximo de acabar
  } else if (percentage <= 16.67) {
    turnTheLEDOn(1); // 1/6, um LED aceso
  } else if (percentage <= 33.33) {
    turnTheLEDOn(2); // 2/6, dois LEDs acesos
  } else if (percentage <= 50) {
    turnTheLEDOn(3); // 3/6, três LEDs acesos
  } else if (percentage <= 66.67) {
    turnTheLEDOn(4); // 4/6, quatro LEDs acesos
  } else if (percentage <= 83.33) {
    turnTheLEDOn(5); // 5/6, cinco LEDs acesos
  } else {
    turnTheLEDOn(6); // Capacidade máxima, todos acesos
  }
} // int ledPins[] = {4(0), 5(1), 6(2), 7(3), 8(4), 9(5), 10(6)} 7 índices


void turnTheLEDOn(int stored) { //acende os LEDs conforme a porcentagem do volume
    if (stored == 0){
        for(int i = 1; i <= 7; i++){
            digitalWrite(ledPins[i], LOW);
        }
        digitalWrite(ledPins[0], HIGH);
        delay(250);
        digitalWrite(ledPins[0], LOW);
        delay(250); //Atualizar +/- na velocidade que máxima de processamento de tiques pelo sensor para evitar perdas
    } else {
        for (int i = 1; i < 7; i++) {
            if (i <= stored) {
                digitalWrite(ledPins[i], HIGH);  // Acende o LED
            } else {
                digitalWrite(ledPins[i], LOW);   // Apaga o LED
            }
        }
    }
}