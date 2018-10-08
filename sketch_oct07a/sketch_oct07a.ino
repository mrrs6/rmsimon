// ---- Global Declarations ---

enum GameScreens{ START, GAME, LOSE, WIN, ERROR };
GameScreens main_state;
GameScreens prev_state;

// ---- // ----

// ---- Functions that program will need ----

void random_blinking() {
  while(true) {
    for(int i=2; i<=6; i++) {
      digitalWrite(i, random(0,2));
    }
    delay(65);
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

}

void loop() {
  switch(main_state) {
    case START:
      sequential_blinking();
      delay(500); // ESTE DELAY devia ser partido pq o estado pode mudar
      break;
    case GAME:
      break;
    case LOSE:
      break;
    case WIN:
      random_blinking();
      break;
    default:
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
  }

}















/*

const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];

int velocity = 1000;

void loop()
{
if (level == 1)
generate_sequence();//generate a sequence;

if (digitalRead(A4) == LOW || level != 1) //If start button is pressed or you're winning
{
show_sequence();    //show the sequence
get_sequence();     //wait for your sequence
}
}

void show_sequence()
{
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);

for (int i = 0; i < level; i++)
{
digitalWrite(sequence[i], HIGH);
delay(velocity);
digitalWrite(sequence[i], LOW);
delay(200);
}
}

void get_sequence()
{
int flag = 0; //this flag indicates if the sequence is correct

for (int i = 0; i < level; i++)
{
flag = 0;
while(flag == 0)
{
if (digitalRead(A0) == LOW)
{
digitalWrite(5, HIGH);
your_sequence[i] = 5;
flag = 1;
delay(200);
if (your_sequence[i] != sequence[i])
{
wrong_sequence();
return;
}
digitalWrite(5, LOW);
}

if (digitalRead(A1) == LOW)
{
digitalWrite(4, HIGH);
your_sequence[i] = 4;
flag = 1;
delay(200);
if (your_sequence[i] != sequence[i])
{
wrong_sequence();
return;
}
digitalWrite(4, LOW);
}

if (digitalRead(A2) == LOW)
{
digitalWrite(3, HIGH);
your_sequence[i] = 3;
flag = 1;
delay(200);
if (your_sequence[i] != sequence[i])
{
wrong_sequence();
return;
}
digitalWrite(3, LOW);
}

if (digitalRead(A3) == LOW)
{
digitalWrite(2, HIGH);
your_sequence[i] = 2;
flag = 1;
delay(200);
if (your_sequence[i] != sequence[i])
{
wrong_sequence();
return;
}
digitalWrite(2, LOW);
}

}
}
right_sequence();
}

void generate_sequence()
{
randomSeed(millis()); //in this way is really random!!!

for (int i = 0; i < MAX_LEVEL; i++)
{
sequence[i] = random(2,6);
}
}
void wrong_sequence()
{
for (int i = 0; i < 3; i++)
{
digitalWrite(2, HIGH);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);
delay(250);
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
delay(250);
}
level = 1;
velocity = 1000;
}

void right_sequence()
{
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
delay(250);

digitalWrite(2, HIGH);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);
delay(500);
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, LOW);
digitalWrite(5, LOW);
delay(500);

if (level < MAX_LEVEL);
level++;

velocity -= 50; //increase difficulty
}

*/
