struct Button {
  const uint8_t PIN;
  volatile uint32_t numberKeyPresses;
  volatile bool pressed;
};
Button button1 = {18, 0, false};

hw_timer_t * timer = NULL;

//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTime();
void IRAM_ATTR isr();

boolean state = HIGH;
void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);  
}


//Called on the first negedge and then susequently starts the timer interrupt
//A negedge can be a pre-emption for a short press, a long press or an EMI interference
//That is decided in the timer interrupts
void IRAM_ATTR isr() {
    detachInterrupt(button1.PIN);
  // Configure Prescaler to 80, as our timer runs @ 80Mhz
  // Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second
  timer = timerBegin(0, 80, true);                
  timerAttachInterrupt(timer, &onTime, true);    
  // Fire Interrupt every 1m ticks, so 1s
  timerAlarmWrite(timer, 1000, true);      
  timerAlarmEnable(timer);
}

volatile int hold = 0;
volatile long int cold = 0;
volatile long int told = 0;
volatile int lng_press = 0;

void IRAM_ATTR onTime() {
  //portENTER_CRITICAL_ISR(&timerMux);
  //Serial.print(" In timer interrupt \n");
  int interrupt_val = digitalRead(button1.PIN);
  if(interrupt_val == HIGH)
  {
    cold = 0;
    hold++;
  
  }
    else{
      hold = 0;
      cold++ ; 
      told++;
    }
  if(hold == 16){    
  if(lng_press == 1){//This is to avoid a false button signal while letting go of the long button press
  timerAlarmDisable(timer);
  timerDetachInterrupt(timer);  // detach interrupt
  timerEnd(timer);
  lng_press = 0;
  attachInterrupt(button1.PIN, isr, FALLING);//Restarting entire process
    
    }
  else{//This is the area where we get a short button press  
  Serial.print(" Button pressed \n");
  state = !state;
  digitalWrite(0, state);
  
  Serial.print(" Number of times it was down \n"+String(told));//To identify if the negedge was caused by emi
  told = 0;
  
  timerAlarmDisable(timer);
  timerDetachInterrupt(timer);  // detach interrupt
  timerEnd(timer);
  attachInterrupt(button1.PIN, isr, FALLING);//Restarting entire process
    }
  }
  else{//This is the area we get a long button press
        if(cold == 1000){
            lng_press = 1;
            told = 0;
            Serial.print(" Very long button pressed \n");
          }   
    }
  
  //portEXIT_CRITICAL_ISR(&timerMux);
}


void loop() {

      Serial.println("-");
      delay(1000);
     
 
}
