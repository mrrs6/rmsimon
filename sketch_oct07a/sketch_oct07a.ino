// ---- Global Declarations ---

enum GameScreens{ START, GAME, LOSE, WIN, ERROR };
GameScreens main_state;
GameScreens prev_state;

// ---- // ----

// ---- Functions that program will need ----

void random_blinking() {
  for(int i=2; i<=6; i++) {
    digitalWrite(i, random(0,2));
  }
}

int sequential_blinking_current_stage = 0;
void sequential_blinking() {
  switch(sequential_blinking_current_stage) {
    case 0:
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      break;
     case 1:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      break;
     case 2:
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      break;
     case 3:
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      break;
  }
  sequential_blinking_current_stage++; // go to the next loop stage
  if(sequential_blinking_current_stage==4) sequential_blinking_current_stage=0; // went too far. go to the beginning of the loop
}

volatile int btn_reset__last_state = HIGH;
volatile bool btn_reset__allowpass = false;
volatile int cycle_counter = 0;

ISR(TIMER1_OVF_vect) { // ISR for 100Hz Timer !
  // 1. Check buttons
  
  switch(main_state) {
    case START:
      if(prev_state!=START) {
        btn_reset__last_state = HIGH; // default value for this variable (waits for btn press).
        btn_reset__allowpass = false;
        cycle_counter = 0;
      }
      if(btn_reset__last_state == HIGH) {
        // Btn was not pressed. A change would be pressing it (LOW).
        if(digitalRead(A0)==HIGH) btn_reset__allowpass = true;
        if(digitalRead(A0)==LOW && btn_reset__allowpass) {
          btn_reset__last_state = LOW;
        } else break;
      } else {
        if(digitalRead(A0)==LOW) {
          if(cycle_counter<10) cycle_counter++; //Stops incrementing at 10 to avoid overflow (and going back to <10)
        } else {
          if(cycle_counter>=10) {
            // Btn pressed long enough
            main_state = GAME;
          }
          cycle_counter=0;
          btn_reset__last_state=HIGH;
        }
      }
      break;
      case LOSE:
      case WIN:
        if(prev_state!=LOSE) {
          btn_reset__last_state = HIGH; // default value for this variable (waits for btn press).
          btn_reset__allowpass = false;
          cycle_counter = 0;
        }
        if(btn_reset__last_state == HIGH) {
          // Btn was not pressed. A change would be pressing it (LOW).
          if(digitalRead(A0)==HIGH) btn_reset__allowpass = true;
          if(digitalRead(A0)==LOW && btn_reset__allowpass) {
            btn_reset__last_state = LOW;
          } else break;
        } else {
          if(digitalRead(A0)==LOW) {
            if(cycle_counter<10) cycle_counter++; //Stops incrementing at 10 to avoid overflow (and going back to <10)
          } else {
            if(cycle_counter>=10) {
              // Btn pressed long enough
              main_state = START;
            }
            cycle_counter=0;
            btn_reset__last_state=HIGH;
          }
        }
        break;
      case GAME:
        if(prev_state!=GAME) {
            btn_reset__last_state = HIGH; // default value for this variable (waits for btn press).
            btn_reset__allowpass = false;
            cycle_counter = 0;
          }
          if(btn_reset__last_state == HIGH) {
            // Btn was not pressed. A change would be pressing it (LOW).
            if(digitalRead(A0)==HIGH) btn_reset__allowpass = true;
            if(digitalRead(A0)==LOW && btn_reset__allowpass) {
              btn_reset__last_state = LOW;
            } else break;
          } else {
            if(digitalRead(A0)==LOW) {
              if(cycle_counter<200) cycle_counter++; //Stops incrementing at 200 to avoid overflow (and going back to <200)
            } else {
              if(cycle_counter>=200) {
                // Btn pressed long enough
                main_state = START;
              }
              cycle_counter=0;
              btn_reset__last_state=HIGH;
            }
          }
          break;
        
    //default:
      // do nothing.
  }
  


  
  TIFR1 = 0x00; //Clear Timer Overflow Flag
  TCNT1  = 64911; //Reset Timer to 0
  
}

// ---- // ----

// ---- Program starts here ----

void setup() {
  // ---- Code to run once, on start ----

  noInterrupts();           // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //int timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  TCNT1 = 64911;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // Define pins A0-A4 as inputs and pins 2-6 as outputs

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  main_state = START;
  prev_state = main_state;

  interrupts();
  
}

void loop() {
  switch(main_state) {
    case START:
      if(prev_state!=START) sequential_blinking_current_stage=0;
      digitalWrite(6, LOW);
      sequential_blinking();
      prev_state = START;
      for (int i=0; i<10 && main_state==START; i++) { delay(50); } // 10x50ms delay
      break;
    case GAME:

      prev_state = GAME;
      break;
    case LOSE:
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      prev_state = LOSE;
      break;
    case WIN:
      random_blinking();
      prev_state = WIN;
      delay(65);
      break;
    default:
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      prev_state = ERROR;
  }

}
