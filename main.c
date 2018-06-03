
/*
 *
 * Conexion:
 * PE4  -> RS
 * PE5  -> EN
 * PD0  -> D4
 * PD1  -> D5
 * PD2  -> D6
 * PD3  -> D7
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "display.h"
#include "driverlib/rom_map.h"
#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"

#include "driverlib/gpio.h"
#include "driverlib/adc.h"

uint32_t ui32Valor[4];
volatile uint32_t ui32VAvg;
volatile float Voltsprom;
char x[6];
#define ValPLLFREQ0 0X00800060;
#define ValPLLFREQ1 0X00000004;

    volatile uint32_t  ValorADC;
    float ValorADCf;
int main(void)
{

    SYSCTL_PLLFREQ1_R = ValPLLFREQ1;
    SYSCTL_PLLFREQ0_R = ValPLLFREQ0;
    while((SYSCTL_PLLSTAT_R&0x01)==0);

    SYSCTL_RCGCGPIO_R  |= SYSCTL_RCGCGPIO_R4 |SYSCTL_RCGCGPIO_R12|SYSCTL_RCGCGPIO_R3;
    SYSCTL_RCGCADC_R  |= SYSCTL_RCGCADC_R0;


    GPIO_PORTE_AHB_AMSEL_R |=0X08;
    GPIO_PORTE_AHB_DEN_R &= ~0X08;

    GPIO_PORTD_AHB_DATA_R = 0X0;
    GPIO_PORTD_AHB_DEN_R = 0XFF;
    GPIO_PORTD_AHB_DIR_R = 0XFF;

    GPIO_PORTN_DATA_R = 0X0;
    GPIO_PORTN_DEN_R = 0XFF;
    GPIO_PORTN_DIR_R = 0XFF;


    ADC0_SSPRI_R=0X00003210 ;

    ADC0_PC_R=0x07;

    ADC0_ACTSS_R   = 0;

    ADC0_EMUX_R   = 0X0000;
    ADC0_SSEMUX3_R  = 0;
    ADC0_SSMUX3_R  = 0;
    ADC0_SSCTL3_R   |= 2;

    ADC0_ACTSS_R   |= 8;
    SYSCTL_PLLFREQ0_R=0;
    SYSCTL_PLLFREQ1_R=0;


    while(1) {

        ADC0_PSSI_R   |= 8;
        while((ADC0_RIS_R & 8 == 0));
        ValorADC  = ADC0_SSFIFO3_R ;
        ADC0_ISC_R   = 8;

        Voltsprom = (ValorADC*3.3/4095);
        Voltsprom= (Voltsprom*1000.0)/10.0;

        if(Voltsprom>20.0 && Voltsprom<35.0){

            GPIO_PORTD_AHB_DATA_R = 0X00;
            GPIO_PORTN_DATA_R = 0X01;
        }
        else if(Voltsprom<20.0){

            GPIO_PORTN_DATA_R = 0X00;
            GPIO_PORTD_AHB_DATA_R = 0X40;
        }

        else if(Voltsprom>35.0){

            GPIO_PORTN_DATA_R = 0X00;
            GPIO_PORTD_AHB_DATA_R = 0X30;
        }

        int32_t TempFinal = (int32_t)Voltsprom;
        int32_t TempFinalFrac = (int32_t)(Voltsprom * 10.0f);
        TempFinalFrac = TempFinalFrac - (TempFinal * 10);

        if (TempFinalFrac<1){
            TempFinalFrac *=-1;
        }
        if (Voltsprom < 0.0){
            TempFinal *= -1;
            x[0] = '-';

        }else{
            x[0]=' ';
        }

        x[3] = TempFinal % 10 + 48;
        TempFinal = TempFinal / 10;
        x[2] = TempFinal % 10 + 48;
        TempFinal = TempFinal / 10;
        x[1] = TempFinal % 10 + 48;
        TempFinal = TempFinal / 10;

        x[4] = '.';
        x[5] = TempFinalFrac % 10 + 48;

        SysCtlDelay(1000000);
        SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

        initLCD();
        char etiqueta[]="Tempe";
        char etiqueta2[]="ratura:";
        char res[13];
        char *p;
        char *temp;
        strcpy(res,etiqueta);
        strcat(res,etiqueta2);
        p=res;
        temp=x;
        printLCD(p);
        setCursorPositionLCD(1,0);
        printLCD(temp);
        SysCtlDelay(10000000);

        clearLCD();
    }
}
