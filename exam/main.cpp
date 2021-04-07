#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);
AnalogIn Ain(D6);
AnalogOut Aout(D7);
DigitalIn b1(D10);
DigitalIn b2(D11);
DigitalIn b3(D12);
 

EventQueue eventqueue(32 * EVENTS_EVENT_SIZE); //t.start(callback(&queue, &EventQueue::dispatch_forever));

using namespace std;
Thread t,tp;//t.start(function);


float cs=0.5;// 1/8
float base=80.00;
/*
void work(){    
    printf("select:%f",cs);
}
*/

void g(){
    uLCD.cls();
    uLCD.printf("%f",cs);
   
    while(1){   //240m-base*cs =mid time
        for(float i=0.0;i<0.909;i+=(0.909/(base*cs))){  //   0.909/80
            Aout=i;
            ThisThread::sleep_for(1ms);
        }
        float a=float(240-(2*base*cs))/1000;
        ThisThread::sleep_for(a);
        for(float i=0.909;i>0;i-=(0.909/(base*cs))){
            Aout=i;
            ThisThread::sleep_for(1ms);
        }
    }

}
/*
void ISR3(){
    eventqueue.call(sampling);
}
*/

void sampling(){//20hz
    ThisThread::sleep_for(8s);
    float adcdata;
    while(1){
        adcdata=Ain;
        printf("%f\r\n",3.3*adcdata);
        ThisThread::sleep_for(50ms);
    }



}


int main(){

    t.start(callback(&eventqueue, &EventQueue::dispatch_forever));
    eventqueue.call(sampling);
    //tp.start(sampling);
   // b3.rise(ISR3);
    while(1){
        if(b1.read()){
            if(cs!=1.000){
                cs*=2; 
            }
            uLCD.cls();
            uLCD.printf("%f",cs);
        }
        if(b2.read()){
            if(b2.read()){
                if(cs!=0.125){
                    cs/=2;
                }
            }
            uLCD.cls();
            uLCD.printf("%f",cs);
        }        
        if(b3.read()){
 
            while(1){   //240m-base*cs =mid time
                for(float i=0.0;i<0.909;i+=(0.909/(base*cs))){  //   0.909/80
                 Aout=i;
                 ThisThread::sleep_for(1ms);
                }
                float a=float(240-(2*base*cs));
                ThisThread::sleep_for(a);
                for(float i=0.909;i>0;i-=(0.909/(base*cs))){
                    Aout=i;
                    ThisThread::sleep_for(1ms);
                }
            }

        }
        
    }


}





void work(){
    cs=1/8;
    uLCD.printf("%f",cs);
}

void ISR1(){

    eventqueue.call(work);

}
