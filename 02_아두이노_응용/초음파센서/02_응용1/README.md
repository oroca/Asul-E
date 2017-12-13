## HC-SR04 초음파 센서 응용 1편 예제 코드



### 연결도

![](../img/sch.png)

<pre>
HC-SR04      아두이노
#1, VCC    -     5V
#2, Trig   -     D13
#3, Echo   -     D12
#4, GND    -     GND
</pre>

<br/>
<br/>

### 결과

물체가 10CM 이하로 가까워지면 LED(13PIN) 점등
물체가 15CM 이상 멀어지면 LED(13PIN) 소등
