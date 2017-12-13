#include <NewPing.h> //NewPing 라이브러리 추가

//첫번째 Trigger, Echo Pin 정의
const int trigPin_ch1 = 4;
const int echoPin_ch1 = 2;

//두번째 Trigger, Echo Pin 정의
const int trigPin_ch2 = 5;
const int echoPin_ch2 = 3;

//최대 측정 거리 설정
#define MAX_DISTANCE 200

//NewPing 클래스 생성
//첫번째
NewPing sonar_ch1(trigPin_ch1, echoPin_ch1, MAX_DISTANCE);
//두번째
NewPing sonar_ch2(trigPin_ch2, echoPin_ch2, MAX_DISTANCE);

void setup() {
  //모니터 프로그램을 위한 시리얼 시작
  Serial.begin(115200);
}

void loop() {
  //최소한 29ms이상은 대기를 해야한다고 함.
  //대기
  delay(50);
  distance1 = sonar_ch1.ping_cm();
  distance2 = sonar_ch2.ping_cm();
  //첫번째 거리 읽기
  Serial.print("Ch1: ");
  Serial.print(distance1);
  Serial.println("cm");

  //두번째 거리 읽기
  Serial.print("Ch2: ");
  Serial.print(distance2);
  Serial.println("cm");
}
