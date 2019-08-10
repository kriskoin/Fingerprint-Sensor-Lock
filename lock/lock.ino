
const int LED_PIN_RED = 9;
const int LED_PIN_GREEN = 10;
const int LED_PIN_BLUE = 11;
const int DISPLAY_TIME = 200; //milliseconds

//led status
enum led_status {
  READING,
  ENROLL,
  OK,
  ERR,
  NONE
};

//lock status
enum lock_status {
  LOCK,
  UNLOCK
};


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


void setup() {
 

}

void loop() {
  /*
  led_print_red();
  delay(1000);
  led_print_green();
  delay(1000);
  led_print_blue();
  delay(1000);*/
  led_print_orange();
  delay(1000);
  /*
  led_print_clear();
  delay(1000);*/
}
