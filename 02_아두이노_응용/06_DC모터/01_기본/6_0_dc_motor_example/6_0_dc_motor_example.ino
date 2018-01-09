//모터 핀
#define IN1 7
#define IN2 6

// 속도 조절을 하기 위한 PIN 
#define ENA 9

void setup() {
//핀 설정 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
pinMode(ENA, OUTPUT);
}

// 속도 설정
// speed : 0 ~ 255 까지 가능
void setMotorSpeed(unsigned char speed){
analogWrite(ENA, speed);  
}

//앞으로
void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

//뒤로
void backward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

//정지
void stopMotor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void loop() {

//최대 속도로 동작
  setMotorSpeed(255);
  //앞으로 가기
forward();
//1초(1000us)동안 동작함
delay(1000); 

//뒤로가기
  backward();
  delay(1000);

//속도 조절 확인
setMotorSpeed(150);  

  //150 속도로 1초간 앞으로
  forward();
delay(1000);

//150 속도로 1초간 뒤로
  backward();
delay(1000);

//loop처음으로 돌아가서 다시 시작
}

