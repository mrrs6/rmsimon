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
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      break;
     case 1:
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      break;
     case 2:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      break;
     case 3:
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      break;
     case 4:
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      break;
  }
  sequential_blinking_current_stage++; // go to the next loop stage
  if(sequential_blinking_current_stage==5) sequential_blinking_current_stage=0; // went too far. go to the beginning of the loop
}

// ---- // ----

// ---- Program starts here ----

void setup() {
  // ---- Code to run once, on start ----

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
}

void loop() {
  switch(main_state) {
    case START:
      if(prev_state!=START) sequential_blinking_current_stage=0;
      sequential_blinking();
      prev_state = START;
      for (int i=0; i<10 && main_state==START; i++) { delay(50); } // 10x50ms delay
      break;
    case GAME:

      prev_state = GAME;
      break;
    case LOSE:

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
