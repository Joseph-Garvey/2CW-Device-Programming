#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

InterruptIn button(USER_BUTTON);
Timeout button_debounce_timeout;
float debounce_time_interval = 0.5;

int t=0;
const int N = 6;
int led_cycle[N]={4,4,4,4,4,4};

Serial pc(SERIAL_TX, SERIAL_RX);

void onButtonStopDebouncing(void);

void onButtonPress(void){
    for(int i=0; i<N; i++){
        if(led_cycle[i] == 4){ 
            led_cycle[i] = t;
            led1=true;
            led2=true;
            led3=true;
            if(led_cycle[i]==0){led1=false;}
            else if(led_cycle[i]==1){led2=false;}
            else if(led_cycle[i]==2){led3=false;}
            i=N; // wasn't working?
            break;
        }
    }
    button.rise(NULL);
    button_debounce_timeout.attach(onButtonStopDebouncing, debounce_time_interval);
}

void onButtonStopDebouncing(void){
    button.rise(onButtonPress);
}

void select_led(int l)
{
        if (l==0) {
                led1 = true;
                led2 = false;
                led3 = false;
        }
        else if (l==1) {
                led1 = false;
                led2 = true;
                led3 = false;
        }
        else if (l==2) {
                led1 = false;
                led2 = false;
                led3 = true;
        }
        else if (l==3){ // if i later want a confirmation flash for debugging
                led1 = false;
                led2 = false;
                led3 = false;
        }
}

int main() {
    button.rise(onButtonPress);
    pc.baud(9600);
    while(true) {
        if(led_cycle[N-1] != 4){
            pc.printf("Playback: LED CYCLE %d LED %d \r\n",t ,led_cycle[t]);
            select_led(led_cycle[t]);
            wait(0.5);
            t=(t+1)%N;
        }
        else {
            select_led(t);
            pc.printf("Cycle %d \r\n", t);
            wait(1);
            t=(t+1)%3;
        }
    }
}