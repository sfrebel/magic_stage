#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Conceptinetics.h>


#define NUM_PWM_SHIELDS     7
#define PWM_FREQ            1600

Adafruit_PWMServoDriver pwm_shields[NUM_PWM_SHIELDS] = {Adafruit_PWMServoDriver(0x40), Adafruit_PWMServoDriver(0x41), Adafruit_PWMServoDriver(0x42), Adafruit_PWMServoDriver(0x43), Adafruit_PWMServoDriver(0x44), Adafruit_PWMServoDriver(0x45), Adafruit_PWMServoDriver(0x46)};

#define DMX_SLAVE_CHANNELS   ((NUM_PWM_SHIELDS*16) + 0) //all pwm chanels + extra
#define DMX_SLAVE_ADRESS     1

// Pin number to change read or write mode on the DMX shield
#define RXEN_PIN             2



// Configure a DMX slave controller
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS , RXEN_PIN );

const int ledPin = 13;




void setup() {
  
  // Enable DMX slave interface and start recording
  // DMX data
  dmx_slave.enable ();  
  
  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (1);
  
  // Set led pin as output pin
  pinMode ( ledPin, OUTPUT );

  for (int i = 0; i <= NUM_PWM_SHIELDS; i++) {
    pwm_shields[i].begin();
    pwm_shields[i].setPWMFreq(PWM_FREQ);
  }
}

void loop() {
  for (int i = 0; i <= NUM_PWM_SHIELDS; i++) {
    for (int j = 0; j <= 16; j++) {
      int temp = dmx_slave.getChannelValue((i*16) + j + 1); // TODO maybe off by one error
      pwm_shields[i].setPWM(j, map(temp, 0 , 255, 0, 4096), map(temp, 0 , 255, 4096, 0));
    }
  }
}
