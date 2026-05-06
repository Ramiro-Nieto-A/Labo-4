
#include <PingPongBuffer.h>
#include <ports.h>
#include <ADC.h>
#include <math.h>


//#define __CORE_CLOCK__ 20000000U
//#define DAC_DMA_CHANNEL 1
#define PIT_CHANNEL 0
#define DAC_DATL_DATA0_WIDTH 8

#define SIM_SOPT8      (*(volatile uint32_t*)0x40048018)
#define SIM_SOPT8_DAC0TRGSEL_MASK  (0xF << 16)
#define SIM_SOPT8_DAC0TRGSEL(x)    ((x) << 16)
#define SIM_SOPT8_DAC0TRGEN_MASK   (1 << 23)

static volatile uint16_t *adc_buffer0;
static volatile uint16_t *adc_buffer1;

static volatile uint16_t *dac_buffer0;
static volatile uint16_t *dac_buffer1;

static volatile uint16_t buffSize = 1024;
static volatile uint16_t buffMask = 1024 - 1;
static volatile uint16_t buffIndex = 0;
static volatile bool buffer_toggle = false;

typedef DAC_Type *DAC_t;
typedef uint16_t DACData_t;
extern volatile bool nextsample;

void blink_led(void);

void FillBuffers(uint16_t bufferSize)
{
    for (int i = 0; i < bufferSize; i++) {
        float theta = (float)i / (float)bufferSize * 3.14159f * 2.0f;
        dac_buffer0[i] = (uint16_t)(2047 + 2047 * sinf(theta));
        dac_buffer1[i] = (uint16_t)(2047 + 2047 * sinf(2*theta));
    }
}

void PIT0_IRQHandler(void)
{
	TurnON(TP1);
	ADC_Start(ADC0, ADC_0_DP1_CHANNEL, ADC_mA);
	while(!ADC_IsReady(ADC0));
	uint16_t adc_in = ADC_getData(ADC0);
	TurnOFF(TP1);

	PIT->CHANNEL[PIT_CHANNEL].TFLG = PIT_TFLG_TIF_MASK;
	if(buffer_toggle)
	{
		adc_buffer0[buffIndex] =adc_in;
		DAC0->DAT[0].DATL = DAC_DATL_DATA0(dac_buffer0[buffIndex]);
		DAC0->DAT[0].DATH = DAC_DATH_DATA1(dac_buffer0[buffIndex++] >> DAC_DATL_DATA0_WIDTH);
	}
	else
	{
		adc_buffer1[buffIndex] =adc_in;
		DAC0->DAT[0].DATL = DAC_DATL_DATA0(dac_buffer1[buffIndex]);
		DAC0->DAT[0].DATH = DAC_DATH_DATA1(dac_buffer1[buffIndex++] >> DAC_DATL_DATA0_WIDTH);
	}

	buffIndex &= buffMask;
	if(0x00 == buffIndex){
		buffer_toggle = !buffer_toggle;
		if(nextsample == 0)
		{
			nextsample = 1;
		}
	}
	blink_led();
}

void PIT_Init(uint32_t freq)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->CHANNEL[PIT_CHANNEL].LDVAL = (__CORE_CLOCK__ / (2*freq)) - 1;
    PIT->CHANNEL[PIT_CHANNEL].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    NVIC_EnableIRQ(PIT0_IRQn);
}

void DAC_Init (void)
{
	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;
	SIM->SCGC2 |= SIM_SCGC2_DAC1_MASK;

	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK | DAC_C0_DACTRGSEL_MASK;
	DAC1->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK | DAC_C0_DACTRGSEL_MASK;
}

void PingPongBuffer_Init(uint32_t freq,uint16_t bufferSize){

	adc_buffer0 = (uint16_t *)malloc(bufferSize*sizeof(uint16_t));
	adc_buffer1 = (uint16_t *)malloc(bufferSize*sizeof(uint16_t));
	dac_buffer0 = (uint16_t *)malloc(bufferSize*sizeof(uint16_t));
	dac_buffer1 = (uint16_t *)malloc(bufferSize*sizeof(uint16_t));
	FillBuffers(bufferSize);
	ADC_Init();                // Analog to digital converter initialization
	DAC_Init();
	PIT_Init(freq);

	buffSize = bufferSize;
	buffMask = buffSize - 1;
	nextsample = 0;

}

volatile uint16_t * GetBufferIn(){
	return (buffer_toggle)?adc_buffer1:adc_buffer0;
}


volatile uint16_t *  SetBufferOut(){
	return (buffer_toggle)?dac_buffer1:dac_buffer0;
}


void blink_led(void)
{

//	static uint32_t speed=4;     // 0.5 seg @ tick =125ms
	static uint32_t speed=5000;  // 0.5 seg @ tick =100us


	if (speed==0)
	{

		PTB->PTOR = (1<<PIN_BLUE_LED)|(1<<PIN_RED_LED);
		speed=5000;

	}

	speed--;



}


