#ifndef __APP_LOGCAT_H__
#define __APP_LOGCAT_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG(fmt, arg...)          bt_logcat("DBG", fmt, ##arg)
#define DEBUG_API(fmt, arg...)      bt_logcat("API", fmt, ##arg)

extern void bt_logcat(const char *tag,char *fmt, ...);


#ifdef __cplusplus
}
#endif
#endif /*__APP_LOGCAT_H__*/
