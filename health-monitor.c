#include <reg51.h>

sbit LED1 = P1^0;
sbit LED2 = P1^1;

int temp = 36;
int heart = 80;
int activity = 0;
int spo2 = 98;
int alert = 0;

void delay()
{
    int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<10;j++);
}

// ----------- INPUT -----------

void read_temp()
{
    temp++;
    if(temp > 40)
        temp = 35;
}

void read_heart()
{
    heart += 5;
    if(heart > 120)
        heart = 70;
}

void read_activity()
{
    activity = !activity;
}

void read_spo2()
{
    spo2--;
    if(spo2 < 90)
        spo2 = 98;
}

// ----------- PROCESS -----------

void process_data()
{
    if(temp > 38 && heart > 110 && spo2 < 94 && activity == 0)
        alert = 4;   // CRITICAL
    else if(spo2 < 94 && activity == 0)
        alert = 3;   // LOW OXYGEN
    else if(temp > 38 && activity == 0)
        alert = 1;
    else if(heart > 110 && activity == 0)
        alert = 2;
    else
        alert = 0;
}

// ----------- OUTPUT -----------

void alert_task()
{
    static int toggle = 0;

    // ACTIVE-LOW: 0 = ON, 1 = OFF

    if(alert == 4)
    {
        LED1 = 0;
        LED2 = 0;   // CRITICAL (steady ON)
    }
    else if(alert == 3)
    {
        toggle = !toggle;

        LED1 = toggle;
        LED2 = toggle;   // LOW OXYGEN (blinking)
    }
    else if(alert == 1)
    {
        LED1 = 0;
        LED2 = 1;   // FEVER
    }
    else if(alert == 2)
    {
        LED1 = 1;
        LED2 = 0;   // HEART
    }
    else
    {
        LED1 = 1;
        LED2 = 1;   // NORMAL
    }
}

// ----------- MAIN -----------

void main()
{
    while(1)
    {
        read_temp(); 
			  delay();
        read_heart(); 
			  delay();
        read_activity(); 
			  delay();
        read_spo2(); 
			  delay();

        process_data(); 
			  delay();
			
        alert_task(); 
			  delay();
    }
}