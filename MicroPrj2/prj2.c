

#include <io.h>
#include <string.h>
#include <mega32.h>
#include <delay.h>
#include <lcd.h>
#include <stdlib.h>


#asm
  .equ __lcd_port = 0x1B;
#endasm

// Define step sequences for two stepper motor phases
flash unsigned char step1_val[]={0x01 ,0x03, 0x02, 0x06, 0x04,0x0c,0x08 ,0x09};
flash unsigned char step2_val[]={0x09,0x08 ,0x0c, 0x04, 0x06, 0x02 ,0x03,0x01};

unsigned char xPosition = 0; 
unsigned char correctAnswers = 0; 

int step=0 , i=0;

interrupt [EXT_INT0] void ext_int0_isr(void) {
    if (PIND.2 == 0) // key pressed
    {
        if (xPosition < 16) {  // Button pressed on the first row of the keypad
            correctAnswers++;
            if (step > 0) {     // Decrease step if not already at the minimum 
               step -= 2;     // Decrement step to reverse direction of the stepper motor

            }
        }
        
        else{ 
          step++;
        }
    }
}

interrupt [EXT_INT2] void ext_int2_isr(void) {
    if (PINB.2 == 0) {
        if (xPosition >= 16){  // Button pressed on the second row of the keypad
            correctAnswers++;
             if (step > 0) {  // Decrease step if not already at the minimum 
                step -= 2;  

            }
        } 
          else{ 
          step++;   // Increment step to advance the stepper motor
        }
    }
}


  
void main(void)
{
DDRA=255;
DDRC=0x0F;
DDRD.2=0;
DDRB.2=0;
srand(42);

lcd_init(16);

 // Display initial messages on the LCD
  lcd_gotoxy(0, 0);
  lcd_puts("Fateme Mosavinjd");
  delay_ms(100);
  lcd_clear(); 
  
  lcd_gotoxy(0, 0);
  lcd_puts("Are you ready?");
  delay_ms(100);
  lcd_clear();

  
  lcd_gotoxy(1, 1);
  lcd_putsf("3");
  delay_ms(100);
  lcd_clear();

  
  lcd_gotoxy(1, 1);
  lcd_putsf("2");
  delay_ms(100);
  lcd_clear();

 
  lcd_gotoxy(1, 1);
  lcd_putsf("1");
  delay_ms(100);
  lcd_clear(); 
  
  
  GICR |=1<<INT0;     // Enable external interrupt 0
  GICR |=1<<INT2;    // Enable external interrupt 2
  #asm ("sei")       // Enable global interrupts
       
 
  
  while(1){      
    
   unsigned char y = rand() % 2;  // Generate a random row (0 or 1)
   xPosition = rand() % 32;  // Generate a random column position (0 to 31)  
    
    // Determine the row based on the column position
   if (xPosition < 16) {
    y = 0;
    } else {
        y = 1;
    }   
    
   // Display "X" at the randomly generated position on the LCD 
   lcd_gotoxy(xPosition % 16, y);
   lcd_puts("X");
   delay_ms(110);
   lcd_clear();   
   
    // Drive the stepper motor with the selected step sequence
    for(i=0; i<=25; i++) {
        PORTC = step2_val[step];
    }
    delay_ms(1);     
   

    if (step == 0) {
        for(i=0; i<=25; i++) {
            PORTC = step1_val[step];
        }                                   
        delay_ms(1);
    }

    step++;
    if (step >= 7) {
        step = 0;     // Reset step to 0 when it reaches the maximum value
    }

    
  
  
  }
  



}


