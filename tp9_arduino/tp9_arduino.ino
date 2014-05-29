/*******************ULTRASONS HC-SR04*************************
 * 
 * Démonstration de l'utilisation du module à ultrasons HC-SR04
 * avec un Arduino. La distance mesurée par le module est affichée
 * sur le LCD
 * 
 * Connexions:
 * 
 * Le module HC-SR04 ne comporte que 4 pins:
 * 
 * VCC du HC-SR04 ---> 5V de l'Arduino Mega
 * Trig du HC-SR04 ---> digital 7 de l'Arduino
 * Echo du HC-SR04 ---> digital 6 de l'Arduino
 * GND du HC-SR04 ---> Gnd de l'Arduino
 * 
 * source: http://electroniqueamateur.blogspot.com
 * 
 **************************************************************/
 
// definition des connexions 
int trigPin = 7;  // pin "trig" branchée dans le 7 de l'Arduino
int echoPin = 6;  // pin "echo" branchée dans le 6 de l'Arduino
 
/*******************LCD 2004 I2C*************************
* YourDuino.com Example Software Sketch
* 20 character 4 line I2C Display
* Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
* terry@yourduino.com 
**************************************************************/

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

#include <Servo.h> 

/*-----( Declare objects )-----*/
// Connection YwRobot (LCD1602 I2C)
// Top to bottom:
// GND - GND
// VCC - 5V
// SDA - ANALOG Pin 4 Uno, Pin 20 Mega
// SCL - ANALOG pin 5 Uno, Pin 21 Mega

// set the LCD address to 0x27 for a 16 chars 2 line display
// Set the pins on the I2C chip used for LCD connections: 
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int ledPin = 13;   // led pin 13

// servo
int pwmServoPin=10;
int servoval=0;
int sens=1;
Servo myservo;  // create servo object to control a servo 

// Variables
int status = 0;
int nb = 0;
long mini = 30;
int action = 0;
int nbaction = 0;
char buf[16];
String stg = "";
int nbdetect = 0;

/*******************SETUP*************************/
void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("R");
  delay(150);  
  
  lcd.begin(20,4);   // initialize the lcd for 16 chars 2 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 2; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(0,0); 
  lcd.print("Hello, world and!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("ARDUINO!");
  delay(1000);  
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0); 
  lcd.print("STOP !");
  lcd.setCursor(0,1); 
  lcd.print("distance=");  
  lcd.setCursor(0,2); 
  lcd.print("nombre  =");  

  myservo.attach(pwmServoPin);  // attaches the servo on pin 10 to the servo object 
  myservo.write(10);  // set servo to default position
}

/*******************MAIN LOOP*************************/
void loop() {
  long temps, distance;
 
  // Nous envoyons un signal haut d'une durée de 10 microsecondes, en sandwich
  // entre deux signaux bas.  Des ultrasons sont émis pendant que le signal est haut
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Lors de la réception de l'écho, le module HC-SR04 émet
  // un signal logique haut (5 v) dont la durée est égale au
  // temps écoulé entre l'émission et la réception de l'ultrason.
  pinMode(echoPin, INPUT);
  temps = pulseIn(echoPin, HIGH);
 
  // Maintenant, nous convertissons la durée en distance
  // puisque la vitesse du son est de 340 m/s , le temps est en microsecondes
  // et les ultrasons parcourent le double de la distance  par rapport é  l'obstacle,
  // la distance en cm sera:
  distance = temps * 340/(2*10000);
  
  sprintf(buf, "%d", distance);
  lcd.setCursor(10,1);
  lcd.print(buf);
  lcd.print(" cm   ");
  
  sprintf(buf, "%d", nb);
  lcd.setCursor(10,2);
  lcd.print(buf);

  // lecture capteur ultrason
  if ( status == 0 ) {
    if ( distance < mini ) { // si détection
        nbdetect++;        
        if (nbdetect > 3) { // si 3 détections consécutives
           if ( action == 0 ) { // Si Attente evenement
              status = 1;
              nb = nb + 1;
              lcd.setCursor(0,0); 
              lcd.print("Ouverture !");
              action = 1; // Ouverture
              nbaction = 0;
              Serial.println("O");  // envoi "O" pour Orange pendant l'ouverture
           }
       }
    } else {
      nbdetect=0;
    }
  }

  // Action Ouverture
  if (action == 1 ) {
    for (int val = 10; val < 100; val++) {
      myservo.write(val); 
      delay(20);
    }
    lcd.setCursor(0,0); 
    lcd.print("PASSER !   ");
	// action=2 attendre la fin du passage
    action = 2;
	
    Serial.println("G");
  }
  
  // Attente fin de passage
  if (action == 2 ) {
    if ( distance > mini ) {
        nbaction = nbaction + 1;
        if ( nbaction > 4 ) {
         
              status = 0;
              lcd.setCursor(0,0); 
              lcd.print("Fermeture !");
              action = 3;
              nbaction = 0;
              Serial.println("M");

        } 
 
    }  else {
       nbaction=0;
    }    
  }
  
  // Action Fermeture
  if (action == 3 ) {
    for (int val = 100; val > 10; val--) {
      myservo.write(val); 
      delay(20);
    }
    lcd.setCursor(0,0); 
    lcd.print("STOP !      ");
    action = 0;
    nbdetect = 0;
    Serial.println("R");
  }
  
  delay(200); 
          
}



