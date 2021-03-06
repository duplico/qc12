
/*
 *  ======== _Grace.h ========
 *  DO NOT MODIFY THIS FILE - CHANGES WILL BE OVERWRITTEN
 */

#ifdef __cplusplus
extern "C" {
#endif

/* external peripheral initialization functions */
void Timer1_A3_graceInit(void);
void PinMux_graceInit(void);
void CS_A_graceInit(void);
void EUSCI_A0_graceInit(void);
void EUSCI_A1_graceInit(void);
void EUSCI_B0_graceInit(void);
void PMM_graceInit(void);
void InterruptVectors_graceInit(void);
void System_graceInit(void);
void RTC_B_graceInit(void);
void Grace_init(void);

#ifdef __cplusplus
}
#endif

