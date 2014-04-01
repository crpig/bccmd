#ifndef __APP_BCSP_H__
#define __APP_BCSP_H__

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
**  Functions
******************************************************************************/
int csr_read_hq(uint16_t *varid, uint8_t *value, uint16_t *length);
int csr_write_bccmd(uint16_t varid, uint8_t *value, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* __APP_BCSP_H__ */

