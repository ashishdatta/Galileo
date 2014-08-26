/***************************************************************************************************************
 * FILE: DNB.ino
 *
 * DESCRIPTION
 * Health Monitoring System for Astronauts!
 *
 * AUTHOR INFORMATION
 * Ashish Datta
 *
 * NOTES
 * Switch pin: D2
 * Temperature Sensor pin: A0
 * Default RGB: 255, 0 ,255
 * Make sure the SWITCH is plugged IN and is in the ON postition before starting health check
 *
 *
 **************************************************************************************************************/

#include <Wire.h>
#include <Math.h>
#include "rgb_lcd.h"

/*CREATE AN LCD OBJECT*/
rgb_lcd lcd;

//==============================================================================================================
// VARIABLE DECLARATION (If you want help put all variables here!)
//==============================================================================================================

/* Edit this section to change the colors of the background */
const int colorR = 255;
const int colorG = 0;
const int colorB = 255;
/* End background color changing */

const int switchPin = 2;       // the number of the pushbutton pin
int switchState     = 0;      // variable for reading the pushbutton status
int B               = 3975;  //B value of the thermistor
int analogData;
float temperature;
float resistance;
//==============================================================================================================
// END VARIABLE DECLARATION
//==============================================================================================================

void setup()
{
  /* Set the switch pin to retrive input: */
  pinMode(switchPin, INPUT);

  /* Set up the LCD's number of columns and rows: */
  lcd.begin(16, 2);

  /* Set the background colors */
  lcd.setRGB(colorR, colorG, colorB);

  /* Print a beginning message to the LCD. */
  lcd.print("Space HealthCheck");

  /* Read the initial state of the switch */
  switchState = digitalRead(switchPin);

  /* THIS IS TO ENSURE ON START UP THAT THE SWITCH IS ON */
  while (switchState == 0) {
    lcd.print("TURN ON POWER!!!");

    /* Keep polling to check the state of the pin */
    switchState = digitalRead(switchPin);
  }
}

void loop()
{
  /* Read the switch to see if ON or OFF*/
  switchState = digitalRead(switchPin);

  //==============================================================================================================
  // Area for temperature sensor code
  //==============================================================================================================

    /* Read data from A0 of the Grove Shield (Change if using a different Anolog
        port*/
    analogData  = analogRead(0);

    /* Retrive the resistance from the temperature sensor*/
    resistance  = (float)(1023-analogData)*10000/analogData;

    /* Convert to temperature via datasheet*/
    temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;

  //==============================================================================================================
  // END OF TEMPERATURE SENSOR CODE
  //==============================================================================================================

  //==============================================================================================================
  // Switch Logic
  //==============================================================================================================

    /* If the switch is ON */
  if (switchState == HIGH ) {

   /* Turn on the display */
   lcd.display();

   /* Set Background color to default color */
   lcd.setRGB(colorR, colorG, colorB);

   /* Clear all text in the display */
   lcd.clear();

   /* Set the cursor to start drawing at col 0: row 0 */
   lcd.setCursor(0,0);
   lcd.print("Space HealthCheck");

   /* Set area to type in Column 0 and row 1 */
   lcd.setCursor(0,1);

   lcd.print("Temp is: ");

   /* Take the temperature in Celsius and convert that to Fahrenheit */
   lcd.print((1.8*temperature)+32);
  }
  else {
    /* if switch is OFF or not plugged in turn off display */
    lcd.setRGB(0,0,0);
    lcd.noDisplay();
  }
  //==============================================================================================================
  // END OF LOGIC FOR SWITCH
  //==============================================================================================================

  /* Choose time in milliseconds to wait before starting loop again */
  delay(1000);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/