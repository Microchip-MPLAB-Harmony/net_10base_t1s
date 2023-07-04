/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MIIM_HELPER_H    /* Guard against multiple inclusion */
#define _MIIM_HELPER_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "driver/miim/drv_miim.h"
#include "driver/ethphy/src/dynamic/drv_extphy_lan867x.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	READ, WRITE, IDLE
} Register_Operation_Type;


typedef void (* MIIM_CALLBACK_HANDLE)( uint32_t regAddress, uint16_t data);

DRV_MIIM_RESULT local_miim_init(void);
void local_miim_close(void);
bool local_miim_register_callback(MIIM_CALLBACK_HANDLE cbFunction);
void local_miim_deregister_callback(void);

DRV_MIIM_RESULT phy_write_register( uint32_t regAddr, uint16_t wData);
DRV_MIIM_RESULT phy_read_register( uint32_t regAddr);
uint8_t phy_read_register_result(uint16_t * data);

uint8_t local_miim_task(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MIIM_HELPER_H */

/* ******************************** End of File *******************************/
