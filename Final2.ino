long long motorStepTime = 0, ingTime=0, optionsTime;
int mixRotationNum = 0, numTurning = 0;
char egg = 0, vanilla=1, suger=2, flour=3, arm=4, oven=5, raspberry=6, pineapple=7, flourNum=0, curStep = 0, mixerSpeed=3, motor[] = {A0, A2, A1, A3};
char phase=0, /*0=>making, 1=>baking, 2=decorating*/  motorType = 0; //0=>mixer, 1=>line, 2=>revolver 
char option;
bool start = true, addedVanila = false, addedSuger=false, mixerOn=true, valveOn=false;
bool motorSteps[] = {HIGH, LOW, LOW, LOW, LOW, LOW, HIGH, LOW, LOW, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH};
void setup() {
  DDRD = B11111111; //0 => 7
  pinMode(8, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  openValve(egg);
  delay(520);
  closeValve(egg);
  
  Serial.begin(9600);
}

void loop() {
  if(phase == 0)
    makeCake();
    
  else if(phase == 1){
    bakeCake();
  }
    
  else if(phase == 2)
    decorateCake();
}
void decorateCake(){
  option = readInput();
  if(option == 1){
    option1();
  }
  if(option == 2){
    option2();
  }
  if(option == 3){
    option3();
  }
  if(option == 4){
    option4();
  }
}
void option1(){
  optionsTime = millis();
  digitalWrite(raspberry, HIGH);
  //decorates in 2 seconds
  controlValve(arm, 1600);
  while(millis()-optionsTime <= 2000){
    if((millis()-motorStepTime>=62)){
      rotate();
    }
  }
  controlValve(arm, 1800);
  digitalWrite(pineapple, HIGH);
  digitalWrite(raspberry, LOW);
  optionsTime = millis();
  while(millis()-optionsTime <= 2000){
    if((millis()-motorStepTime>=62)){
      rotate();
    }
  }
  digitalWrite(pineapple, LOW);
  controlValve(arm, 1500);
  option = 5;
}
void option2(){
  optionsTime = millis();
  char i=0;
  bool fill = true;
  digitalWrite(pineapple, LOW);
  controlValve(arm, 1600);
  while(millis()-optionsTime <= 2000){
    if(millis()-optionsTime >= i*250){
      if(fill)
        digitalWrite(raspberry,HIGH);
      else
        digitalWrite(raspberry,LOW);
      i++;
      fill = !fill;
    }
    if((millis()-motorStepTime>=62)){
      rotate();
    }
  }
  optionsTime = millis();
  digitalWrite(raspberry,LOW);
  fill = true;
  i=0;
  controlValve(arm, 1800);
  while(millis()-optionsTime <= 2000){
    if(millis()-optionsTime >= i*250){
      if(fill)
        digitalWrite(pineapple, HIGH);
      else
        digitalWrite(pineapple, LOW);
      fill = !fill;
      i++;
    }
    if((millis()-motorStepTime>=62)){
      rotate();
    }
  }
  digitalWrite(pineapple, LOW);
  digitalWrite(raspberry, LOW);
  controlValve(arm, 1500);
  option = 5;
}
void option3(){
  optionsTime = millis();
  bool fill = true;
  char i=0;
  digitalWrite(pineapple, LOW);
  controlValve(arm, 1600);
  //decorates in 15 seconds I increased the time so testing becomes possible
  while(millis()-optionsTime <= 14000){
    if(millis()-optionsTime >= i*500){
      if(fill)
        digitalWrite(raspberry, HIGH);
      else
        digitalWrite(raspberry, LOW);
      fill = !fill;
      i++;
    }
    if((millis()-motorStepTime>=437)){
      rotate();
    }
  }
  digitalWrite(raspberry, LOW);
  controlValve(arm, 2000);
  delay(500);
  digitalWrite(pineapple, HIGH);
  delay(500);
  digitalWrite(pineapple, LOW);
  controlValve(arm, 1500);
  option = 5;
}
void option4(){
  optionsTime = millis();
  char fill = 0;
  bool pine = true;
  char i=0;
  controlValve(arm, 1600);
  //decorates in 15 seconds I increased the time so testing becomes possible
  while(millis()-optionsTime <= 14000){
    if(millis()-optionsTime >= i*500){
      if(fill%2==0){
        if(pine){
          digitalWrite(raspberry, LOW);
          digitalWrite(pineapple, HIGH);
        }
        else{
          digitalWrite(raspberry, HIGH);
          digitalWrite(pineapple, LOW);
        }
        pine = !pine;
      }
      else{
        digitalWrite(raspberry, LOW);
        digitalWrite(pineapple, LOW);
      }
      fill++;
      i++;
    }
    if((millis()-motorStepTime>=437)){
      rotate();
    }
  }
  digitalWrite(pineapple, LOW);
  controlValve(arm, 1900);
  optionsTime = millis();
  digitalWrite(raspberry, HIGH);
  while(millis()-optionsTime <= 2000){
    if((millis()-motorStepTime>=62)){
      rotate();
    }
  }
  controlValve(arm, 2000);
  delay(500);
  digitalWrite(raspberry, LOW);
  controlValve(arm, 1500);
  option = 5;
}
char readInput(){
  curStep = 0;
  digitalWrite(8, HIGH);
  while(true){
    if(digitalRead(12))
      return 1;
      
    if(digitalRead(11))
      return 2;
    
    if(digitalRead(10))
      return 3;
    
    if(digitalRead(9))
      return 4;
    
  }
}
void bakeCake(){
  //one rotation/sec
  for(int i=0; i<32; i++){
    rotate();
    if(i != 31)
      delay(31);
  }
  digitalWrite(oven, HIGH);
  delay(1000);
  digitalWrite(oven, LOW);
  for(int i=0; i<32; i++){
    rotate();
    if(i != 31)
      delay(31);
  }
  delay(1000);
  motorType++;
  curStep=0;
  phase++; 
}
void makeCake(){
  if(motorType==0 && mixerOn && (millis()-motorStepTime >= mixerSpeed)){
    rotate();
    if(numTurning == 32){
      mixRotationNum++;
      numTurning = 0;
    }
    if(mixRotationNum == 9 && !addedSuger){
      mixerOn = false;
    }
    if(mixRotationNum == 19){
      mixerSpeed = 6;
    }
    if(mixRotationNum == 31){
      mixerOn = false;
      motorType++;
      phase++;
      mixRotationNum = 0;
      numTurning = 0;
      curStep = 0;
    }
  }
  //adding Vanila
  if(!addedVanila && mixRotationNum >= 5){
    
    addIng(vanilla,100);
  }
  //adding suger
  if(addedVanila && !addedSuger){
    addIng(suger, 200);
  }
  //adding flour the mixer willstop while addingI
  if(mixRotationNum == (19+ 4*flourNum) && flourNum<3){
    openValve(flour);
    delay(100);
    closeValve(flour); 
    flourNum++;
  }
}
void controlValve(char pin, int degree){
  digitalWrite(pin, HIGH);
  delayMicroseconds(degree);
  digitalWrite(pin, LOW);
}
void openValve(char pin){
  controlValve(pin, 2000);
  valveOn = true;
}
void closeValve(char pin){
  controlValve(pin, 1500);
  valveOn = false;
}
void rotate(){
  if(motorType == 0){
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);
  }
  else if(motorType == 1){
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);
  }
  else if(motorType == 2){
    digitalWrite(A4, LOW);
    digitalWrite(A5, HIGH);
  }
  
  if(curStep == 16){
    curStep = 0;
  }
  char i = 0;
  while(i<4){
    digitalWrite(motor[i], motorSteps[curStep]);
    i++;
    curStep++;
  }
  motorStepTime = millis();
  numTurning++;
}
void addIng(char pinNumber, int duration){
  if(!valveOn){
      openValve(pinNumber);
      ingTime = millis();
    }
    if(millis()-ingTime >= duration){
      closeValve(pinNumber);
      if(pinNumber == vanilla)
        addedVanila = true;
      else if(pinNumber == suger){
        closeValve(suger);
        addedSuger = true;
        mixerOn = true;
      }
    }
}
