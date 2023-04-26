//80MHz clock
static const uint16_t timer_divider = 80; //divder so clock ticks at 1 MHZ
static const uint64_t timer_max_count = 1000000; 

static const int led_pin = 2;

//esp-32 timer
static hw_timer_t *timer = NULL;

//isr - in ram instead of flash so its faster access
void IRAM_ATTR onTimer()
{
  int pin_state = digitalRead(led_pin);
  digitalWrite(led_pin, !pin_state);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);

  timer = timerBegin(0, timer_divider, true);

  timerAttachInterrupt(timer, &onTimer, true);

  timerAlarmWrite(timer, timer_max_count, true);

  timerAlarmEnable(timer);


}

void loop() {
  // put your main code here, to run repeatedly:

}
