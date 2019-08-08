
const int LED_PIN_RED = 9;
const int LED_PIN_GREEN = 10;
const int LED_PIN_BLUE = 11;
const int DISPLAY_TIME = 100; //millisconds

/*
 * Setup the rgb values for the led
*/
void set_led(int r,int g, int b,int delay_time){
  analogWrite(LED_PIN_RED,r);
  analogWrite(LED_PIN_GREEN,g);
  analogWrite(LED_PIN_BLUE,b);
  delay(delay_time);
}

void led_print_blue(){
  Serial.println("print blue");
  set_led(0,0,255,DISPLAY_TIME);
}

void setup() {
 

}

void loop() {
  led_print_blue();

}
