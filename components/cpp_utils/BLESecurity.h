/*
 * BLESecurity.h
 *
 *  Created on: Dec 17, 2017
 *      Author: chegewara
 */

#ifndef COMPONENTS_CPP_UTILS_BLESECURITY_H_
#define COMPONENTS_CPP_UTILS_BLESECURITY_H_
#include <esp_gap_ble_api.h>

class BLESecurity {
public:
	BLESecurity();
	virtual ~BLESecurity();
	void setAuthenticationMode(esp_ble_auth_req_t auth_req);
	void setCapability(esp_ble_io_cap_t iocap);
	void setInitEncryptionKey(uint8_t init_key, uint8_t key_size = 16);
	void setRespEncryptionKey(uint8_t resp_key, uint8_t key_size = 16);
	static char* esp_key_type_to_str(esp_ble_key_type_t key_type);

private:
	esp_ble_auth_req_t m_authReq;
	esp_ble_io_cap_t m_iocap;
	uint8_t m_initKey;
	uint8_t m_respKey;
	uint8_t m_keySize;
};

class BLESecurityCallbacks {
public:
	virtual ~BLESecurityCallbacks() {};

	virtual uint32_t onPassKeyRequest() = 0;
	virtual void onPassKeyNotify(uint32_t pass_key);
	virtual bool onSecurityRequest();
	virtual void onAuthenticationComplete(esp_ble_auth_cmpl_t);
};

#endif /* COMPONENTS_CPP_UTILS_BLESECURITY_H_ */
