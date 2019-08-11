
#include <Adafruit_Fingerprint.h>
#include <Servo.h>


const int LED_PIN_RED = 9;
const int LED_PIN_GREEN = 10;
const int LED_PIN_BLUE = 11;
const int DISPLAY_TIME = 200; //milliseconds
const int ENROLL_INPUT_PIN = 8;
const int SERVO_PIN = 5;
//led status
enum led_status {
  READING,
  ENROLL,
  ENROLL_CONFIRM,
  OK,
  ERR,
  NONE
};

//lock status
enum lock_status {
  LOCK,
  UNLOCK
};


//system status
enum systems_status {
  SYS_READING,
  SYS_FINGER_OK,
  SYS_FINGER_ERR,
  SYS_ENROLL,
  SYS_ENROLL_CONFIRM
};

//gobal variables
enum systems_status sys_st;
enum led_status led_st;

/*
 * Setup the rgb values for the led
*/
void set_led(int r,int g, int b,int delay_time){
  analogWrite(LED_PIN_RED,r);
  analogWrite(LED_PIN_GREEN,g);
  analogWrite(LED_PIN_BLUE,b);
  delay(delay_time);
}

void led_print_red(){
  set_led(255,0,0,DISPLAY_TIME);
}
void led_print_orange(){
  set_led(200,29,0,DISPLAY_TIME);
}
void led_print_green(){
  set_led(0,255,0,DISPLAY_TIME);
}
void led_print_blue(){
  set_led(0,0,255,DISPLAY_TIME);
}

void led_print_clear(){
  set_led(0,0,0,DISPLAY_TIME);
}

void update_led(enum led_status st ){

  switch(st){
    case READING :
      led_print_clear();
    break;
    
    case ENROLL:
      led_print_blue();
    break;

    case ENROLL_CONFIRM:
      led_print_orange();
    break;
    
    case OK:
      led_print_green();
    break;
    
    case ERR:
      led_print_red();
    break;
    
    case NONE:
      led_print_clear();
    break;
    
    default:
      led_print_clear();
    break;
    }
}

void debug_status(enum systems_status sys_st,enum led_status st ){

  switch(sys_st){
    case SYS_READING :
      Serial.print("sys_st: SYS_READING ");
    break;
    
    case SYS_ENROLL:
      Serial.print("sys_st: SYS_ENROLL ");
    break;

    case SYS_ENROLL_CONFIRM:
      Serial.print("sys_st: SYS_ENROLL_CONFIRM ");
    break;

    }

  switch(st){
    case READING :
      Serial.println("led_st: READING ");
    break;
    
    case ENROLL:
      Serial.println("led_st: ENROLL ");
    break;

    case ENROLL_CONFIRM:
      Serial.println("led_st: ENROLL_CONFIRM ");
    break;
    
    case OK:
      Serial.println("led_st: OK ");
    break;
    
    case ERR:
      Serial.println("led_st: ERR ");
    break;
    
    case NONE:
      Serial.println("led_st: NONE ");
    break;
    
    
    }
}

void update_system(enum systems_status sys_st,enum led_status led_st ){
 
  switch(sys_st){
    case SYS_READING :
      led_st= READING;
      update_led(led_st);
    break;
    
    case SYS_ENROLL:
      led_st= ENROLL;
      update_led(led_st);
    break;

    case SYS_ENROLL_CONFIRM:
      led_st= ENROLL_CONFIRM;
      update_led(led_st);
    break;

    case SYS_FINGER_OK:
      led_st= OK;
      update_led(led_st);
    break;
    
    case SYS_FINGER_ERR:
      led_st= ERR;
      update_led(led_st);
    break;

    }
    //debug_status(sys_st,led_st);
}


// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo servo;
int servo_pos =0;

void setup() {
 //setup the system
 Serial.begin(9600);

 // set the data rate for the sensor serial port
 finger.begin(57600);
 
 //finger.emptyDatabase();
 pinMode(ENROLL_INPUT_PIN,INPUT);
 sys_st=SYS_READING;
 sys_st=SYS_ENROLL_CONFIRM;
 update_system(sys_st,led_st);
 //servo.attach(SERVO_PIN);
 //TODO: init servo position
 
}

int enroll_input = 0;
uint8_t id;
int finger_id = 0;

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void loop() {
  //Read input values
  
  enroll_input = digitalRead(ENROLL_INPUT_PIN);
  if (enroll_input == HIGH){
       sys_st=SYS_ENROLL;
       update_system(sys_st,led_st);
       Serial.println("Ready to enroll a fingerprint!");
       Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
       id = readnumber();
       if (id == 0) {// ID #0 not allowed, try again!
           return;
       }
       Serial.print("Enrolling ID #");
       Serial.println(id);
       while (!  getFingerprintEnroll() );
   }else{
      sys_st=SYS_READING;
      update_system(sys_st,led_st);
      finger_id=getFingerprintIDez();
      delay(50);            //don't ned to run this at full speed.
      if(finger_id>0){
        //set the led green and send and ok signal
        sys_st=SYS_FINGER_OK;
        update_system(sys_st,led_st);
       // set_servo_unlock_position();
      }else{
        //set the led red
        //sys_st=SYS_FINGER_ERR;
        //update_system(sys_st,led_st);
      }
   }
  
 
 
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}


// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

void set_servo_unlock_position(){
  for(servo_pos =0; servo_pos< 180; servo_pos +=1){
    servo.write(servo_pos);
    delay(10);
  }
}
