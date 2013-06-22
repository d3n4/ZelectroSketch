/* States */
#define SETUP_STATE        0x0
#define LOOP_STATE         0x1

/* Actions */
#define CHANGE_STATE       0x0
#define PINMODE            0x1
#define DIGITALWRITE       0x2
#define DIGITALREAD        0x3
#define ANALOGREAD         0x4
#define ANALOGWRITE        0x5

/* Status */
#define STATUS_FAIL        0x0
#define STATUS_OK          0x1

/* Behaviours */
int STATE = SETUP_STATE;
boolean TERMINATE = false;

byte pull(){
  int bytes;
  while((!(bytes=Serial.available()) && !TERMINATE));
  if(bytes > 0)
    return Serial.read();
}

String pullString(int length){
  String buf = "";
  while(buf.length() < length){
      buf += (char)pull();
  }
  return buf;
}

void send(byte data)
{
  Serial.write(data);
}

String toString(byte data){
  String buf = "";
  buf += (char)data;
  return buf;
}

void OK(){ send(STATUS_OK); }
void FAIL(){ send(STATUS_FAIL); }

void pullEvent(){
  OnEvent(pull());
}

void OnEvent(byte action){
  if(STATE == SETUP_STATE){
    if(action == CHANGE_STATE){
      STATE = LOOP_STATE;
      return OK();
    }
    
    if(action == PINMODE)
      return pinMode(pull(), pull());
      
  } else if(STATE == LOOP_STATE){
    if(action == DIGITALWRITE)
      return digitalWrite(pull(), pull());
    if(action == DIGITALREAD)
      return send(digitalRead(pull()));
    if(action == ANALOGREAD)
      return send(analogRead(pull()));  
     if(action == ANALOGWRITE)
      return analogWrite(pull(), pull());  
      
  } else {
    
  }
  FAIL();
}

void setup(){
  Serial.begin(9600);
  OK();
  while(STATE == SETUP_STATE) pullEvent();
}

int done = 0;

void loop(){
  TERMINATE = false;
  pullEvent();
}
