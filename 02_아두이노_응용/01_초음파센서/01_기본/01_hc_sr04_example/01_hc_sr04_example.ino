// 초음파 핀 정의하기
const int trigPin = 13;
const int echoPin = 12;

// 초음파 시간 저장 변수
long duration;
// 초음파 거리 저장 변수
int  distance;

void setup() {
//trigPin은 초음파를 내보내는 핀 출력으로 설정.
pinMode(trigPin, OUTPUT);
//echoPin은 초음파를 읽는 핀 입력으로 설정.
  pinMode(echoPin, INPUT);

  //모니터 프로그램 실행
  Serial.begin(115200);
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

  //화면에 출력
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println("CM");
}
