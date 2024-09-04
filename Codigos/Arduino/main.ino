#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Instancia o driver do PWM
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Portas do pwm 
#define base 0
#define art1Esq 2
#define art1Dir 4
#define art2 6
#define art3 8
#define pulso 10
#define garra 12

// Define as constantes para o controle dos servos
// TODO: resolver os problemas com os servos da articulação 1
#define SERVOMIN  100  // Valor mínimo para o pulso
#define SERVOMAX  500  // Valor máximo para o pulso

// Define os ângulos para alternar
#define ANGLE_A1  120
#define ANGLE_A2  170

// Variaveis de estado
// Variável para armazenar o estado atual da garra
bool garraState = false;  // False: ANGLE_A1, True: ANGLE_A2
// Variável para armazenar o estado atual da articulação 3
int art3AnguloInicial = 170;
// Controle de graus de movimento das articulações
//MovesSteps
int moveStepArt1 = 10;
int moveStepArt2 = 10;
int moveStepArt3 = 10;
int moveStepBase = 10;
// Variável para armazenar o estado atual do pulso
int pulsoInicial = 90;
// Variável para armazenar o estado atual da articulação 2
int art2AnguloInicial = 120;
// Variável para armazenar o estado atual da articulação 1
int art1AnguloInicial = 80;


void setup() {
  pwm.begin();                    // Inicia o objeto PWM
  pwm.setPWMFreq(50);             // Define a frequência de trabalho do servo (50Hz)
  Serial.begin(9600);             // Inicia a comunicação serial a 9600 bps
  Serial.println("Aguardando comandos...");
}

void loop() {
  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Lê o comando da porta serial
    command.trim();  // Remove espaços em branco no início e no final da string

    // Controle Left e Right - Pulso
    if (command == "RightPulso") {
      if (pulsoInicial <= 90) { // Limita o incremento para não ultrapassar 180 graus
        int targetAngle = pulsoInicial + 90; // Define o ângulo alvo
        for (int angle = pulsoInicial; angle <= targetAngle; angle++) {
          int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
          pwm.setPWM(pulso, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade do movimento (maior delay = mais lento)
        }
        pulsoInicial = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo pulso para: ");
        Serial.print(pulsoInicial);
        Serial.println(" graus");
      }
    }

    if (command == "LeftPulso") {
      if (pulsoInicial >= 90) { // Limita o decremento para não ultrapassar 0 graus
        int targetAngle = pulsoInicial - 90; // Define o ângulo alvo
        for (int angle = pulsoInicial; angle >= targetAngle; angle--) {
          int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
          pwm.setPWM(pulso, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade do movimento (maior delay = mais lento)
        }
        pulsoInicial = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo pulso para: ");
        Serial.print(pulsoInicial);
        Serial.println(" graus");
      }
    }

    // Controle crossLeft e crossRight - Base

    if (command == "crossLeft") {
      if (base - moveStepBase >= 0) { // Verifica se não ultrapassa 0 graus
        int targetAngle = base - moveStepBase; // Define o ângulo alvo
        for (int angle = base; angle >= targetAngle; angle--) {
          int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
          pwm.setPWM(0, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade (maior delay = mais lento)
        }
        base = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo base para: ");
        Serial.print(base);
        Serial.println(" graus");
      }
    }

    if (command == "crossRight") {
      if (base + moveStepBase <= 180) { // Verifica se não ultrapassa 180 graus
        int targetAngle = base + moveStepBase; // Define o ângulo alvo
        for (int angle = base; angle <= targetAngle; angle++) {
          int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
          pwm.setPWM(0, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade (maior delay = mais lento)
        }
        base = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo base para: ");
        Serial.print(base);
        Serial.println(" graus");
      }
    }

    // Controle Up e Down - Articulação 1
    if (command == "DownArt1") {
      if (art1AnguloInicial + moveStepArt1 <= 180) { // Verifica se não ultrapassa 180 graus
        int targetAngle = art1AnguloInicial + moveStepArt1; // Define o ângulo alvo
        for (int angle = art1AnguloInicial; angle <= targetAngle; angle++) {
          int pulselength = map(angle, 0, 180, 205, 409);
          pwm.setPWM(art1Esq, 0, pulselength);
          pwm.setPWM(art1Dir, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade (maior delay = mais lento)
        }
        art1AnguloInicial = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo art1 para: ");
        Serial.print(art1AnguloInicial);
        Serial.println(" graus");
      }
    }

    if (command == "UpArt1") {
      if (art1AnguloInicial - moveStepArt1 >= 0) { // Verifica se não ultrapassa 0 graus
        int targetAngle = art1AnguloInicial - moveStepArt1; // Define o ângulo alvo
        for (int angle = art1AnguloInicial; angle >= targetAngle; angle--) {
          int pulselength = map(angle, 0, 180, 205, 409);
          pwm.setPWM(art1Esq, 0, pulselength);
          pwm.setPWM(art1Dir, 0, pulselength);
          delay(15); // Ajusta o delay para controlar a velocidade (maior delay = mais lento)
        }
        art1AnguloInicial = targetAngle; // Atualiza a posição inicial
        Serial.print("Movendo art1 para: ");
        Serial.print(art1AnguloInicial);
        Serial.println(" graus");
      }
    }

    // Controle Up e Down - Articulação 2
    if (command == "DownArt2"){
      if (art2AnguloInicial + moveStepArt2 <= 180){
        art2AnguloInicial += moveStepArt2;
        int pulselength = map(art2AnguloInicial, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(art2, 0, pulselength);
        Serial.print("Movendo art2 para: ");
        Serial.print(art2AnguloInicial);
        Serial.println(" graus");
        delay(10);
      }
    }

    if (command == "UpArt2"){
      if (art2AnguloInicial - moveStepArt2 >= 0){
        art2AnguloInicial -= moveStepArt2;
        int pulselength = map(art2AnguloInicial, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(art2, 0, pulselength);
        Serial.print("Movendo art2 para: ");
        Serial.print(art2AnguloInicial);
        Serial.println(" graus");
        delay(10);
      }
    }

    // Controle Up e Down - Articulação 3
    if (command == "DownArt3"){
      if (art3AnguloInicial + moveStepArt3 <= 170){
        art3AnguloInicial += moveStepArt3;
        int pulselength = map(art3AnguloInicial, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(art3, 0, pulselength);
        Serial.print("Movendo art3 para: ");
        Serial.print(art3AnguloInicial);
        Serial.println(" graus");
        delay(10);
      }
    }
    if (command == "UpArt3"){
      if (art3AnguloInicial - moveStepArt3 >= 0){
        art3AnguloInicial -= moveStepArt3;
        int pulselength = map(art3AnguloInicial, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(art3, 0, pulselength);
        Serial.print("Movendo art3 para: ");
        Serial.print(art3AnguloInicial);
        Serial.println(" graus");
        delay(10);
      }
    }

    // Controle A - Abrir e fechar a garra
    if (command == "BUTTON_A") {
      // Alterna o estado do servo
      garraState = !garraState;

      // Define o ângulo com base no estado atual
      int angulo = garraState ? ANGLE_A2 : ANGLE_A1;
      int pulselength = map(angulo, 0, 180, SERVOMIN, SERVOMAX);

      // Move o servo conectado à porta 0 do PCA9685
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
