import processing.serial.*;

float valeurArduino = 0; // valeur envoyé par le capteur Arduino

Serial myPort;

String cmd="R"; // valeur envoyé par Arduino

void setup() {
  // création d'une fenêtre de 400×400 pixels
  size(2120, 1280);
  
  // initialisation du port série pour communiquer avec Arduino, mettre la bonne valeur
  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.bufferUntil('\n');
  
  background(0);
  fill(255,0,0); // Red
  ellipse(960,540,800,800);

}

void draw() {
 
//  nothing to do, all are in serial event

}

void serialEvent(Serial myPort) {
  // récupérer la valeur envoyée par Arduino
  String inString = myPort.readStringUntil('\n');
  inString.replace('\n', ' ');
  cmd=trim(inString);
  if (inString != null) { // New data

       println("après:"+cmd+":");
       if ( cmd.equals("R") ) {
         println("red");
          red();
        } else if ( cmd.equals("G") ) {
          println("green");
          green();
       } else if (cmd.equals("M") ) {
         println("magenta");
          magenta();
       } else if (cmd.equals("O") ) {
          println("orange");
         orange();
       }

  }
}


void red() {
  background(0);
  fill(255,0,0); // Red
  ellipse(960,540,800,800);
  String btnpressed = "Stop";
  textSize(64);
  text(btnpressed, 200, 400);
}

void green() {
  background(0);
  fill(64,255,3); // Green
  ellipse(960,540,800,800);
  String btnpressed = "Avancer";
  textSize(64);
  text(btnpressed, 200, 400);
}

void orange() {
  background(0);
  fill(255,104,3); // Orange
  ellipse(960,540,800,800);
  String btnpressed = "Ouverture";
  textSize(64);
  text(btnpressed, 200, 400);
  
}

void magenta() {
  background(0);
  fill(242,3,255); // magenta
  ellipse(960,540,800,800);
  String btnpressed = "Fermeture";
  textSize(64);
  text(btnpressed, 200, 400);
  
}
