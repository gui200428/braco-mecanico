#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Instancia o driver do PWM
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define as constantes para o controle dos servos
#define SERVOMIN  100  // Valor mínimo para o pulso
#define SERVOMAX  500  // Valor máximo para o pulso

// Define os ângulos para alternar
#define ANGLE_A1  120
#define ANGLE_A2  170

// Variável para armazenar o estado atual do servo
bool servoState = false;  // False: ANGLE_A1, True: ANGLE_A2

void setup() {
  pwm.begin();                    // Inicia o objeto PWM
  pwm.setPWMFreq(50);             // Define a frequência de trabalho do servo (50Hz)
  Serial.begin(9600);             // Inicia a comunicação serial a 9600 bps
  Serial.println("Aguardando comandos...");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Lê o comando da porta serial

    // Verifica se o comando recebido é "A"
    if (command.trim() == "BUTTON_A") {
      // Alterna o estado do servo
      servoState = !servoState;

      // Define o ângulo com base no estado atual
      int angulo = servoState ? ANGLE_A2 : ANGLE_A1;
      int pulselength = map(angulo, 0, 180, SERVOMIN, SERVOMAX);  // Mapeia o ângulo para o pulso

      // Abre / fecha a garra
      pwm.setPWM(12, 0, pulselength);

      Serial.print("Movendo para: ");
      Serial.print(angulo);
      Serial.println(" graus");
    }

    // Limpa o buffer de entrada para evitar leituras incorretas
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
