#include<Servo.h>

Servo Platform;
Servo Pince;

//Variables definition
int mode= 1;
int cond = 0;
int initial = 0;
int initmod2 = 0;
int test= 90;
int test2=90;
int test3 =90;
int plop = 0;
int btnmode = 0;
int pos = 90; //Position's initialisation.
int detec = 0; //0 while the ball is not detected and 1 otherwise.
int Try = 0; // To constraint a part of the code that we want to run only once.
int dist = 0;  //Variable for the distance between the ball and the camera
int pas =10;// step for the motors movements.
int pasmode2 =0;
int gauche; // Left displacement
int droite; //Right displacement
double pin_led = 11; // PIN on which the led is wired.

//protection value, when the force sensor value is >a, the motor stop and the led is on.
int Protection = 50;

// force sensor
int ForcePin = 3;  // Force sensor on the A3 pin
int ForceRead;  // Force value when closing
int ForceRead2; // Force value when holding an object
int Posact; // actual motor position (angular)

// bp is a value that will caracterize the button state as when a button is pressed, its analogic value become superior to 200
const int bp = 200;

//Speed value in degree. For this particular motor, 0° to 90° is an opening direction, 0 being the max opening speed while 90° to 180° is a closing direction with 180 being the maximum closing speed
const int vf = 150;//closing speed
const int vo = 60;//opening speed
const int vs = 84;//stoping speed(because of some errors, this speed can vary between 82 and 86)

//pushbutton
const int btnPin1  = A0; //Button 1 on A0 pin
const int btnPin2  = A2;// Burron 2 on A2 pin
const int btnPin3 = A1;
int btnVal1  = 0; //To save the analogic value of our buttons
int btnVal2  = 0;
int btnVal3 = 0;
int btn1 = 0; // 1 if the button 1 is pressed 
int btn2 = 0;// 1 if the button 2 is pressezd
int btn3 = 0;
int Object; // 1 if the ball is in a certain distance from the plier

void setup() {

  Serial.begin(9600); 
  Platform.attach(9); // platform's motor on the pin 9
  Pince.attach(10); // plier'smotor on the pin 10 
  Pince.write(vs); // the plier is not moving in initial state
  pinMode(pin_led, OUTPUT);
  Platform.write(90); // the platform is a 90° in initial state
}
unsigned long startCollecting;  // timers variables
unsigned long startCollecting2;
unsigned long startCollecting3;
unsigned long startCollecting4;
unsigned long startCollecting5;
unsigned long startCollecting6;
unsigned long startCollecting7;



void loop() {
btnVal1=analogRead(btnPin1);  
btnVal2=analogRead(btnPin2);
btnVal3=analogRead(btnPin3);
ForceRead = analogRead(ForcePin); // read the force sensors value continuously

if (mode ==2 || mode ==3) {if (Serial.available() > 0) // If the ball is detected by the camera
  {
    if (initial==1 || (mode ==2 && btnmode==1)){
    detec = 1;
    if (Serial.read() == 'X')  // get the position from python
    {
      pos= Serial.parseInt();
      if (Serial.read() == 'Y') { // get the distance from python
        dist = Serial.parseInt();}
        }}
   if (mode ==3){
   // if( pos == 1 || pos == -1){
    //Platform.attach(9);  // reconnecting the motor
    if(pos == -1) {
    test3 += 3;
    ;}                    
    if(pos == +1){
    test3 -= 3;
    } 
    Platform.write(test3);}
    //else{Platform.detach();}
  
ForceRead2 = analogRead(ForcePin);
    if (dist == 1){ 

       Object = 1;  // 1 when the ball is at the plier extremity and ready to be grabbed
}}}

//if (btnmode ==1) { digitalWrite(pin_led,HIGH);}
//else{ digitalWrite(pin_led,LOW);}

 if (btnVal3 > 200){ btn3 = 1; cond = 0; initial = 0; initmod2=0;}
 else {btn3 = 0;}
  
  if (btnVal1 > 200)
  { btn2 =1;
   startCollecting2 = millis();}
   else { if(mode ==1 || mode ==2) {btn2 = 0; }}

  if(btnVal2 > 200){
    btn1 =1;
    startCollecting = millis();}
  else { if(mode ==1 || mode ==2) {btn1 = 0; }} 

if (mode == 1 || mode ==2){
if(btn2 ==1 && btn1 == 1 && startCollecting3 ==0) 
  {btnmode = 1;
  startCollecting3 = millis(); startCollecting =0; startCollecting2=0;
}

if (startCollecting3 >0) {
  if(millis() - startCollecting3 >2000) {{if(btn2 ==1 && btn1 == 1){ btnmode = 0;}} 
  if(millis() - startCollecting3 >6000 && btnmode == 0){startCollecting3 = 0;}
 } }}

 

if(btn3 ==1 && startCollecting4 ==0 && mode==1) 
  {mode =2;
  startCollecting4 = millis();
}

if (startCollecting4 >0) {
  if(millis() - startCollecting4 >3000 && mode ==2){if(btn3 == 1){ mode = 3; startCollecting5=millis();}}}
  if(startCollecting5 >0) { 
  if(millis() - startCollecting5 >3000 && mode ==3){if (btn3 ==1){
    mode =1 ; startCollecting6 = millis();}}}
    if (startCollecting6>0){
    if(millis() - startCollecting6 >2000){startCollecting4 =0; startCollecting5=0; startCollecting6 =0;}
  }
  


// MODE 2
if (mode ==2)
{if (initmod2 == 0){btnmode = 0;
if( cond ==0){
if ( ForceRead < 70)
 { Pince.write(95);  //close the plier until the force sensor value is over a certain treshold in which case the motor stop and the led is on
   digitalWrite(pin_led, LOW);}

 else 
 {Pince.write(vs);
  digitalWrite(pin_led, HIGH);cond =1;startCollecting7 = millis();
}}
else{
if (millis() - startCollecting7 < 4000)  // open the plier for 4 seconds
{Pince.write(76);}
else
{Pince.write(vs); initmod2 =1;}}}
  
  
  if (btn1 ==1 || btn2 ==1) {
  Platform.attach(9);
  if( btn1 ==1){
   test2 += 1;
   delay(8);}
 if( btn2 ==1){
   test2 -= 1;
   delay(8);} 
    Platform.write(test2);}                    
else {Platform.detach()
;}}

// MODE 1
if (mode ==1) {
  if(btnmode == 0){
if (btn1 ==1 || btn2 ==1) {
  Platform.attach(9);
  if( btn1 ==1){
   test += 1;
   delay(8);}
 if( btn2 ==1){
   test -= 1;
   delay(8);} 
    Platform.write(test);}                    
else {Platform.detach()
;}}

if(btnmode == 1){
if (btn1 == 1) { 
 Pince.write(75);}
else if (btn2 == 1) { 

  if ( ForceRead < 70)
 {Pince.write(92);  // 
  digitalWrite(pin_led, LOW);}
  else {
  digitalWrite(pin_led,HIGH); Pince.write(vs);}
}
else 
{Pince.write(vs);}
}
else 
{Pince.write(vs);} // puis stop les moteurs
}
  
// MODE 3
  if (mode ==3) {
       Platform.attach(9);
       if(Try ==0){Platform.write(90);
       if ( ForceRead < 70)
 { Pince.write(95);  //
   digitalWrite(pin_led, LOW);}

 else 
 {Pince.write(vs);
  digitalWrite(pin_led, HIGH);Try = 1;
}}
else{
       
    if(btn1 ==1) {
    if (detec == 0) {  // si l'objet n'est pas encore trouvé 
if (millis() - startCollecting < 2000)  //  
{Pince.write(78);}
else 
{Pince.write(vs);initial=1;} // puis stop les moteurs

for (pos = 0; pos <= 180; pos += 1) {
    //The plier patrols while the ball is not yet detected
    // in steps of 1 degree
    if (Serial.available() <=0){ 
    Platform.write(pos);}            
    delay(15);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) {
        if (Serial.available() <= 0){ 
    Platform.write(pos);}             
    delay(15); // 
  }}}}


// Button 2 
  
 if (btn2 == 1) {
 btn1 ==0;
 if (millis() - startCollecting2 < 2000) 
{Pince.write(82);}  
else 
{
 if ( ForceRead < 70)
 { Pince.write(95); 
   digitalWrite(pin_led, LOW);}

 else 
 {Pince.write(vs);
  digitalWrite(pin_led, HIGH);
}
}}}

// 

if (Object == 1) {
  //if (Try == 0) { // to run this part of code only once
 btn1 =0; btn2 = 0; 
 if ( ForceRead < 70)
 { Pince.write(95);
   digitalWrite(pin_led, LOW);}

 else 
 {Pince.write(vs);
  digitalWrite(pin_led, HIGH);
}

//Try = 1 ;
}
//}

}
