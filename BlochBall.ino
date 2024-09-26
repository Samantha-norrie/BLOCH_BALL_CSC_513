//www.elegoo.com
//2016.12.09

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

// YELLOW
const int X_PLUS_PIN = 11;
//const int X_MINUS_PIN = 10;

// BLUE
const int Z_ZERO_PIN = 12; 
const int Z_ONE_PIN = 13;

// Values for Bases
const String Z_ZERO = "0";
const String Z_ONE = "1";
const String X_PLUS = "+";
const String X_MINUS = "-";
const String Y_PLUS = "i";
const String Y_MINUS = "-i";

const String VERTICAL[2] = {Z_ZERO, Z_ONE};
const String HORIZONTAL[4] {X_PLUS, Y_PLUS, X_MINUS, Y_MINUS};

// Values for navigation and calc
String initialState = "";
String finalState = "";

// Values for workflow progression
/*
0 -> selecting initial state
1 -> selecting final state
2 -> displaying calculations
*/
int gameState = 0;

String currentPosition = Z_ZERO;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  pinMode(Z_ZERO_PIN, OUTPUT);
  pinMode(Z_ONE_PIN, OUTPUT);
  pinMode(X_PLUS_PIN, OUTPUT);
  // pinMode(X_MINUS_PIN, OUTPUT);
  Serial.begin(9600);


}
void performCalc() {
  Serial.print("performing calc\n ");
  Serial.print(initialState);
  Serial.print(finalState);
  Serial.print("\n");
}

void handleVerticalChange(bool up) {
  if (currentPosition.equals(Z_ZERO)) {
    up? currentPosition = X_MINUS: currentPosition = X_PLUS;
  } else if (currentPosition.equals(Z_ONE)) {
    up? currentPosition = X_PLUS: currentPosition = X_MINUS;
  } else {
    up? currentPosition = Z_ZERO: currentPosition = Z_ONE;
  }
}

void handleHorizontalChange(bool right) {

  if (currentPosition.equals(Z_ZERO)) {
    right? currentPosition = Y_PLUS: currentPosition = Y_MINUS;
  } else if (currentPosition.equals(Z_ONE)) {
    right? currentPosition = Y_MINUS: currentPosition = Y_PLUS;
  } else if (currentPosition.equals(Y_PLUS)) {
    right? currentPosition = X_MINUS: currentPosition = X_PLUS;
  } else if (currentPosition.equals(Y_MINUS)) {
    right? currentPosition = X_PLUS: currentPosition = X_MINUS;
  } else if (currentPosition.equals(X_PLUS)) {
    right? currentPosition = Y_PLUS: currentPosition = Y_MINUS;
  } else {
    right? currentPosition = Y_MINUS: currentPosition = Y_PLUS;
  }
}

void setLight() {
  if (currentPosition.equals(Z_ZERO)) {
    digitalWrite(Z_ZERO_PIN, HIGH);
    digitalWrite(Z_ONE_PIN, (gameState == 1 && initialState == Z_ONE? HIGH: LOW));
    digitalWrite(X_PLUS_PIN, (gameState == 1 && initialState == X_PLUS? HIGH: LOW));
  } else if (currentPosition.equals(Z_ONE)) {
    digitalWrite(Z_ONE_PIN, HIGH);
    digitalWrite(Z_ZERO_PIN, (gameState == 1 && initialState == Z_ZERO? HIGH: LOW));
    digitalWrite(X_PLUS_PIN, (gameState == 1 && initialState == X_PLUS? HIGH: LOW));
  } else if (currentPosition.equals(Y_PLUS)) {
    Serial.print("Y PLUS \n");
  } else if (currentPosition.equals(Y_MINUS)) {
    Serial.print("Y MINUS \n");
  } else if (currentPosition.equals(X_PLUS)) {
    digitalWrite(X_PLUS_PIN, HIGH);
    digitalWrite(Z_ZERO_PIN, (gameState == 1 && initialState == Z_ZERO? HIGH: LOW));
    digitalWrite(Z_ONE_PIN, (gameState == 1 && initialState == Z_ONE? HIGH: LOW));
  } else {
    Serial.print("X MINUS \n");
  }
}

void logInfo() {
  if (gameState == 0) {
    initialState = currentPosition;
    gameState++;
    Serial.print(initialState);
    Serial.print(" INITAL STATE DONE\n");
  } else if (gameState == 1) {
    finalState = currentPosition;
    gameState++;
    Serial.print("FINAL STATE DONE\n");
  } else if (gameState == 2){
    performCalc();
    gameState++;
  } else {
    gameState = 0;
  }
}


void loop() {
  // Serial.print("Switch:  ");
  int press = digitalRead(SW_pin);
  int xCoord = analogRead(X_pin);
  int yCoord = analogRead(Y_pin);

  setLight();

  if (xCoord >= 600) {
    handleHorizontalChange(true);
    // digitalWrite(Z_ZERO_PIN, HIGH);
    // digitalWrite(Z_ONE_PIN, LOW);
    // digitalWrite(X_PLUS_PIN, HIGH);
    // digitalWrite(X_MINUS_PIN, HIGH);

  } else if (xCoord <= 400) {
    handleHorizontalChange(false);
    // digitalWrite(Z_ZERO_PIN, HIGH);
    // digitalWrite(Z_ONE_PIN, LOW);
    // digitalWrite(X_PLUS_PIN, HIGH);

  } else if (yCoord >= 600) {
    handleVerticalChange(true);
    // digitalWrite(Z_ZERO_PIN, LOW);
    // digitalWrite(Z_ONE_PIN, HIGH);
    // digitalWrite(X_PLUS_PIN, HIGH);
  } else if (yCoord <= 400) {
    handleVerticalChange(false);
    // digitalWrite(Z_ZERO_PIN, LOW);
    // digitalWrite(Z_ONE_PIN, HIGH);
    // digitalWrite(X_PLUS_PIN, HIGH);

  } else if (press == 0) {
    Serial.print("press detected\n");
    // digitalWrite(Z_ONE_PIN, LOW);
    // digitalWrite(Z_ZERO_PIN, HIGH);
    // digitalWrite(X_PLUS_PIN, HIGH);

    logInfo();
  }
  Serial.print(currentPosition);
  Serial.print("\n");
  // Serial.print("size");
  // Serial.print(sizeof(gates)/sizeof(gates[0]));
  // Serial.print(digitalRead(SW_pin));
  // Serial.print("\n");
  // Serial.print("X-axis: ");
  // Serial.print(analogRead(X_pin));
  // Serial.print("\n");
  // Serial.print("Y-axis: ");
  // Serial.println(analogRead(Y_pin));
  // Serial.print("\n\n");
  delay(500);
}
