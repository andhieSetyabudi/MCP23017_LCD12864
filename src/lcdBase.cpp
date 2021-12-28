#include "lcdBase.h"

uint16_t lcdBase::utf8(uint8_t char_utf_8_data)
{
    uint16_t char_utf_8 = 0;
    
    if (char_count_utf_8 == 0){
        
        char_utf_8_1 |= char_utf_8_data;
        char_count_utf_8  = 1;
    }
    else if (char_count_utf_8 == 1){
        
        char_utf_8_2 |= char_utf_8_data;
        char_count_utf_8  = 2;
    }
    if (char_count_utf_8 == 2){
        
        char_utf_8 |= char_utf_8_1;
        char_utf_8 <<= 8;
        char_utf_8 |= char_utf_8_2;
        
        char_utf_8_1 = 0;
        char_utf_8_2 = 0;
        char_count_utf_8 = 0;
        
        return char_utf_8;
    }
    else return 0;
}

/*
uint16_t lcdBase::cp1251(uint16_t char_cp1251_data)
{
    char_cp1251_data -= 32;
    char_cp1251_data = char_cp1251_data * font_size_x;
    return char_cp1251_data;
}
size_t lcdBase::write(uint8_t character)
{
    bool start = false;
    uint16_t character_n = 0;
    
    if(character <= 127){
        character_n = cp1251(character);
        count_char = 0;
        start = true;
    }
    else if (character > 127){
        character_n = utf8(character);
        //53392 53439
        
        if (character_n >= 0xD090 && character_n <= 0xD0BF){
            character_n -= 53200;
        }
        //53632 53647
        else if(character_n >= 0xD180 && character_n <= 0xD18F){
            character_n -= 53392;
        }
        if(character_n != 0){
            character_n = cp1251(character_n);
            count_char = 0;
            start = true;
        }
    }
    
    if (start == true){
        if(character_n == 0) {
            drawFillBox(font_x - 1, font_y, font_size_x + 1, font_size_y, inverting_text);
            font_x += font_size_x;
        }
        
        if (character_n > 0){
            uint8_t temp_data;
            int count_space=0;
            int count_var=0;
            
            for (int count_x = 0; count_x < font_size_x; count_x++ ) {
                if(font_data[count_x + character_n] == 0x0 && count_var == 0) count_space++;
                
                if(font_data[count_x + character_n] != 0x0  ){
                    
                    for (int count_y = 0; count_y < font_size_y; count_y++) {
                        
                        if (inverting_text == 0) temp_data = font_data[count_x + character_n];
                        else
                        if (inverting_text == 1) temp_data = ~font_data[count_x + character_n];
                        
                        temp_data <<= 7 - count_y;
                        temp_data >>= 7;
                        drawPixel(font_x + count_x - count_space , font_y + count_y, temp_data);
                    }
                    
                    count_var = 1;
                    if(font_data[count_x + character_n] != 0x0) char_count += 1;
                }
            }
            
            drawVLine(font_x + char_count, font_y, font_size_y, font_size_x, inverting_text);
            
            font_x += char_count + 1;
        }
    }
}
*/

size_t lcdBase::write(uint8_t character)
{
    if(character - 32 == 0){
        drawVLine(font_x, font_y, font_size_y, font_space, inverting_space);
        font_x += font_space - 1;
    }
    else if(character <= 127){
        write_char(character);
    } else {
        uint16_t character_utf8 = utf8(character);
        
        if (character_utf8 >= 0xD090 && character_utf8 <= 0xD0BF){
            character_utf8 -= 53200;
        }
        else if(character_utf8 >= 0xD180 && character_utf8 <= 0xD18F){
            character_utf8 -= 53392;
        }
        
        if(character_utf8 != 0){
            write_char(character_utf8);
        }
    }
}

void lcdBase::write_char(uint16_t character)
{
    character -= 32;
    uint8_t temp_data;
    uint8_t count_space = 0;
    uint32_t count_var;
        
    for (uint8_t count_x = 0; count_x < font_size_x; count_x++){
        count_var = count_x + font_length_sysinfo + (character * font_size_x);
        
        if (font_data[count_var] != 0){
            for (uint8_t count_y = 0; count_y < font_size_y; count_y++){
                temp_data = ((font_data[count_var] << 7-count_y) >> 7) & 1;
                if(inverting_text) temp_data ^= 1;
                drawPixel(font_x + count_x - count_space, font_y + count_y, temp_data);
            }
            count_char++;
        }
        else if (font_data[count_var + 1] == 0){
            count_space++;
        }
    }
    
    /*
    count_var = font_size_x-1 + font_length_sysinfo + (character * font_size_x);
    if(font_data[count_var] ){
    }
    */
    
    font_x += count_char + font_characters_space;
    drawVLine(font_x - 1, font_y, font_size_y, font_characters_space, inverting_space);
    count_char = 0;
}

void lcdBase::setFont(const uint8_t* data)
{
    font_length_sysinfo = data[0];
    font_characters_space = data[1];
    font_space = data[2];
    font_size_x = data[3];
    font_size_y = data[4];
    
    font_data = (uint8_t *)data;
}

void lcdBase::invertingText(bool _inverting_text, bool _inverting_space)
{
    inverting_text = _inverting_text;
    inverting_space = _inverting_space;
}

void lcdBase::drawText(uint8_t x, uint8_t y, char* data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}

void lcdBase::drawText(uint8_t x, uint8_t y, const String &data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, char data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, const char data[])
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, uint8_t data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, int data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, unsigned int data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, long data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, unsigned long data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}

#if not defined (__AVR__)
void lcdBase::drawText(uint8_t x, uint8_t y, long long data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, unsigned long long data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
#endif

void lcdBase::drawText(uint8_t x, uint8_t y, double data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, const __FlashStringHelper *data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}
void lcdBase::drawText(uint8_t x, uint8_t y, const Printable &data)
{
    count_char = 0;
    font_x = x;
    font_y = y;
    print(data);
    count_char = 0;
}

void lcdBase::resumeDrawText(char* data)                         {  print(data); }
void lcdBase::resumeDrawText(const String & data)                {  print(data); }
void lcdBase::resumeDrawText(char data)                          {  print(data); }
void lcdBase::resumeDrawText(const char data[])                  {  print(data); }
void lcdBase::resumeDrawText(uint8_t data)                       {  print(data); }
void lcdBase::resumeDrawText(int data)                           {  print(data); }
void lcdBase::resumeDrawText(unsigned int data)                  {  print(data); }
void lcdBase::resumeDrawText(long data)                          {  print(data); }
void lcdBase::resumeDrawText(unsigned long  data)                {  print(data); }
#if not defined (__AVR__)
void lcdBase::resumeDrawText(long long data)                     {  print(data); }
void lcdBase::resumeDrawText(unsigned long long data)            {  print(data); }
#endif
void lcdBase::resumeDrawText(double data)                        {  print(data); }
void lcdBase::resumeDrawText(const __FlashStringHelper * data)   {  print(data); }
void lcdBase::resumeDrawText(const Printable& data)              {  print(data); }

void lcdBase::drawPixel(uint8_t x, uint8_t y, uint8_t pixel)
{
    if (x <= 127 && y <= 63) {
        uint8_t x_byte = x/8 ;
        uint8_t x_bit = x%8;
        
        if(pixel){ // if pixel = 1
            
            frame_buffer[y][x_byte] |= (1 << (7 - x_bit));
        }
        else { // if pixel not = 1
            frame_buffer[y][x_byte] &= ~(1 << (7 - x_bit));
        }
    }
}

void lcdBase::drawHLine(uint8_t x, uint8_t y, uint8_t end_line, uint8_t line_width, uint8_t pixel)
{
    for (int i = 0; i < end_line; i++) {
        for (int i2 = 0; i2 < line_width; i2++) {
            drawPixel(x + i, y + i2, pixel);
        }
    }
}

void lcdBase::drawVLine(uint8_t x, uint8_t y, uint8_t end_line, uint8_t line_width, uint8_t pixel)
{
    for (int i = 0; i < end_line; i++) {
        for (int i2 = 0; i2 < line_width; i2++) {
            drawPixel(x + i2, y + i, pixel);
        }
    }
}

void lcdBase::drawBox(uint8_t x, uint8_t y, uint8_t end_x, uint8_t end_y, uint8_t border_width, uint8_t pixel)
{
    drawHLine(x, y, end_x, border_width , pixel);
    drawHLine(x, y + end_y - border_width, end_x, border_width , pixel);
    
    drawVLine(x, y, end_y, border_width , pixel);
    drawVLine(x + end_x - border_width, y, end_y, border_width , pixel);
}

void lcdBase::drawFillBox(uint8_t x, uint8_t y, uint8_t end_x, uint8_t end_y, uint8_t pixel)
{
    drawHLine(x, y, end_x, end_y , pixel);
}

void lcdBase::drawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t pixel)
{
    /*из wikipedia*/
    int x0 = x;
    int y0 = y;
    
    x = radius;
    y = 0;
    int dx = 1 - (radius <<1);
    int dy =  0;
    int err = 0;
    
    while (x >= y)
    {
        drawPixel(x0 + x, y0 + y, pixel);
        drawPixel(x0 + y, y0 + x, pixel);
        drawPixel(x0 - y, y0 + x, pixel);
        drawPixel(x0 - x, y0 + y, pixel);
        drawPixel(x0 - x, y0 - y, pixel);
        drawPixel(x0 - y, y0 - x, pixel);
        drawPixel(x0 + y, y0 - x, pixel);
        drawPixel(x0 + x, y0 - y, pixel);
        
        y++;
        err += dy;
        dy += 2;
        
        if ((err << 1) + dx > 0)
        {
            x--;
            err += dx;
            dx += 2;
        }
    }
}

void lcdBase::drawFillCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t pixel)
{
    int x0 = radius;
    int y0 = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;
    
    while (x0 >= y0)
    {
        for (int i = x - x0; i <= x + x0; i++)
        {
            drawPixel(i, y + y0, pixel);
            drawPixel(i, y - y0, pixel);
        }
        for (int i = x - y0; i <= x + y0; i++)
        {
            drawPixel(i, y + x0, pixel);
            drawPixel(i, y - x0, pixel);
        }
        
        y0++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x0--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void lcdBase::drawBitmap(uint8_t x, uint8_t y, float size_x, uint8_t size_y, uint8_t *bitmap_buffer, uint8_t pixel)
{
    uint8_t x_byte = x/8 ;
    uint8_t x_bit = x%8;
    
    if (x == 0 && y == 0 && size_x == 128 && size_y == 64)
    {
        int i = 0;
        for (int i2 = 0; i2 < 64; i2++) {
            for (int i3 = 0; i3 < 16; i3++) {
                if(pixel == 0) frame_buffer[i2][i3] &= ~bitmap_buffer[i];
                else
                if(pixel == 1) frame_buffer[i2][i3] |= bitmap_buffer[i];
                
                i++;
            }
        }
    }
    else if ((x_bit == 0) && (x + size_x <= 128))
    {
        int i3 = 0 ;
        for (int i = 0; i < size_y - y; i++) {
            for (int i2 = 0; i2 < round((size_x - x) / 8); i2++) {
                
                if(pixel == 0) frame_buffer[i + y][i2 + x / 8 ] &= ~bitmap_buffer[i3];
                else
                if(pixel == 1) frame_buffer[i + y][i2 + x / 8 ] |= bitmap_buffer[i3];
                
                i3++;
            }
        }
    }
    else if ((x_bit != 0) || ((x_bit == 0) && (x + size_x >= 128)))
    {
        uint8_t temp_bitmap;
        int CountBuffer = 0;
        
        for (int CountY = 0; CountY < size_y; CountY++) {
            for (int i = 0; i < round(size_x / 8); i++) {
                for (int i2 = 0; i2 < 8; i2++) {
                    
                    if(pixel == 0) temp_bitmap = ~bitmap_buffer[i + CountBuffer];
                    else
                    if(pixel == 1) temp_bitmap = bitmap_buffer[i + CountBuffer];
                    
                    temp_bitmap <<= i2;
                    temp_bitmap >>= 7;
                    drawPixel(x + i2 + i * 8, y + CountY, temp_bitmap);
                }
            }
            CountBuffer += round(size_x / 8);
        }
    }
}