#ifndef lcdbase_h_
#define lcdbase_h_

#include <Arduino.h>
#include <Print.h>

class lcdBase : public Print
{
public:
    uint16_t utf8(uint8_t char_utf_8_data);
    uint16_t cp1251(uint16_t char_cp1251_data);
    
    void write_char(uint16_t character);
    virtual size_t write(uint8_t);

    void setFont(const uint8_t* data);

    void invertingText(bool _inverting_text, bool _inverting_space);
    
    void drawChar(uint8_t x, uint8_t y, char* );
    
    void drawText(uint8_t x, uint8_t y, char* );
    void drawText(uint8_t x, uint8_t y, const String &);
    void drawText(uint8_t x, uint8_t y, char);
    void drawText(uint8_t x, uint8_t y, const char[]);
    void drawText(uint8_t x, uint8_t y, uint8_t);
    void drawText(uint8_t x, uint8_t y, int);
    void drawText(uint8_t x, uint8_t y, unsigned int);
    void drawText(uint8_t x, uint8_t y, long);
    void drawText(uint8_t x, uint8_t y, unsigned long );
    #if not defined (__AVR__)
    void drawText(uint8_t x, uint8_t y, long long);
    void drawText(uint8_t x, uint8_t y, unsigned long long);
    #endif
    void drawText(uint8_t x, uint8_t y, double);
    void drawText(uint8_t x, uint8_t y, const __FlashStringHelper *);
    void drawText(uint8_t x, uint8_t y, const Printable&);

    void resumeDrawText(char*);
    void resumeDrawText(const String &);
    void resumeDrawText(char);
    void resumeDrawText(const char[]);
    void resumeDrawText(uint8_t);
    void resumeDrawText(int);
    void resumeDrawText(unsigned int);
    void resumeDrawText(long);
    void resumeDrawText(unsigned long );
    #if not defined (__AVR__)
    void resumeDrawText(long long);
    void resumeDrawText(unsigned long long);
    #endif
    void resumeDrawText(double);
    void resumeDrawText(const __FlashStringHelper *);
    void resumeDrawText(const Printable&);
    
    void drawPixel(uint8_t x, uint8_t y, uint8_t pixel);
    
    void drawHLine(uint8_t x, uint8_t y, uint8_t end_line, uint8_t line_width, uint8_t pixel);
    void drawVLine(uint8_t x, uint8_t y, uint8_t end_line, uint8_t line_width, uint8_t pixel);
    
    void drawBox(uint8_t x, uint8_t y, uint8_t end_x, uint8_t end_y, uint8_t border_width, uint8_t pixel);
    void drawFillBox(uint8_t x, uint8_t y, uint8_t end_x, uint8_t end_y, uint8_t pixel);
    
    void drawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t pixel);
    void drawFillCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t pixel);
    
    void drawBitmap(uint8_t x, uint8_t y, float size_x, uint8_t size_y, uint8_t *bitmap_buffer, uint8_t pixel);
    
    uint8_t count_char;
    
    uint8_t font_length_sysinfo;
    uint8_t font_characters_space;
    uint8_t font_space;
    uint8_t font_x;
    uint8_t font_y;
    
    uint8_t font_size_x;
    uint8_t font_size_y;
    
    bool inverting_text = 0;
    bool inverting_space = 0;
    uint8_t char_utf_8_1;
    uint8_t char_utf_8_2;
    
    uint16_t char_count_utf_8;
    
    uint8_t* font_data;
    uint8_t frame_buffer[64][16];

};

#endif