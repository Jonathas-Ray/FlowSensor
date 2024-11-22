#include <LiquidCrystal.h>
#define Tick = 500/246; // 0,9817 L == 483.12 Tiques, onde cada 500 ML são 246;

int flowSensorInc = 2;  // Sensor 1
int flowSensorDec = 3;  // Sensor 2
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

float totalStored_ml = 0.0;

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

  lcd.begin(16, 2);

  attachInterrupt(digitalPinToInterrupt(flowSensorInc), increaseStorage, RISING); // método para configurar interrupções contando os pulsos dos sensores
  attachInterrupt(digitalPinToInterrupt(flowSensorDec), decreaseStorage, RISING); // (entrada, função a ser executada, comportamento a ser observado no sensor)

  lcd.print("Total armazenado:");
  lcd.setCursor(0, 1);  // necessário p/ passar pra segunda linha, do contrário sobrescreve a primeira
  lcd.print("%.3f Litros", totalStored_ml);
}

void loop() {
  //Dada a ausência da necessidade de controlar melhor os dados de entrada e saída de pulsos, os fiz diretamente sem uma variável intermediária
  totalStored += increasePulse;
  increasePulse = 0;

  totalStored -= decreasePulse;
  decreasePulse = 0;
  totalStored_ml = totalStored * Tick;
}