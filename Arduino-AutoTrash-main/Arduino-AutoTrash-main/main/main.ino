#include "src/LiquidCrystal_I2C.h"

// OUTPUT VALUES:
// 0 = No Trash detected
// 1 = Trash detected

const bool debug = true;
const int output = 0;
const int moisture_sensitivity = 600; // less than this is detected
const int trashcan_limit = 10; // less than this in cm is detected
const int sensor_delay = 100;
const int passes = 10;

const int infrared_sensor = 9;
const int moisture_sensor = A0;
const int inductive_sensor = 11;
const int ultrasonic_trigger_1 = 6;
const int ultrasonic_echo_1 = 7;
const int buzzer = 12;


const int stepper_1 = 13;

bool check_array[passes][3];
const int num_bins = 3;
int bins[num_bins] = {0, 0, 0};


// LCD
const int col = 16;
const int row = 2;
LiquidCrystal_I2C lcd(0x27, col, row);
// 4 pins SCL: A5 SDA:A4
// setup: 40 chars per line 16 in display

const char* messages[] = {
  "Arduino Trash                           ",//0
  "Trashcan 1: FULL                        ",//1
  "Trashcan 2: FULL                        ",//2
  "Trashcan 3: FULL                        ",//3
  "Trash Detected !                        ",//4
  "                                        ",//5
  "Detected                                ",//6
  "Dry Waste                               ",//7
  "Wet Waste                               ",//8
  "Metal Waste                             ",//9
  "Error                                   ",//10

};

void setup()
{
  Serial.begin(9600);
  pinMode(infrared_sensor, INPUT);
  pinMode(inductive_sensor, INPUT);
  pinMode(ultrasonic_trigger_1, OUTPUT);
  pinMode(ultrasonic_echo_1, INPUT);
  pinMode(stepper_1, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin();
  if(debug)
    Serial.println("DEBUG!!!");
}

void loop()
{
  bool ir_flag = digitalRead(infrared_sensor) == LOW;
  bool mo_flag = analogRead(moisture_sensor) < moisture_sensitivity;
  bool in_flag = digitalRead(inductive_sensor) == HIGH;
  
  if (ir_flag || mo_flag || in_flag)
    calculate(ir_flag, mo_flag, in_flag);

  delay(1000);
  
  if (debug) {
    Serial.print("infrared : ");
    Serial.println(digitalRead(infrared_sensor));
    Serial.print("moisture: ");
    Serial.println(analogRead(moisture_sensor));
    Serial.print("inductive: ");
    Serial.println(digitalRead(inductive_sensor));
    Serial.print("ultrasonic 1: ");
    Serial.println(check_level(1));
  }
}

void print(int l1 = 0, int l2 = 5){
  lcd.clear();
  lcd.print(messages[l1]);
  lcd.print(messages[l2]);
  delay(sensor_delay);
}

void monitor(){
  if(debug){
    // Serial.println("MONITOR START");
    // delay(1000)
  }
}

void drop(bool open) {
  if(open){
    
  } else {
    
  }
}

void selector(int num){
  switch(num){
    case 1: break;
    case 2: break;
    case 3: break;
    default: break;
  }
}


void calculate(bool ir_flag, bool mo_flag, bool in_flag)
{
  // LOGIC
  if (ir_flag && mo_flag == false && in_flag == false){
    print(7, 6);
    monitor();
  }
  else if (ir_flag && mo_flag && in_flag == false){
    print(8, 6);
    monitor();
  }
  else if (ir_flag && mo_flag == false && in_flag){
    print(9, 6);
    monitor();
  }
  else{
    print(10,5);
  }
  
}

void loop_bins(){
  for(int i = 1; i <= num_bins; i++){
    selector(i);
    bins[i-1] = check_level(i);
  }
}


int check_level(int index) {
  digitalWrite(ultrasonic_trigger_1, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic_trigger_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trigger_1, LOW);
  
  float distance;
  // calibrated
  distance = (pulseIn(ultrasonic_echo_1, HIGH)*.0343)/2;

  int m_flag = 5;
  if(distance < trashcan_limit){
    m_flag = index;
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
  }
  print(0, m_flag);
  return distance;
}
