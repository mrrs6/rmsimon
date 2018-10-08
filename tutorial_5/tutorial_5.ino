enum GameScene {START, GAME, WIN, LOSE, ERRO};
GameScene main_state;
GameScene prev_state;

void setup()
{
  // Declarar pins
  // Pins de entrada
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  // Pins de saída
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  main_state = START;
  prev_state = main_state;
}

int piscar_sequencial_nivel_atual = 0; // valor inicial

void piscar_sequencial()
{
    switch(piscar_sequencial_nivel_atual)
    {
      case 0:
       digitalWrite(2,LOW);
       digitalWrite(3,LOW);
       digitalWrite(4,LOW);
       digitalWrite(5,HIGH);
       digitalWrite(6,HIGH);
       break;
      case 1:
       digitalWrite(2,LOW);
       digitalWrite(3,LOW);
       digitalWrite(4,HIGH);
       digitalWrite(5,HIGH);
       digitalWrite(6,LOW);
       break;
      case 2:
       digitalWrite(2,LOW);
       digitalWrite(3,HIGH);
       digitalWrite(4,HIGH);
       digitalWrite(5,LOW);
       digitalWrite(6,LOW);
       break;
      case 3:
       digitalWrite(2,HIGH);
       digitalWrite(3,HIGH);
       digitalWrite(4,LOW);
       digitalWrite(5,LOW);
       digitalWrite(6,LOW);
       break;
      case 4:
       digitalWrite(2,HIGH);
       digitalWrite(3,LOW);
       digitalWrite(4,LOW);
       digitalWrite(5,LOW);
       digitalWrite(6,HIGH);
       break;
    }
    piscar_sequencial_nivel_atual++; //acrescenta sempre 1
    if(piscar_sequencial_nivel_atual==5) // se chegar ao próximo valor que não existe
    {
      piscar_sequencial_nivel_atual = 0; // retorna ao valor inicial
    }
}


void loop()
{
  
  switch(main_state)
  {
    case START:
      if(prev_state!=START) piscar_sequencial_nivel_actual=0; // se o estado anterior não é start, faz reset, i.e., retorna ao valor inicial
      piscar_sequencial();
      for (int i=0; i<10 && main_state==START; i++) { delay(50); } // delay de 10x 50 ms
      prev_state = START; 
      break;
    case GAME:

      prev_state = GAME;
      break;
    case WIN:

      prev_state=WIN;
      break;
    case LOSE:

      prev_state=LOSE;
      break;
    case ERRO:
    default:
      //Erro, corre o default poi não há break
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      prev_state=ERRO;
  }
  
}


