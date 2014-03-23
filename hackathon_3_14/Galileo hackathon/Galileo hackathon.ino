
#include <LCD4Bit_mod.h> 
#include <stdlib.h>

//create object to control an LCD.  
//number of lines in display=1
LCD4Bit_mod lcd = LCD4Bit_mod(2); 

char buf[4];
char scorebuff[13];
char pointValChar[13];
int totalScore=100;
int pointValue;
void setup() { 
  
  Serial.begin(115200);
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
  lcd.init();
  lcd.clear();
  lcd.printIn("PACMAN v0.1");
    
}
void loop() {
  
  system("/pacman.sh");
  system("hcitool rssi B4:B6:76:F7:61:82 | awk '{print $NF}' | tr -d '\n' > /tmp/output.txt"); //Get the signal stregnth from GHOST BD; awk will provide just the numbers
  
  FILE *fp;
  fp = fopen("/tmp/output.txt", "r"); //open the out from the system call which holds the rssi info 
  fgets(buf, 4, fp);
  fclose(fp);
  
  
  pointValue = atoi(buf); //is from the buffer for rssi info. 
  if(pointValue < -30){
    totalScore -= 1; //is our global score variable 
  }
   else{
   totalScore -= 10;
  } 
   if(pointValue == 0 || totalScore == 0){
    lcd.clear();
    lcd.printIn("Game over!");  
    totalScore = 100;
  }
  
  else{
  lcd.clear(); //Clear so the display so that it only shows one number per second. 
  lcd.printIn("Your score: ");
  lcd.printIn(itoa(totalScore, pointValChar, 10));
  }
  
  delay(2000);
  
}  
  
