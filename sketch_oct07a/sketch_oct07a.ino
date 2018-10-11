// ---- Global Declarations ---

enum GameScreens{ START, GAME, LOSE, WIN, ERROR };
volatile GameScreens main_state;
volatile GameScreens prev_state;
volatile int game_scene=1;

// ---- // ----

















#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


// ---- Functions that program will need ----

void playNote(const int note, int duration) {
    tone(8, note, duration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
}

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

volatile bool METRONOME = false;
volatile int METRONOME_STATE = HIGH;
volatile int btn_reset__last_state = HIGH;
volatile bool btn_reset__allowpass = false;
volatile int cycle_counter = 0;
volatile int cycle_counter2 = 0;
volatile bool dbg_print_flag_1 = true;
volatile int quartos_tempos_passados = 0;
volatile bool increment__tempos_passados = false;

ISR(TIMER1_OVF_vect) { // ISR for 100Hz Timer !
  noInterrupts();
  
  // 1. Check buttons
  
  switch(main_state) {
    case START:
      if(prev_state!=START) {
        btn_reset__last_state = HIGH; // default value for this variable (waits for btn press).
        btn_reset__allowpass = false;
        cycle_counter = 0;
        dbg_print_flag_1 = true;
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
          else if(dbg_print_flag_1) {
            Serial.println("ISR [S]: Pressed long enough. Release to switch to GAME.");
            dbg_print_flag_1=false;
          }
        } else {
          if(cycle_counter>=10) {
            // Btn pressed long enough
            main_state = GAME;
            Serial.println("ISR [S]: Switch to GAME.");
          }
          cycle_counter=0;
          btn_reset__last_state=HIGH;
        }
      }
      break;
      case LOSE:
      case WIN:
        if(prev_state!=LOSE && prev_state!=WIN) {
          btn_reset__last_state = HIGH; // default value for this variable (waits for btn press).
          btn_reset__allowpass = false;
          cycle_counter = 0;
          dbg_print_flag_1 = true;
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
            cycle_counter2 = 0;
            dbg_print_flag_1 = true;
          }
          
          if(btn_reset__last_state == HIGH) {
                                  

          if(cycle_counter2>=25) {
            if(cycle_counter2%25==0 && cycle_counter2>0) {
              if(METRONOME) {
                digitalWrite(6, METRONOME_STATE);  
                if(increment__tempos_passados) quartos_tempos_passados++;
              }
            } 
            
            if(cycle_counter2>=50) {
              if(METRONOME) {
                METRONOME_STATE = METRONOME_STATE==HIGH?LOW:HIGH;
                digitalWrite(6, METRONOME_STATE);            
                cycle_counter2=0;
              }
            }

          }
          cycle_counter2++;
          if(cycle_counter2>60) cycle_counter2=0;

          if(game_scene==3) {
            digitalWrite(2, digitalRead(A1)==HIGH?LOW:HIGH);
            digitalWrite(3, digitalRead(A2)==HIGH?LOW:HIGH);
            digitalWrite(4, digitalRead(A3)==HIGH?LOW:HIGH);
            digitalWrite(5, digitalRead(A4)==HIGH?LOW:HIGH);
          }







            // Btn was not pressed. A change would be pressing it (LOW).
            if(digitalRead(A0)==HIGH) btn_reset__allowpass = true;
            if(digitalRead(A0)==LOW && btn_reset__allowpass) {
              btn_reset__last_state = LOW;
            } else break;
          } else {
            if(digitalRead(A0)==LOW) {
              if(cycle_counter<180) {
                cycle_counter++; //Stops incrementing at 180 to avoid overflow (and going back to <180)
              } else {
                digitalWrite(2,LOW);
                digitalWrite(3,LOW);
                digitalWrite(4,LOW);
                digitalWrite(5,LOW);
                digitalWrite(6,LOW);
                if(dbg_print_flag_1) {
                  Serial.println("ISR [G]: Pressed long enough. Release to switch to START.");
                  dbg_print_flag_1 = false; 
                }
              }
            } else {
              if(cycle_counter>=180) {
                // Btn pressed long enough
                main_state = START;
                Serial.println("ISR [G]: Switch to START.");

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
  interrupts();
  
}

// ---- // ----

// ---- Program starts here ----

void setup() {
  // ---- Code to run once, on start ----
  Serial.begin(9600);
  Serial.println("Start...");
  
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

  playNote(NOTE_A4, 150);
  playNote(NOTE_D5, 250);
  Serial.println("Loading START screen ...");
}

int duracoes_4x[4] = {16, 8, 4, 2};

int rd_nivel[8];
int rd_userinput[8];

void loop() {
  switch(main_state) {
    case START:
      if(prev_state!=START) {
        for(int i=0; i<3; i++) delay(10);     
        prev_state = START;
        sequential_blinking_current_stage=0;
        playNote(NOTE_A4, 150);
        game_scene=1;
        Serial.println("Loading START screen ...");
      }
      digitalWrite(6, LOW);
      sequential_blinking();
      for (int i=0; i<10 && main_state==START; i++) { delay(50); } // 10x50ms delay
      break;
    case GAME:
      if(prev_state!=GAME) {
        for(int i=0; i<3; i++) delay(10);     
        prev_state=GAME;   
      }
      switch(game_scene) {
        case 1:
        
          // 1. Entrada.
          digitalWrite(2, HIGH);
          digitalWrite(3, HIGH);
          digitalWrite(4, HIGH);
          digitalWrite(5, HIGH);
          digitalWrite(6, LOW);
          for(int j=0; j<100 && main_state==GAME; j++) delay(10);

          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);

          METRONOME=true;
          for(int i=0; i<200 && main_state==GAME; i++) delay(10);
          increment__tempos_passados = true;
          game_scene++;
        
          break;
        case 2: // Nivel 1
        
          // Preparar jogo.
          for(int i=0; i<8; i++) rd_nivel[i] = duracoes_4x[random(0,4)]; // Gera o nivel
          Serial.print("Seq gerada: "); for(int j=0; j<8; j++) Serial.print(rd_nivel[j]); Serial.println(" ");
          quartos_tempos_passados = 0;

          for(int i=0; i<8; i++) {
            int c = quartos_tempos_passados;
            while((quartos_tempos_passados-c)<(rd_nivel[i]-1)) {
              tone(8, NOTE_A4, 10);
            
              for(int j=0; j<4; j++) {
                if(rd_nivel[i]==16) digitalWrite(2,HIGH); else digitalWrite(2,LOW); 
                if(rd_nivel[i]==8) digitalWrite(3,HIGH); else digitalWrite(3,LOW); 
                if(rd_nivel[i]==4) digitalWrite(4,HIGH); else digitalWrite(4,LOW); 
                if(rd_nivel[i]==2) digitalWrite(5,HIGH); else digitalWrite(5,LOW); 
              }
            }
              while (quartos_tempos_passados-c<rd_nivel[i]) delay(10);
              noTone(8);
              for(int j=0; j<4; j++) {
                digitalWrite(j+2,LOW);
              }
            }

          METRONOME = false;
          game_scene++;
          break;
          
        case 3: // User will press the switches now.

          quartos_tempos_passados = 0;
           
          

          for(int i=0; i<8; i++) {
            Serial.print("-");Serial.println(i);
            int c = quartos_tempos_passados;
            while(digitalRead(A1)==HIGH && digitalRead(A2)==HIGH && digitalRead(A3)==HIGH && digitalRead(A4)==HIGH) { // wait for user to start
              if(quartos_tempos_passados > 40) {
                main_state=LOSE; 
                break;
              }
            }
            if(digitalRead(A1)==LOW && digitalRead(A2)==HIGH && digitalRead(A3)==HIGH && digitalRead(A4)==HIGH) {
              rd_userinput[i]=16;
              Serial.println("16");
            } else if (digitalRead(A1)==HIGH && digitalRead(A2)==LOW && digitalRead(A3)==HIGH && digitalRead(A4)==HIGH) {
              rd_userinput[i]=8;              Serial.println("8");
            } else if (digitalRead(A1)==HIGH && digitalRead(A2)==HIGH && digitalRead(A3)==LOW && digitalRead(A4)==HIGH) {
              rd_userinput[i]=4;               Serial.println("4");
            }  else if (digitalRead(A1)==HIGH && digitalRead(A2)==HIGH && digitalRead(A3)==HIGH && digitalRead(A4)==LOW) {
              rd_userinput[i]=2;              Serial.println("2");
            }
            c = quartos_tempos_passados;
            while(!(digitalRead(A1)==HIGH && digitalRead(A2)==HIGH && digitalRead(A3)==HIGH && digitalRead(A4)==HIGH)){
              if(quartos_tempos_passados-c > 40) {
                main_state=LOSE; 
                break;
              }
            }
            for(int j=0; j<30; j++) delay(10);
          }          

          game_scene++;
          break;
        case 4:
          //Compare input with sequence...
          bool pass = true;
          for(int i=0; i<8; i++) {
            if(rd_nivel[i]!=rd_userinput[i]) pass=false;
          }
          if(pass) main_state=WIN; else main_state=LOSE;
          
          break;
      }






      break;
    case LOSE:
      if(prev_state!=LOSE) {
        playNote(NOTE_C3, 250);
        prev_state=LOSE;
      }
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      break;
    case WIN:
      if(prev_state!=START && digitalRead(A0)==HIGH) { 
        random_blinking();
        playNote(NOTE_C3, 50);
        random_blinking();
        playNote(NOTE_D3, 50);
        random_blinking();
        playNote(NOTE_E3, 50);
        random_blinking();
        playNote(NOTE_F3, 50);
        random_blinking();
        playNote(NOTE_G3, 50);
        random_blinking();
        playNote(NOTE_A3, 50);
        random_blinking();
        playNote(NOTE_B3, 50);
        random_blinking();
        playNote(NOTE_C4, 50);
        random_blinking();
        playNote(NOTE_D4, 50);
        random_blinking();
        playNote(NOTE_E4, 50);
        prev_state = WIN;
      }

      delay(35);
      random_blinking();      
      delay(35);
      random_blinking();
      break;
    default:
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      for(int i=0; i<3; i++) delay(10);     
      prev_state = ERROR;
  }

}
