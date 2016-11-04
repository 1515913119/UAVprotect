/*-------------------------------------------------------------------------------------------------
   无人机失控保护系统   V1.0.0
   NEUQ
   作者：孙航 吴伯彦 杨友升 叶鑫 安会爽
   Date：2016.10
   遥控收入 D5；
   降落伞控制舵机接口 servo1 D9； 
   气囊控制接口 servo2 D8；
   气囊舵机控制接口servo3 D10；
   继电器接口 D3；
   蜂鸣器接口 D4；
   JY901 iic接口 硬件iic；
   电流传感器 A0 A1 A2 A3；
   工作指示led D6；
   失控led D7；
   哈哈哈哈哈
   噢噢噢噢噢噢噢噢
  ------------------------------------------------------------------------------------------------- */
#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <JY901.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
int currenttest();
int PWMtest();
int angletest();
int execute();
void setup()
{
  JY901.StartIIC();
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  pinMode(5, INPUT);

  digitalWrite(3,HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  
  myservo1.attach(9);
  myservo2.attach(8);
  myservo3.attach(10);

  myservo1.write(70);//降落伞初始位置
  myservo2.write(15);//气囊函道输出为0
  myservo3.write(80);//气囊舵机控制初始位置

  Serial.begin(9600);
  delay(5000);//开机延时5s
  digitalWrite(7, HIGH);
}

void loop()
{
	//PWMtest();
	//currenttest();
	while (PWMtest())
	{
		execute();
	}
	delay(100);
}
  /*while(angletest)
  {
   execute();
  }
*/

int angletest()
{
 // digitalWrite(3, HIGH);
  float  c;
  float  b;
  JY901.GetAngle();
  c = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
  b = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
  if (sqrt(c * c) > 90 || sqrt(b * b) > 90 )
    return (1);
  else
    return (0);
}
int currenttest()
{
  int i, j, k, l, bias, avr;
  i = 1023-analogRead(0);
  j = analogRead(1);
  k = analogRead(2);
  l = analogRead(3);
  avr = (i + j + k + l) / 4;
  bias = (i - avr) * (i - avr) + (j - avr) * (j - avr) + (k - avr) * (k - avr) + (l - avr) * (l - avr);
  Serial.print(i);
  Serial.print("/");
  Serial.print(j);
  Serial.print("/");
  Serial.print(k);
  Serial.print("/");
  Serial.print(l);
  Serial.print("/");
  Serial.print(bias);
  Serial.println(" ");
}
int PWMtest()
{
  int x;
  x = pulseIn(5, HIGH);
  Serial.print(x);
  Serial.println("/");
  if (x > 2000)
    return (1);
  else
    return (0);
}

int execute()
{
  digitalWrite(3, LOW);//继电器断开
  delay(200);
  myservo1.write(180);//降落伞弹开
  digitalWrite(3, HIGH);//飞控上电
  
  delay(400);
  myservo3.write(160);//气囊舵机弹开
  delay(50);
  myservo2.write(170);//气囊充气
  delay(1600);
  myservo2.write(65);//气囊保持
  while (1)
  {
   digitalWrite(6, LOW);
   digitalWrite(4, LOW);  //蜂鸣器报警
   delay(500);
   digitalWrite(6, HIGH);
   digitalWrite(4, HIGH);
   delay(500);  
  }

}



