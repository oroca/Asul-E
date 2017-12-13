// 초음파 핀 정의하기
const int trigPin = 13;
const int echoPin = 12;
// led 핀 정의하기
const int ledPin = 11;

long duration; // 시간 변수
int  distance; // 거리 변수

//led 켜지는 거리 = 10cm
const int ledOnDistance = 10;
//led 꺼지는 거리 = 15cm
const int ledOffDistance = 15;
int ledStatus = LOW;

//거리를 이용해서 LED 상태 표시 함수
void ledProc(int distance){
  //LED가 꺼져 있는 상태라면
  if(ledStatus == LOW){
    //거리가 LED ON 거리보다 작거나 같다면
    //즉, 거리가 10cm 이하라면
    if(distance <= ledOnDistance){
       //LED ON
       ledStatus = HIGH;
    }
  }else {
    //LED가 켜져있는 상태라면
    //거리가 LED OFF 거리만큼 크거나 같다면
    //즉, 거리가 15cm 이상이라면
    if(distance >= ledOffDistance){
      //LED OFF
      ledStatus = LOW;
    }
  }
  //ledStatus 변수값을 이용해서 LED에 적용
  digitalWrite(ledPin, ledStatus);
}

void setup() {
  //ledPin은 LED를 제어아하는 핀 출력으로 설정.
  pinMode(ledPin, OUTPUT);

  //trigPin은 초음파를 내보내는 핀 출력으로 설정.
  pinMode(trigPin, OUTPUT);

  //echoPin은 초음파를 읽는 핀 입력으로 설정.
  pinMode(echoPin, INPUT);

  //모니터 프로그램 실행
  Serial.begin(9600);
}
void loop() {
//초음파 초기화
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //초음파 송신
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  //10us 후 정지
  digitalWrite(trigPin, LOW);

  //ECHO 핀를 통해 리턴된 시간 읽기
  duration = pulseIn(echoPin, HIGH);

  //시간 데이터를 거리데이터로 환산하는 식
  distance = duration*0.034/2; //Cm

  //거리를 이용해 LED 제어 함수 호출
  ledProc(distance);

  //화면에 출력
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println("CM");
}
