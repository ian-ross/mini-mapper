extern SCnSCB_Type    *SCnSCB;     /*!< System control Register not in SCB */
extern SCB_Type       *SCB;        /*!< SCB configuration struct */
extern SysTick_Type   *SysTick;    /*!< SysTick configuration struct */
extern NVIC_Type      *NVIC;       /*!< NVIC configuration struct */
extern ITM_Type       *ITM;        /*!< ITM configuration struct */
extern DWT_Type       *DWT;        /*!< DWT configuration struct */
extern TPI_Type       *TPI;        /*!< TPI configuration struct */
extern CoreDebug_Type *CoreDebug;  /*!< Core Debug configuration struct */

#if defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U)
extern MPU_Type       *MPU;        /*!< Memory Protection Unit */
#endif

extern FPU_Type       *FPU;        /*!< Floating Point Unit */
