#include "mbed.h"
#include "LM75B.h"
#include "C12832.h"

LocalFileSystem local("local");     // Create the local filesystem under the name "local"
C12832 lcd(p5, p7, p6, p8, p11);    // set up the LCD
LM75B sensor(p28,p27);              // set up the temperature sensor
Serial pc(USBTX, USBRX);            // set up the serial console
FILE *fp;                           // file pointer for temperature data

Thread logThread;
Thread displayThread;
Thread readThread;

float temp;                     // temperature variable
float min_temp = 100;          // minimum temperature variable
float max_temp = -100;         // maximum temperature variable
float avg_temp = 0;             // average temperature variable
int num_temps = 0;              // number of temperature readings
const int LOG_INTERVAL = 30000; // log temperature every 5 minutes
const int LCD_UPDATE_INTERVAL = 1; // update LCD every 1 second
Timer timer;

void logTemperature()
{
    while(1) {
        fp = fopen("/local/temps.txt", "a");   // open file for temperature data, "a" to append data
        if (fp == NULL) {                      // check if file opened successfully
            lcd.cls();
            lcd.printf("Error: could not open file\n");
            wait_us(1);
            continue;
        }
        temp = sensor.read();           // read temperature
        // update min, max, and avg temperature values
        if (temp < min_temp) {
            min_temp = temp;
        }
        if (temp > max_temp) {
            max_temp = temp;
        }
        avg_temp = (avg_temp * num_temps + temp) / (num_temps + 1);
        num_temps++;

        fprintf(fp, "Max temp: %.2f | Min temp: %.2f | Average temp: %.2f \r\n", max_temp, min_temp, avg_temp);    // write temperature to file
        fflush(fp);     // flush file output buffer
        fclose(fp);
        ThisThread::sleep_for(LOG_INTERVAL);
    }
}

void updateLCD()
{
    while(1) {
        lcd.cls();
        lcd.locate(0,10);
        lcd.printf("Min: %.2f C  Max: %.2f C\n", min_temp, max_temp);
        lcd.locate(0,20);
        lcd.printf("Avg: %.2f C\n", avg_temp);
        ThisThread::sleep_for(LCD_UPDATE_INTERVAL * 1000);
    }
}

void readTemperature()
{
    while(1) {
        temp = sensor.read();
        lcd.cls();
        lcd.printf("Temp: %.2f C\n", temp);
        ThisThread::sleep_for(1000);
    }
}

int main()
{
    logThread.start(logTemperature);
    displayThread.start(updateLCD);
    readThread.start(readTemperature);

printf("Logging thread state: %d\n", logThread.get_state());
printf("Display thread state: %d\n", displayThread.get_state());
printf("Temp read thread state: %d\n", readThread.get_state());


    while (1) {
        // wait for threads to finish
        timer.reset();
    }
}

