#include "Wire.h"
#include "I2C_LCD12864.h"

I2C_LCD12864 lcd(&Wire);

void setup()
{
    Wire.begin();
    lcd.init();
    lcd.clear(0, 0);
    lcd.clearBuffer();
    
    lcd.setFont(z_font_5x8); 

    lcd.drawText(1,50, "String ");
    lcd.sendBuffer(); // push buffer data to CGRAM
    lcd.setBL_on();
    delay(2000);

}

void loop()
{
    lcd.clear(0,0);
    lcd.clearBuffer();
    lcd.drawText(50,50, "HELLO WORD !");
    lcd.sendBuffer();
    return;
}
