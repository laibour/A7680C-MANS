#ifndef _CORE_AUTH_H_
#define _CORE_AUTH_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>
#include "core_stdinc.h"
#include "core_string.h"
#include "core_sha256.h"
#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
//#include "simcom_os.h"

#define CORE_AUTH_SDK_VERSION "sdk-c-4.0.0"
#define CORE_AUTH_TIMESTAMP   "2524608000000"

//int32_t core_auth_tls_psk(aiot_sysdep_portfile_t *sysdep, char **psk_id, char psk[65], char *product_key, char *device_name, char *device_secret, char *module_name);
int32_t core_auth_mqtt_username(char **dest, char *product_key, char *device_name);
int32_t core_auth_mqtt_password(char **dest, char *product_key, char *device_name, char *device_secret);
int32_t core_auth_mqtt_clientid(char **dest, char *product_key, char *device_name, char *secure_mode, char *extend_clientid);
//int32_t core_auth_http_body(aiot_sysdep_portfile_t *sysdep, char **dest, char *product_key, char *device_name, char *device_secret, char *module_name);

#if defined(__cplusplus)
}
#endif

#endif

