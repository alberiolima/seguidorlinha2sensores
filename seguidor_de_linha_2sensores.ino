/*
 * Código simples para seguidor de linha, usando sensores digitais que retorna 0 quando fora da linha e 1 quando dentro
 * feito apenas para aprendizagem da lógica
 */

/*
 * Pinos da ponte H, (driver dos motores) para o teste foi usada a L298N 
 * 4 e 5 para motores direito,  6 e 7 para o esquerdo, necessário para inversão do sentido de rotação do motor
 * nesse programa a inversão da rotação não foi necessária
 * 3 e 9 são para controlar a velocidade de rotação do motor usando PWM de 0 a 255
 * obs: Um valor PWM abaixo de 80 talvez não consiga fazer o motor girar
 */
#define ENA   3 //PWM (controle da velocidade do motor direito)
#define INT1  5 //MOTOR-A 
#define INT2  4 //
#define INT3  7 //MOTOR-B 
#define INT4  6
#define ENB   9 //PWM (controle da velocidade do motor esquerdo)

//Pinos dos sensores 
#define sensorEsquerda  A2
#define sensorDireita   A3

//Velocidade constante PWM
#define velocidade 100

//Correcao necessário quando um motor gira mais rápido que outro, será somando a velocidade
#define correcaoMotorEsquerdo 0 
#define correcaoMotorDireito  0

//Variáveis que guarda o valor da leitura, para recuperação no caso de sair da linha
//Também servepara ele entrar caso seja colocado fora da linha
boolean leituraAnteriorEsquerda = false; 
boolean leituraAnteriorDireita = false;

void setup() {
  //Define pinos como saída para ponte H
  pinMode(ENA, OUTPUT); 
  pinMode(INT1, OUTPUT); 
  pinMode(INT2, OUTPUT);
  pinMode(INT3, OUTPUT);
  pinMode(INT4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  //Define pinos dos sensores como entrada, verifique a necessidade ou não de usar INPUT_PULLUP
  pinMode( sensorEsquerda, INPUT_PULLUP );
  pinMode( sensorDireita, INPUT_PULLUP );

  delay(3000); //Aguarda um tempo depois de ligado o carrinho para poder ligar os motore
  
  //Liga inicialmente os dois motores na mesma velocidade, necessário quando quiser colocar o carro fora da linha
  digitalWrite(INT1, HIGH); 
  digitalWrite(INT2, LOW);
  digitalWrite(INT3, HIGH);
  digitalWrite(INT4, LOW);  
  analogWrite(ENA, velocidade + correcaoMotorDireito); //Roda direita  
  analogWrite(ENB, velocidade + correcaoMotorEsquerdo); //Roda esquerda
}

void loop() {
  //boolean valorSensorEsquerda = ( analogRead( sensorEsquerda ) > 200 ); //Converte valor analogico para digital, necessario apenas se usar sensor analogico
  //boolean valorSensorDireita = ( analogRead( sensorDireita ) > 200 );
  boolean valorSensorEsquerda = (boolean)analogRead( sensorEsquerda ); 
  boolean valorSensorDireita =  (boolean)analogRead( sensorDireita );

  if (!valorSensorEsquerda&&!valorSensorDireita){ //*** dois sensores fora da linha
    valorSensorEsquerda = leituraAnteriorEsquerda; //Usa dados da leitura anterior, necessário 
    valorSensorDireita = leituraAnteriorDireita; //quando saiu muito rápido da linha
  }

  if (valorSensorEsquerda&&valorSensorDireita){ //*** dois sensores na linha
    analogWrite(ENA, velocidade + correcaoMotorDireito);  //dois motores mesma velocidade
    analogWrite(ENB, velocidade + correcaoMotorEsquerdo); 
  } else if (valorSensorEsquerda&&!valorSensorDireita){ //*** Apenas sensor esquerdo sobre a linha
    analogWrite(ENB, 0); //Para roda esquerda
  } else if (!valorSensorEsquerda&&valorSensorDireita){ //*** Apenas sensor direito sobre a linha
    analogWrite(ENA, 0); //Para roda direita
  }     
  leituraAnteriorEsquerda = valorSensorEsquerda;
  leituraAnteriorDireita = valorSensorDireita;
}
