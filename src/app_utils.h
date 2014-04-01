#ifndef __APP_UTILS_H__
#define __APP_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t BT_writePskey(uint16_t pskey, uint16_t length, uint16_t value[]);
extern uint8_t BT_warmReset(void);

extern void BT_reqlog(uint16_t opcode, void *msg);
extern void BT_cfmlog(uint16_t opcode, void *msg);


#ifdef __cplusplus
}
#endif

#endif /* __APP_UTILS_H__ */

