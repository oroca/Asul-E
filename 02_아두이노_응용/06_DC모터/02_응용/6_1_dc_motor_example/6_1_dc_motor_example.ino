//모터 PIN
#define IN1 7
#define IN2 6

#define IN3 5
#define IN4 4

//속도 PIN
#define ENA 9
#define ENB 3

//제어할 모터 번호
#define CH1 0
#define CH2 1
#define ALL_CH 2

void setup() {
//핀 설정
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

// 속도 설정
// mode : 각 모터를 선택적으로 변경 가능
// CH1, CH2, ALL_CH
// speed : 0 ~ 255 까지 가능
void setMotorSpeed(unsigned char mode, unsigned char speed){
  if(mode == CH1){
    analogWrite(ENA, speed);
  }else if(mode == CH2){
    analogWrite(ENB, speed);    
  }else{
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);    
  }
}

//앞으로
void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//뒤로
void backward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//왼쪽
void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//오른쪽
void turnRight(){  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

//정지
void stopMotor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);  
}

void loop() {
//최대 속도로 동작
  setMotorSpeed(ALL_CH, 255);

  //앞으로 가기
  forward();
  delay(1000); //1초동안 동작함
  //뒤로가기
  backward();
  delay(1000);
  //왼쪽으로 돌기
  turnLeft();
  delay(1000);
  //오른쪽으로 돌기
  turnRight();
  delay(1000);

  //속도 조절 확인
  setMotorSpeed(ALL_CH, 150);  
  forward();
  delay(1000);
  backward();
  delay(1000);
  turnLeft();
  delay(1000);
  turnRight();
  delay(1000);
}

