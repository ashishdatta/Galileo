//example use of LCD4Bit_mod library

#include <LCD4Bit_mod.h> 
#include <stdlib.h>

//create object to control an LCD.  
//number of lines in display=1
LCD4Bit_mod lcd = LCD4Bit_mod(2); 

//Key message
char msgs[5][15] = {"Right Key OK ", 
                    "Up Key OK    ", 
                    "Down Key OK  ", 
                    "Left Key OK  ", 
                    "Select Key OK" };
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
char buf[4];
char scorebuff[13];
char pointValChar[13];
int totalScore=100;
int pointValue;
void setup() { 
  
  Serial.begin(115200);
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
  lcd.init();
  
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)

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
  
  //lcd.printIn("The link quality is ");
  
  //shit
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
  
  // shit
  //  lcd.printIn(buf);
    delay(2000);
  
  
  
// BUTTON CONTROL.   
  adc_key_in = analogRead(0);    // read the value from the sensor  
  digitalWrite(13, HIGH);  
  key = get_key(adc_key_in);		        // convert into key press
	
	if (key != oldkey)				    // if keypress is detected
	{
    delay(50);		// wait for debounce time
		adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);		        // convert into key press
    if (key != oldkey)				
    {			
      oldkey = key;
      if (key >=0){
      lcd.cursorTo(2, 0);  //line=2, x=0
  			lcd.printIn(msgs[key]);
      }
    }
  }
  
  //delay(1000);
  digitalWrite(13, LOW);
  

 
  
  
}

// Convert ADC value to key number
int get_key(unsigned int input)
{
	int k;
    
	for (k = 0; k < NUM_KEYS; k++)
	{
		if (input < adc_key_val[k])
		{
           
    return k;
        }
	}
    
    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed
    
    return k;
}

char* itoa(int value, char* result, int base) {
		// check that the base if valid
		if (base < 2 || base > 36) { *result = '\0'; return result; }
	
		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;
	
		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );
	
		// Apply negative sign
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--= *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;
	}
