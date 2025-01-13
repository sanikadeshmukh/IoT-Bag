#include&lt;LiquidCrystal.h&gt;
#include&lt;string.h&gt;
#include &lt;SoftwareSerial.h&gt;
#include&lt;TinyGPS.h&gt;
SoftwareSerial gsm(9, 10); //RX,TX
LiquidCrystal Lcd(2,3,4,5,6,7);
TinyGPS gps;
String url=&quot;&quot;;
int sw1=11,sw2=12,SOS=8,rel=13;
char c;
//String mes=&quot;Track&quot;;
//String gps;
//String pos;
void setup()
{
pinMode(rel,OUTPUT);
pinMode(sw1,INPUT);
pinMode(sw2,INPUT);
pinMode(SOS,INPUT);
Serial.begin(9600);
gsm.begin(9600);
delay(100);
Lcd.begin(16,2);

Lcd.setCursor(0,0);
Lcd.clear();
Lcd.print(&quot;-*-VPM THANE-*-&quot;);
delay(5000);
Lcd.clear();
Lcd.print(&quot;-*-SMART BAG-*-&quot;);
digitalWrite(rel,HIGH);

41

}

void loop()
{
int a,b;
char x,m;
String pass1,msg;
delay(10);
a=digitalRead(sw1);
b=digitalRead(sw2);
while(Serial.available())
{
delay(100);
for(int i=0;i&lt;12;i++)
{
x=Serial.read();
pass1=pass1+x;
}
}
if(a==0)
{
Lcd.clear();
Lcd.print(&quot;TIMETABLE 1&quot;);
delay(5000);
Lcd.clear();
Lcd.print(&quot;JPR&quot;);
Lcd.setCursor(12,11);
Lcd.print(&quot;OSY&quot;);
Lcd.setCursor(12,0);
Lcd.print(&quot;CSE&quot;);
Lcd.setCursor(0,4);
Lcd.print(&quot;SEN&quot;);

}

42

if(b==0)
{
Lcd.clear();
Lcd.print(&quot;TIMETABLE 2&quot;);
delay(5000);
Lcd.clear();
Lcd.setCursor(0,0);
Lcd.print(&quot;STE&quot;);
Lcd.setCursor(12,11);
Lcd.print(&quot;MNG&quot;);
Lcd.setCursor(12,0);
Lcd.print(&quot;AMI&quot;);
Lcd.setCursor(0,4);
Lcd.print(&quot;AJP&quot;);
}

if((pass1.charAt(8)==&#39;7&#39;)&amp;&amp;(pass1.charAt(9)==&#39;A&#39;)&amp;&amp;(pass1.charAt(10)==&#39;A&#39;)&amp;&amp;(pass1.ch
arAt(11)==&#39;8&#39;))
{
Lcd.setCursor(0,0);
Lcd.print(&quot; &quot;);
}

if((pass1.charAt(8)==&#39;C&#39;)&amp;&amp;(pass1.charAt(9)==&#39;B&#39;)&amp;&amp;(pass1.charAt(10)==&#39;2&#39;

)&amp;&amp;(pass1.charAt(11)==&#39;4&#39;))
{
Lcd.setCursor(12,11);
Lcd.print(&quot; &quot;);
}

43

if((pass1.charAt(8)==&#39;0&#39;)&amp;&amp;(pass1.charAt(9)==&#39;2&#39;)&amp;&amp;(pass1.charAt(10)==&#39;E&#39;
)&amp;&amp;(pass1.charAt(11)==&#39;E&#39;))
{
Lcd.setCursor(12,0);
Lcd.print(&quot; &quot;);
}

if((pass1.charAt(8)==&#39;7&#39;)&amp;&amp;(pass1.charAt(9)==&#39;1&#39;)&amp;&amp;(pass1.charAt(10)==&#39;A&#39;
)&amp;&amp;(pass1.charAt(11)==&#39;3&#39;))
{
Lcd.setCursor(0,4);
Lcd.print(&quot; &quot;);
}

ReceiveMessage();

while(gsm.available())
{
//Serial.write(gsm.read());
m = gsm.read();
msg+=m;
// Serial.println(&#39;2&#39;);
delay(5);
}

Serial.println(msg);
String message = msg.substring(57,62);
Serial.println(message);
//Serial.println(&#39;3&#39;);
//Serial.println(message.length());
//Serial.println(message.equals(mes));
//delay(100);

44
if((message == &quot;track&quot;) || (digitalRead(SOS)==0))
{
Serial.println(&#39;*&#39;);
digitalWrite(rel,LOW);
String loc=location();
SendMessage(loc);
digitalWrite(rel,HIGH);
delay(10);
}
}

void SendMessage(String str)
{
Serial.println(&#39;4&#39;);
gsm.println(&quot;AT+CMGF=1&quot;); //Sets the GSM Module in Text Mode
delay(1000); // Delay of 1000 milli seconds or 1 second
gsm.println(&quot;AT+CMGS=\&quot;+919757389373\&quot;\r&quot;); // Replace x with mobile number
delay(1000);
gsm.println(str);// The SMS text you want to send
delay(100);
gsm.println((char)26);// ASCII code of CTRL+Z
delay(1000);
}

void ReceiveMessage()
{
gsm.println(&quot;AT+CNMI=2,2,0,0,0&quot;); // AT Command to receive a live SMS
// Serial.println(&#39;1&#39;);
delay(1500);
}

String location()
{
bool newData = false;

45

unsigned long chars;
unsigned short sentences, failed;
// For one second we parse GPS data and report some key values

for (unsigned long start = millis(); millis() - start &lt; 1000;)
{
while (Serial.available())
{
char c = Serial.read();
// Serial.write(c); // uncomment this line if you want to see the GPS data flowing

if (gps.encode(c)) // Did a new valid sentence come in?
newData = true;
}
}

if (newData)
{
float flat, flon;
unsigned long age;
gps.f_get_position(&amp;flat, &amp;flon, &amp;age);

/*Serial.print(&quot;LAT=&quot;);

Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
Serial.print(&quot; LON=&quot;);
Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
Serial.print(&quot; SAT=&quot;);
Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 :
gps.satellites());

Serial.print(&quot; PREC=&quot;);

46

Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());*/
String lat = String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
String longt = String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
url = &quot;https://www.google.com/maps/search/?api=1&amp;query=&quot;;
url += lat;
url += &#39;,&#39;;
url += longt;
}

else
{
url = &quot;https://goo.gl/maps/xg95XSh6QAm&quot;;
}

Serial.println(url);
return url;

}
