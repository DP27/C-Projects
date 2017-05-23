  /************************************************************************************************
  Date on which the project was created :21/11/2013
  Auther: Dishant R Patel Student ID:050665132
  Project name: DC Motor Control and Monitoring System 
  Project Objective: Design and construct a prototype that will control a low power DC motor 
  speed by pressing repeatedly one push- button to increase the speed of the motor and another
  push-button to decrease the speed.The status of the motor speed will be displayed on the PC 
  monitor.Optionally you may produce an audio beep with every button press to provide the user
  with audio feedback [bonus].
  ************************************************************************************************/




# include <hidef.h>
# include "derivative.h"
#define PERIOD 10000 /*for 1msec interrupt generation */

void TPM1C1isr(void);//interrupt service routine for push down button
void TPM1C0isr(void); //interrupt service routine for increasing the speed of the motor
void TPM1C0open(void);//initializing timer1 channel0 for input capture(push up to inc the speed)
void TPM1C1open(void);//initializing timer1 channel1 for input capture(for dec the speed)
void SCI2open (unsigned int Baud);//initializing for SCI communication
void SCI2putch(unsigned char Data);
void SCI2puts(unsigned char *s);
void PWM0open(unsigned char prescale_factor, unsigned int period, unsigned int duty);

volatile unsigned char ch;
volatile unsigned int period,Taskcounter=0,Taskcounter1,i,j,Taskcounter3,period;/*period for varying the beep occurance*/
void TPM2C0open(void);
/*set up timer output compare */
/* TPM1 Channel 0 Interrupt Service Routine (ISR)*/

interrupt void TPM2C0isr(void) {

   TPM2C0V = TPM2C0V + PERIOD; /* sets counts into counter = to previous value + required delay */
                                //Taskcounter1=period;
   Taskcounter3 ++;
   
   if(Taskcounter3 == 150){         //for generating a gap after an interval of 150ms
     TPM2C0SC=0x10;                 //it indicates that the event interrupt is enabled while the output toggle is disabled
   }
   
   if(Taskcounter3 >= period+150){  //for generating the beep after an interval of 2 sec+150ms(gap interval) 
        TPM2C0SC=0x14;               //it indicates that the event interrupt is enabled along with the output toggle
      Taskcounter3=0;              //making the Taskcounter1=0 so that it can again be used to execute a task again             
      
   }
     
    
   
                                                                             
   TPM2C0SC_CH0F = 0; /*clear interrupt flag for C0*/
   TPM2C0SC_CH0IE = 1; /* enable timer interrupt on C0 */

}
void main(void)
{
       SOPT_COPE = 0; /* disable watchdog */
       LEDG = 0x00; /* initially LEDs OFF */
       LEDRL = 0x00;
       period=2000; 
                         SCI2open(2400); //setting the baud rate
                         TPM2SC = 0x08; /* select bus clock for TPM1 :prescale = 1 */
                         TPM2C0open(); /* initialize timer and set up timer output compare */
                         TPM1SC = 0x0C; /* select bus clock for timer 1 - prescale = 8 */
                         TPM1C0open(); /* initialize channel for capture */
                         TPM1C1open();
              EnableInterrupts; /* clear CCR I bit */ 



for ( ; ; )
{SOPT_COPE = 0;  
if(SW_SW7==0){    i=0;
                  TPM1C0SC_CH0IE = 0;
                  TPM2C1SC_CH1IE=0;
                  TPM1C1SC_CH1IE=0;
                  TPM2C0SC_CH0IE=0;
                           
                  TPM1C0SC_CH0F=0;  //disabling all the interrupts and setting the duty cycle to 0% when SW7=0                                  
                  TPM1C1SC_CH1F=0;
                  TPM2C0SC_CH0F=0;
                  TPM2C1SC_CH1F=0;
                  LEDRL=0x00;
                  LEDRH=0x00;
                  PWM0open(0x07,50000,0);
                  Taskcounter=0;       
                              for(j;j<=1;j++){
                              
                              if(SW_SW7==0 && j!=0){
                                
                             
                              SCI2puts("DC Motor status: OFF  "); // send message 
                                SCI2putch(ch); // send character 
                                    SCI2puts("\r\n");j=1;}
} 
}
else if(SW_SW7==1){    j=0;
                      TPM1C0SC_CH0IE = 1;                //enabling all the interrupts when SW7=1
                      TPM2C1SC_CH1IE=1;
                      TPM1C1SC_CH1IE=1;
                      TPM2C0SC_CH0IE=1;
                             for(i;i<=1;i++){
                              
                             if(Taskcounter==0 && i!=0){
                                
                                    SCI2puts("DC Motor status: ON Speed=0% "); // send message 
                                    for(i=0;i<20000;i++){}
                                      
                                    SCI2puts("\r\n"); i=1;
                              }
   }
                             }
                                      }
                                                        }
interrupt void TPM1C0isr(void){
                                   Taskcounter++;
                                       TPM1C0SC_CH0F=0;
   

                    if(Taskcounter==1){ PWM0open(0x07,50000,5000);LEDRL=0x01 ;//motor speed is set to 10% of its total capacity and beep 
                      SCI2puts("DC Motor status: ON Speed=10%"); // send message 
                      SCI2puts("\r\n");                           //beep period is set to 2000
                      period = 2000;}
                      
                       
                    if(Taskcounter==2){ PWM0open(0x07,50000,10000);LEDRL=0x03 ;  //motor speed is inc to 20%
                     SCI2puts("DC Motor status: ON Speed=20% "); // send message 
                     SCI2puts("\r\n");
                     period = 1500;}                             //beep period is set to 1500 i.e. beep period is reduced to indicate that the speed is increased


                    if(Taskcounter==3){ PWM0open(0x07,50000,15000);LEDRL=0x07 ;  //motor speed is inc to 30%
                     SCI2puts("DC Motor status: ON Speed=30% "); // send message 
                     SCI2puts("\r\n");
                     period = 1000;}                             // beep period is set to 1000
                     
                      
                    if(Taskcounter==4){ PWM0open(0x07,50000,20000);LEDRL=0x0F ; //motor speed is inc to 40%
                     SCI2puts("DC Motor status: ON Speed=40% "); // send message 
                     SCI2puts("\r\n");
                     period = 800;}                          // beep period is set to 800
                     
                      
                   if(Taskcounter==5){ PWM0open(0x07,50000,25000);LEDRL=0x1F ;//motor speed is inc to 50% 
                    SCI2puts("DC Motor status: ON Speed=50% "); // send message 
                    SCI2puts("\r\n");
                    period = 700;}                          // beep period is set to 700
                    
                     
                  if(Taskcounter==6){ PWM0open(0x07,50000,30000);LEDRL=0x3F ;  //motor speed is inc to 60%
                   SCI2puts("DC Motor status: ON Speed=60% "); // send message 
                   SCI2puts("\r\n");period = 600;}          // beep period is set to 600
                   
                   
                    
                  if(Taskcounter==7){ PWM0open(0x07,50000,35000);LEDRL=0x7F ;  //motor speed is inc to 70%
                    SCI2puts("DC Motor status: ON Speed=70% "); // send message 
                    SCI2puts("\r\n");period = 500;}          //  beep period is set to 500
                    
                     
                  if(Taskcounter==8){ PWM0open(0x07,50000,40000);LEDRL=0xFF ;  //motor speed is inc to 80%
                   SCI2puts("DC Motor status: ON Speed=80% "); // send message 
                   SCI2puts("\r\n");period = 400;}           //  beep period is set to 400
                   
                   
                  if(Taskcounter==9){ PWM0open(0x07,50000,45000);LEDRL=0xFF ;LEDRH=0x01;//motor speed is inc to 90% 
                    SCI2puts("DC Motor status: ON Speed=90% "); // send message 
                    SCI2puts("\r\n");period = 300;}           // beep period is set to 300


                 if(Taskcounter==10){ PWM0open(0x07,50000,50000);LEDRL=0xFF ;LEDRH=0x03;//motor speed is inc to 100% 
                    SCI2puts("DC Motor status: ON Speed=100% "); // send message 
                    SCI2puts("\r\n");period = 200;}           // beep period is set to 200



if(Taskcounter>10){Taskcounter=10;LEDRL=0xFF ;LEDRH=0x03; }  //if pushed button is pressed more than 10 times than in that case its value would stick to 10

  
                  if(Taskcounter>10 && TPM1C1SC_CH1F==1){Taskcounter=0;LEDRL=0x00;}
}

                 
                 
                 
                 
                 
                 
                 
                     interrupt void TPM1C1isr(void) {
                     
                     
                     
                     Taskcounter1=Taskcounter-1;
                       
                            TPM1C1SC_CH1F=0;
                         
                                if(Taskcounter>=1){
  
 
if(Taskcounter1==65535 && Taskcounter==10){Taskcounter=1;LEDRL=0x00;} 


                        if(Taskcounter1==0){ PWM0open(0x07,50000,0);LEDRL=0x00 ; LEDRH=0x00;//motor speed is reduced to 0%
                          SCI2puts("DC Motor status: ON Speed reduced to 0% "); // send message 
                           SCI2puts("\r\n");period = 2000;}



                        if(Taskcounter1==1){ PWM0open(0x07,50000,5000);LEDRL=0x01 ;LEDRH=0x00;//motor speed is reduced to 10%
                          SCI2puts("DC Motor status: ON Speed reduced to 10% "); // send message 
                          SCI2puts("\r\n");period = 1500;}
                          
                          
                          
                       if(Taskcounter1==2){ PWM0open(0x07,50000,10000);LEDRL=0x03 ;LEDRH=0x00;//motor speed is reduced to 20% 
                          SCI2puts("DC Motor status: ON Speed reduced to 20% "); // send message 
                          SCI2puts("\r\n");period = 1000;}
                          
                          
                          
                       if(Taskcounter1==3){ PWM0open(0x07,50000,15000);LEDRL=0x07 ;LEDRH=0x00; //motor speed is reduced to 30%
                          SCI2puts("DC Motor status: ON Speed reduced to 30% "); // send message 
                          SCI2puts("\r\n");period = 800;}
                          
                          
                          
                       if(Taskcounter1==4){ PWM0open(0x07,50000,20000);LEDRL=0x0F ;LEDRH=0x00;//motor speed is reduced to 40% 
                          SCI2puts("DC Motor status: ON Speed reduced to 40% "); // send message 
                          SCI2puts("\r\n");period = 700;}


                       if(Taskcounter1==5){ PWM0open(0x07,50000,25000);LEDRL=0x1F;LEDRH=0x00; //motor speed is reduced to 50%
                          SCI2puts("DC Motor status: ON Speed reduced to 50% "); // send message 
                          SCI2puts("\r\n");period = 600;}
                          
                          
                          
                       if(Taskcounter1==6){ PWM0open(0x07,50000,30000);LEDRL=0x3F ;LEDRH=0x00; //motor speed is reduced to 60%
                          SCI2puts("DC Motor status: ON Speed reduced to 60% "); // send message 
                          SCI2puts("\r\n");period = 600;}
  
  
  
                       if(Taskcounter1==7){ PWM0open(0x07,50000,35000);LEDRL=0x7F;LEDRH=0x00;  //motor speed is reduced to 70%
                          SCI2puts("DC Motor status: ON Speed reduced to 70% "); // send message 
                          SCI2puts("\r\n");period = 500;}



                       if(Taskcounter1==8){ PWM0open(0x07,50000,40000);LEDRL=0xFF;LEDRH=0x00;  //motor speed is reduced to 80%
                          SCI2puts("DC Motor status: ON Speed reduced to 80% "); // send message 
                          SCI2puts("\r\n");period = 400;}



                       if(Taskcounter1==9){ PWM0open(0x07,50000,45000);LEDRL=0xFF ;LEDRH=0x01; //motor speed is reduced to 90%
                          SCI2puts("DC Motor status: ON Speed reduced to 90% "); // send message 
                          SCI2puts("\r\n");period = 300;}
                                                  
                                                  
                                                   Taskcounter=Taskcounter1;
                      
                      
                       if(Taskcounter1>9){Taskcounter=10;LEDRL=0xFF ;LEDRH=0x01;}

                                                                        } 


                                                                                        }

void TPM1C0open(void)
{
TPM1C0SC = 0x08; /* select input capture mode on both edges */ 
TPM1C0SC_CH0F = 0; /* clear interrupt flag for C0 */ 
TPM1C0SC_CH0IE = 1; /* enable timer interrupt on C0 */
}



void TPM1C1open(void)
{
TPM1C1SC = 0x08; /* select input capture mode on both edges */ 
TPM1C1SC_CH1F = 0; /* clear interrupt flag for C0 */ 
TPM1C1SC_CH1IE = 1; /* enable timer interrupt on C0 */
}



void SCI2open (unsigned int Baud) //argument: Baud rate 
{ 
// Calculate baud rate divisor: Divisor = bus clock/(16 * baud rate) 
SCI2BD= ((20000000L/16)/Baud) & 0xFFFF; 
 SCI2C2=0x0C; // enable Rx and Tx, no interrupts 
// other registers left in default 
}




void SCI2putch(unsigned char Data) 
{ 
while (SCI2S1_TDRE == 0) ; // wait for transmit ready flag TDRE = 1 
SCI2D=Data; //then output a character value 
} 
 
/*output a string of characters via SCI*/ 
/*argument: address of the string*/ 
/*the pointer is declared, it is automatically initialized to the first character of the string*/ 




void SCI2puts(char *s) 
{ 
while (*s) // keep outputting characters till the end of string (NUL) 
 SCI2putch(*s++); // send one character and point to the next character 
}





void PWM0open(unsigned char prescale_factor, unsigned int period, unsigned int duty)
{ 
 TPM1SC = (prescale_factor & 0x07) | 0x08; /* prescale the bus clock */ 
 /* edge aligned output */ 
 TPM1MOD = period - 1; /* PWM period */ 
 TPM1C2SC = 0x28; /* edge aligned PWM */ 
 TPM1C2V = duty; /* set time high so that duty cycle = duty /period *100% */ 
}





void TPM2C0open(void)
{             TPM2C0SC = 0x10; /*select output compare mode + toggle Port D pin 0*/
              TPM2C0V = TPM2CNT+PERIOD; /* sets counts into counter = to current TPM1CNT + required delay */
              TPM2C0SC_CH0F = 0; /*clear interrupt flag for C0*/
              TPM2C0SC_CH0IE = 1; /* enable timer interrupt on C0 */

}

