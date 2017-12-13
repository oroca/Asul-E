#include <SoftwareSerial.h>

//블루투스 핀
const int btTXD = 2;
const int btRXD = 3;
//LED 핀
const int ledPin = 13;

//블루투스용 소프트웨어 시리얼 포트 설정
SoftwareSerial btSerial(btTXD, btRXD);

void setup() {
//모니터 프로그램 셋팅 및 시작
  Serial.begin(9600);

  //블루투스 시리얼 셋팅 및 시작
  btSerial.begin(9600);

  //LED 포트 설정
  pinMode(ledPin, OUTPUT);

  //시작을 알림.
  Serial.println("Start The Bluetooth App");
}

void loop() {
  //블루투스로 데이터가 들어온 것이 있다면
  if(btSerial.available()){
      //ch 변수에 블루투스로 받은 데이터 저장
      int ch = btSerial.read();

      //데이터 화면에 표시
      Serial.write(ch);

      //데이터가 a이면 LED ON
      if(ch == 'a'){
        //LED ON
        digitalWrite(ledPin, HIGH);
        //스마트폰에 LED ON 메시지 보냄.
        btSerial.println("LED ON");
      }
      //데이터가 b이면 LED OFF
      else if(ch == 'b'){
        //LED OFF
        digitalWrite(ledPin, LOW);
        //스마트폰에 LED OFF 메시지 보냄.
        btSerial.println("LED OFF");
      }
  }
}
