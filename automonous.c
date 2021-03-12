#include "mcc_generated_files/mcc.h"
#include "pololu3pi.h"
#include "automonous.h"

void pid_state(void);
void differential_state(void);

static  struct sensorval_s sensorvalues;
static  unsigned int sensorReadIndex=0;

void (*autostate)(void) = pid_state;

void followline(void)
{
    sensorReadIndex=0;
    unsigned char pd_mode=1;
    calibrate();
    go_pd(50);    // tell slave to start PID mode
    while(roam_PORT) autostate();

}

void pid_state(void)
{
    sensorvalues = readsensors();

    if ((sensorvalues.s0.word > 250) && (sensorvalues.s4.word>250))
    {
        stop_pd(); // tell slave to stop PID mode
        while(roam_PORT);
    }
    if ((sensorvalues.s1.word > 200) && (sensorvalues.s3.word>200))
    {
        autostate = differential_state;
        stop_pd();
        steer_diff(sensorvalues);
    }

    {
        save_data(sensorvalues, sensorReadIndex);
        sensorReadIndex++;
        if(sensorReadIndex>599) 
        {
            stop_pd();
            while(roam_PORT);
        }
    }
}

void differential_state(void)
{
    sensorvalues = readsensors();
    steer_diff(sensorvalues);  
    if (((sensorvalues.s1.word) < 50) && ((sensorvalues.s3.word) < 50))
    {
        autostate = pid_state;
        forward(0);
        go_pd(50);
    }

    {
        save_data(sensorvalues, sensorReadIndex);
        sensorReadIndex++;
        if(sensorReadIndex>599) 
        {
            forward(0);
            while(roam_PORT);
        }
    }                
}
