#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#define PIN 6


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

void setup(void)
{
  matrix.begin();
  matrix.setBrightness(30);
  matrix.fillScreen(0);
  matrix.show();
}

void loop(void)
{
  matrix.fillScreen(0);
  Snowflake2(matrix.Color(0, 0, 255));
  matrix.show();
  delay(500);
}

void Heart1(uint32_t c) {
  matrix.fillRect(4, 3, 2, 2, c);
}

void Heart2(uint32_t c) {
  matrix.drawLine(3, 3, 3, 4, c);
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawLine(5, 2, 5, 5, c);
  matrix.drawLine(6, 3, 6, 4, c);
}

void Heart3(uint32_t c) {
  matrix.drawLine(2, 1, 2, 2, c);
  matrix.drawLine(2, 5, 2, 6, c);
  matrix.drawLine(3, 0, 3, 7, c);
  matrix.drawLine(4, 0, 4, 7, c);
  matrix.drawLine(5, 1, 5, 6, c);
  matrix.drawLine(6, 2, 6, 5, c);
  matrix.drawLine(7, 3, 7, 4, c);
}

void Heart4(uint32_t c) {
  matrix.drawLine(1, 1, 1, 2, c);
  matrix.drawLine(1, 5, 1, 6, c);
  matrix.drawLine(2, 0, 2, 7, c);
  matrix.drawLine(3, 0, 3, 7, c);
  matrix.drawLine(4, 1, 4, 6, c);
  matrix.drawLine(5, 2, 5, 5, c);
  matrix.drawLine(6, 3, 6, 4, c);
}

void Heart5(uint32_t c) {
  matrix.drawLine(0, 1, 0, 2, c);
  matrix.drawLine(0, 5, 0, 6, c);
  matrix.drawLine(1, 0, 1, 7, c);
  matrix.drawLine(2, 0, 2, 7, c);
  matrix.drawLine(3, 1, 3, 6, c);
  matrix.drawLine(4, 2, 4, 5, c);
  matrix.drawLine(5, 3, 5, 4, c);
}

void Coffee1(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
}

void Coffee2(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
}
void Coffee3(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
}

void Coffee4(uint32_t c) {
  matrix.drawLine(3, 1, 3, 7, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(3, 7, 5, 7, c);
  matrix.drawLine(7, 3, 7, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(4, 6, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(0, 2, c);
  matrix.drawPixel(0, 5, c);
}

void Face1(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
}

void Face2(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
}

void Face3(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
}

void Face4(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(4, 2, c);
  matrix.drawPixel(4, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(5, 5, c);
}

void Face5(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 6, c);
}

void Face6(uint32_t c) {
  matrix.drawLine(0, 2, 0, 5, c);
  matrix.drawLine(7, 2, 7, 5, c);
  matrix.drawLine(2, 7, 3, 7, c);
  matrix.drawLine(2, 0, 3, 0, c);
  matrix.drawLine(4, 0, 5, 0, c);
  matrix.drawLine(5, 7, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawLine(5, 3, 5, 4, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 6, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 6, c);
}

void Snowflake1(uint32_t c) {
  matrix.drawPixel(1, 0, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 3, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawLine(3, 1, 3, 2, c);
  matrix.drawLine(3, 4, 3, 5, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 4, c);
  matrix.drawPixel(4, 7, c);
  matrix.drawLine(5, 1, 5, 2, c);
  matrix.drawLine(5, 4, 5, 5, c);
  matrix.drawPixel(6, 3, c);
  matrix.drawPixel(6, 5, c);
  matrix.drawPixel(7, 0, c);
  matrix.drawPixel(7, 3, c);
  matrix.drawPixel(7, 6, c);
}

void Snowflake2(uint32_t c) {
  matrix.drawLine(0, 2, 0, 3, c);
  matrix.drawLine(0, 5, 0, 6, c);
  matrix.drawLine(1, 0, 1, 1, c);
  matrix.drawLine(1, 5, 1, 6, c);
  matrix.drawLine(3, 1, 3, 2, c);
  matrix.drawLine(3, 4, 3, 5, c);
  matrix.drawLine(4, 2, 4, 3, c);
  matrix.drawLine(4, 5, 4, 6, c);
  matrix.drawLine(6, 1, 6, 2, c);
  matrix.drawLine(6, 6, 6, 7, c);
  matrix.drawLine(7, 1, 7, 2, c);
  matrix.drawLine(7, 4, 7, 5, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(2, 1, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(2, 7, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawPixel(5, 0, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 6, c);
  matrix.drawPixel(6, 4, c);
  matrix.drawPixel(2, 3, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(5, 4, c);
  matrix.drawPixel(5, 2, c);
}

void Snowflake3(uint32_t c) {
  matrix.drawLine(3, 2, 3, 4, c);
  matrix.drawLine(4, 2, 4, 3, c);
  matrix.drawLine(4, 5, 4, 6, c);
  matrix.drawLine(5, 2, 5, 4, c);
  matrix.drawPixel(1, 2, c);
  matrix.drawPixel(1, 4, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 4, c);
  matrix.drawPixel(2, 6, c);
  matrix.drawPixel(3, 0, c);
  matrix.drawPixel(3, 6, c);
  matrix.drawPixel(5, 0, c);
  matrix.drawPixel(5, 6, c);
  matrix.drawPixel(6, 2, c);
  matrix.drawPixel(6, 4, c);
  matrix.drawPixel(6, 6, c);
  matrix.drawPixel(7, 2, c);
  matrix.drawPixel(7, 4, c);
}
void Snowflake4(uint32_t c) {
  matrix.drawPixel(1, 0, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawLine(2, 3, 2, 5, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawLine(4, 1, 4, 3, c);
  matrix.drawLine(4, 5, 4, 7, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawLine(6, 3, 6, 5, c);
  matrix.drawPixel(7, 0, c);
  matrix.drawPixel(7, 3, c);
  matrix.drawPixel(7, 6, c);
}
void Snowflake5(uint32_t c) {
  matrix.fillRect(0, 3, 2, 2, c);
  matrix.fillRect(3, 3, 2, 2, c);
  matrix.fillRect(6, 3, 2, 2, c);
  matrix.drawLine(3, 0, 3, 1, c);
  matrix.drawLine(3, 6, 3, 7, c);
  matrix.drawLine(4, 0, 4, 1, c);
  matrix.drawLine(4, 6, 4, 7, c);
  matrix.drawPixel(1, 1, c);
  matrix.drawPixel(1, 6, c);
  matrix.drawPixel(2, 2, c);
  matrix.drawPixel(2, 5, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(5, 2, c);
  matrix.drawPixel(6, 1, c);
  matrix.drawPixel(6, 6, c);
}

void Snowflake6(uint32_t c) {
  matrix.drawLine(2, 2, 2, 3, c);
  matrix.drawLine(2, 5, 2, 6, c);
  matrix.drawLine(4, 3, 4, 5, c);
  matrix.drawLine(6, 2, 6, 3, c);
  matrix.drawLine(6, 5, 6, 6, c);
  matrix.drawPixel(1, 3, c);
  matrix.drawPixel(3, 1, c);
  matrix.drawPixel(3, 3, c);
  matrix.drawPixel(3, 5, c);
  matrix.drawPixel(4, 1, c);
  matrix.drawPixel(4, 7, c);
  matrix.drawPixel(5, 1, c);
  matrix.drawPixel(5, 3, c);
  matrix.drawPixel(5, 5, c);
  matrix.drawPixel(7, 3, c);
}

void heartAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Heart1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Heart5(c);
  matrix.show();
  delay(500);
}
void coffeeAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Coffee1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Coffee4(c);
  matrix.show();
  delay(500);
}

void happyAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face4(c);
  matrix.show();
  delay(500);
}

void sadAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face6(c);
  matrix.show();
  delay(500);
}

void emotionsAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Face4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face6(c);
  matrix.show();
  delay(500);
  Face5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Face3(c);
  matrix.show();
  delay(500);
}

void snowflakesAnimation(uint32_t c) {
  matrix.fillScreen(0);
  Snowflake1(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake2(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake3(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake4(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake5(c);
  matrix.show();
  delay(500);
  matrix.fillScreen(0);
  Snowflake6(c);
  matrix.show();
  delay(500);
}
