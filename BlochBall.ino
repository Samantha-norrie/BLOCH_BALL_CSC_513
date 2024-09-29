
// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

// YELLOW
const int X_PLUS_PIN = 11;
const int X_MINUS_PIN = 10;

// RED
const int Y_PLUS_PIN = 9;
const int Y_MINUS_PIN = 8;

// BLUE
const int Z_ZERO_PIN = 13; 
const int Z_ONE_PIN = 12;

// Values for Bases
const String Z_ZERO = "Z_ZERO";
const String Z_ZERO_KET = "|0>";
const String Z_ONE = "Z_ONE";
const String Z_ONE_KET = "|1>";
const String X_PLUS = "X_PLUS";
const String X_PLUS_KET = "|+>";
const String X_MINUS = "X_MINUS";
const String X_MINUS_KET = "|->";
const String Y_PLUS = "Y_PLUS";
const String Y_PLUS_KET = "|+i>";
const String Y_MINUS = "Y_MINUS";
const String Y_MINUS_KET = "|-i>";

const char Z = 'Z';
const char Y = 'Y';
const char X = 'X';

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
  pinMode(X_MINUS_PIN, OUTPUT);

  pinMode(Y_PLUS_PIN, OUTPUT);
  pinMode(Y_MINUS_PIN, OUTPUT);

  Serial.begin(9600);


}

String getKetRepresentation(String state) {
  if (state.equals(Z_ZERO)){
    return Z_ZERO_KET;
  } else if (state.equals(Z_ONE)){
    return Z_ONE_KET;
  } else if (state.equals(X_PLUS)){
    return X_PLUS_KET;
  } else if (state.equals(X_MINUS)){
    return X_MINUS_KET;
  } else if (state.equals(Y_PLUS)){
    return Y_PLUS_KET;
  } else {
    return Y_MINUS_KET;
  }
}

String getIntraBasisConversionGate(String state) {
  if (state[0] == "Z") {
    return "X";
  } else {
    return "Z";
  }
}

void performCalc() {
  char initialBasis = initialState[0];
  char finalBasis = finalState[0];

  String gatePattern = "";
  String intermediateState = "";

  // If initial state and final state aren't in the same basis, change initial state to being in the basis of final state
  if (initialBasis != finalBasis) {
    // Serial.print("BASES DONT EQUAL");
    if ((initialBasis == Z && finalBasis == X) || (initialBasis == X && finalBasis == Z)) {
      // Serial.print("1");
      gatePattern = "H";
      if (initialState.equals(Z_ZERO)) {
        intermediateState = X_PLUS;
      } else if (initialState.equals(Z_ONE)) {
        intermediateState = X_MINUS;
      } else if (initialState.equals(X_PLUS)) {
        intermediateState = Z_ZERO;
      } else {
        intermediateState = Z_ONE;
      }
    } else if (initialBasis == Z && finalBasis == Y) {
      // Serial.print("2");
      gatePattern = "SH";
      if (initialState.equals(Z_ZERO)) {
        intermediateState = Y_PLUS;
      } else {
        intermediateState = Y_MINUS;
      }
    } else if (initialBasis == X && finalBasis == Y) {
      gatePattern = "S";
      // Serial.print("3");
      if (initialState.equals(X_PLUS)) {
        intermediateState = Y_PLUS;
      } else {
        intermediateState = Y_MINUS;
      }
    } else if (initialBasis == Y && finalBasis == Z) {
      gatePattern = "S†H";
      // Serial.print("4");
      if (initialState.equals(Y_PLUS)) {
        intermediateState = Z_ZERO;
      } else {
        intermediateState = Z_ONE;
      }
    } else if (initialBasis == Y && finalBasis == X) {
      gatePattern = "S†";
      // Serial.print("5");
      if (initialState.equals(Y_PLUS)) {
        intermediateState = X_PLUS;
      } else {
        intermediateState = X_MINUS;
      }
    }
  } 

  // If states still aren't the same, apply extra gate to flip state
  if (!(initialState.equals(finalState) || intermediateState.equals(finalState))) {
    if (finalBasis == Z) {
      // Serial.print("6");
      gatePattern = "X" + gatePattern;
    } else {
      // Serial.print("7");
      gatePattern = "Z" + gatePattern;
    }
  }
  
  Serial.print("\nCONVERSION\n");
  Serial.print(getKetRepresentation(finalState) + " = " + gatePattern + getKetRepresentation(initialState));
}

void handleVerticalChange(bool up) {
  if (currentPosition.equals(Z_ZERO)) {
    up? currentPosition = X_MINUS: currentPosition = X_PLUS;
  } else if (currentPosition.equals(Z_ONE)) {
    up? currentPosition = X_PLUS: currentPosition = X_MINUS;
  } else {
    up? currentPosition = Z_ONE: currentPosition = Z_ZERO;
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
  digitalWrite(Z_ZERO_PIN, ((currentPosition.equals(Z_ZERO) || (gameState != 0 && initialState == Z_ZERO))? HIGH: LOW));
  digitalWrite(Z_ONE_PIN, ((currentPosition.equals(Z_ONE) || (gameState != 0 && initialState == Z_ONE))? HIGH: LOW));
  digitalWrite(X_PLUS_PIN, ((currentPosition.equals(X_PLUS) || (gameState != 0 && initialState == X_PLUS))? HIGH: LOW));
  digitalWrite(X_MINUS_PIN, ((currentPosition.equals(X_MINUS) || (gameState != 0 && initialState == X_MINUS))? HIGH: LOW));
  digitalWrite(Y_PLUS_PIN, ((currentPosition.equals(Y_PLUS) || (gameState != 0 && initialState == Y_PLUS))? HIGH: LOW));
  digitalWrite(Y_MINUS_PIN, ((currentPosition.equals(Y_MINUS) || (gameState != 0 && initialState == Y_MINUS))? HIGH: LOW));
}

void logInfo() {
  if (gameState == 0) {
    initialState = currentPosition;
    gameState++;
    Serial.print("INITIAL STATE: ");
    Serial.print(getKetRepresentation(initialState));
    Serial.print("\n");
  } else if (gameState == 1) {
    finalState = currentPosition;
    Serial.print("FINAL STATE: ");
    Serial.print(getKetRepresentation(finalState));
    Serial.print("\n");
    performCalc();
    gameState++;
  } else {
    Serial.print("\n\nRESTARTING SESSION\n\n");
    gameState = 0;
    currentPosition = Z_ZERO;
  }
}


void loop() {
  int press = digitalRead(SW_pin);
  int xCoord = analogRead(X_pin);
  int yCoord = analogRead(Y_pin);

  setLight();

  if (xCoord >= 600) {
    handleHorizontalChange(true);
  } else if (xCoord <= 400) {
    handleHorizontalChange(false);
  } else if (yCoord >= 600) {
    handleVerticalChange(true);
  } else if (yCoord <= 400) {
    handleVerticalChange(false);
  } else if (press == 0) {
    logInfo();
  }
  delay(500);
}
