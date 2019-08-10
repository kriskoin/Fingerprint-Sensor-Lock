
const int LED_PIN_RED = 9;
const int LED_PIN_GREEN = 10;
const int LED_PIN_BLUE = 11;
const int DISPLAY_TIME = 200; //milliseconds
const int ENROLL_INPUT_PIN = 8;

//led status
enum led_status {
  READING,
  ENROLL,
  ENROLL_COMFIRM,
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
  SYS_ENROLL
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

    case ENROLL_COMFIRM:
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

    }

  switch(st){
    case READING :
      Serial.println("led_st: READING ");
    break;
    
    case ENROLL:
      Serial.println("led_st: ENROLL ");
    break;

    case ENROLL_COMFIRM:
      Serial.println("led_st: ENROLL_COMFIRM ");
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

    }
    debug_status(sys_st,led_st);
}




void setup() {
 //setup the system
 Serial.begin(9600);
 pinMode(ENROLL_INPUT_PIN,INPUT);
 sys_st=SYS_READING;
 update_system(sys_st,led_st);
}
int enroll_input = 0;
void loop() {
  //Read input values
  enroll_input = digitalRead(ENROLL_INPUT_PIN);
  if (enroll_input == HIGH){
       sys_st=SYS_ENROLL;
   }else{
      sys_st=SYS_READING;
   }
  
  update_system(sys_st,led_st);
}
