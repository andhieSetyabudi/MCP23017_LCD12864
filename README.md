# MCP23017_LCD12864

This library based on MCP23017 ( I2C I/O expander ) that would be driver for LCD12864-V2.0 ( china lcd ) with ST7920 driver ( built-in it )

## LCD 12864-V.20
![LCD top view](https://github.com/andhieSetyabudi/MCP23017_LCD12864/blob/main/img/lcd_to.jpg)
![LCD bottom view](https://github.com/andhieSetyabudi/MCP23017_LCD12864/blob/main/img/lcd_bottom.jpg)

## MCP23017 I2C I/O expander module
![MCP23017 I2C I/O expander module](https://github.com/andhieSetyabudi/MCP23017_LCD12864/blob/main/img/i2c_expander.jpg)
![MCP23017 I2C I/O expander module](https://github.com/andhieSetyabudi/MCP23017_LCD12864/blob/main/img/c9bc9c9e-2c37-45f4-a79a-c4409af56354.jpg)

##Reference ( datasheet )
- [LCD12864](https://www.exploreembedded.com/wiki/images/7/77/QC12864B.pdf)
- [MCP32017](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf)

In parallel 8bit mode, set PSB pin on High state ( for parallel ). 

## Sample for displaying an image
- ![MCP32017](https://github.com/andhieSetyabudi/MCP23017_LCD12864/blob/main/img/img3.jpeg)

Convert the bitmap ( in an even matrix resolution size ) that not exceed than lcd pixel's size
i use LVGL online tools :
[LVGL-tools](https://lvgl.io/tools/imageconverter).
 - Upload your BMP file
 - Set color format to **Indexed 2 colors**
 - check **Output in big-endian format** on the Options
 - then click **convert**

You just need to copy the data array, not all of the existing source inside that files
> in my sample codes
```arduino
...
    lcd.clear(0,0);
    lcd.clearBuffer();
    lcd.drawBitmap(1,1,84,48,(uint8_t*)bitmapArray,1);    // image size : 84x48
    lcd.sendBuffer();**
 ...
```

I would say thank you very much for 

<a href="https://github.com/devMorpheus"><img src="https://user-images.githubusercontent.com/42593803/147535208-82015d09-bc99-444d-a226-36aa698a7073.png" align="left" height="48" width="48"></a>

https://github.com/devMorpheus/zLCD
as reference for LCD7920 driver 
