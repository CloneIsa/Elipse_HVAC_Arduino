#include <SimpleModbusSlave.h>
#define BUTTON1_PIN 7
#define BUTTON2_PIN 4

// Índices dos registradores Modbus
enum 
{     
  BUTTON1_TOGGLED_STATE, // 1
  POT1_VAL,              // 2
  BUTTON2_TOGGLED_STATE, // 3
  POT2_VAL,              // 4
  POT3_VAL,              // 5
  HOLDING_REGS_SIZE      
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; 

// Estados anteriores dos botões
bool lastButton1State = HIGH; // INPUT_PULLUP = HIGH quando não pressionado
bool lastButton2State = HIGH;

bool toggledState1 = 0;
bool toggledState2 = 0;

void setup()
{
  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);
  modbus_update_comms(9600, SERIAL_8N1, 1);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void loop()
{
  modbus_update();

  // Leitura dos potenciômetros
  holdingRegs[POT1_VAL] = analogRead(A1);
  holdingRegs[POT2_VAL] = analogRead(A2);
  holdingRegs[POT3_VAL] = analogRead(A3);

  // Leitura dos botões com detecção de borda de descida
  bool currentButton1State = digitalRead(BUTTON1_PIN);
  if (lastButton1State == HIGH && currentButton1State == LOW) {
    toggledState1 = !toggledState1;
    holdingRegs[BUTTON1_TOGGLED_STATE] = toggledState1;
  }
  lastButton1State = currentButton1State;

  bool currentButton2State = digitalRead(BUTTON2_PIN);
  if (lastButton2State == HIGH && currentButton2State == LOW) {
    toggledState2 = !toggledState2;
    holdingRegs[BUTTON2_TOGGLED_STATE] = toggledState2;
  }
  lastButton2State = currentButton2State;
}
