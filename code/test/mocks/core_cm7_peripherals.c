#include "stm32f767xx.h"
#include "core_cm7.h"

static SCnSCB_Type scnscb;
SCnSCB_Type *SCnSCB = &scnscb;

static SCB_Type scb;
SCB_Type *SCB = &scb;

static SysTick_Type systick;
SysTick_Type *SysTick = &systick;

static NVIC_Type nvic;
NVIC_Type *NVIC = &nvic;

static ITM_Type itm;
ITM_Type *ITM = &itm;

static DWT_Type dwt;
DWT_Type *DWT = &dwt;

static TPI_Type tpi;
TPI_Type *TPI = &tpi;

static CoreDebug_Type coredebug;
CoreDebug_Type *CoreDebug = &coredebug;

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U)
static MPU_Type mpu;
MPU_Type *MPU = &mpu;
#endif

static FPU_Type fpu;
FPU_Type *FPU = &fpu;
