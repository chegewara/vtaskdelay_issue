/**
 * Create a new BLE server.
 */
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
//#include "BLESecurity.h"
#include <esp_log.h>
#include <string>
#include <Task.h>


#include "sdkconfig.h"

static char LOG_TAG[] = "SampleServer";
BLEService *pService;
BLEService *pService1;
  BLECharacteristic *reportChar1;
  BLECharacteristic *reportChar2;
  BLECharacteristic *reportChar3;

class MyTask : public Task {
	void run(void*){
    	//vTaskDelay(10000);
		while(1){
			uint8_t a[] = {0x0, 0x0, 0x13, 0x0,0x0,0x0,0x0,0x0};
			reportChar1->setValue(a,sizeof(a));
			reportChar1->notify();
			uint8_t v[] = {0x0, 0x0, 0x0, 0x0,0x0,0x0,0x0,0x0};

			reportChar1->setValue(v, sizeof(v));
			reportChar1->notify();
			vTaskDelay(2000);
		}
	}
};
  MyTask *task;

  class MyCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer){
      task->start();
    }

    void onDisconnect(BLEServer* pServer){
      task->stop();
    }
  };

  class MySecurity : public BLESecurityCallbacks {

  	uint32_t onPassKeyRequest(){
  		return 123456;
  	}
  	void onPassKeyNotify(uint32_t pass_key){
          ESP_LOGE(LOG_TAG, "The passkey Notify number:%d", pass_key);
  	}
  	bool onSecurityRequest(){
  		return true;
  	}
  	void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl){
  		if(auth_cmpl.success){
  			ESP_LOGI(LOG_TAG, "remote BD_ADDR:");
  			esp_log_buffer_hex(LOG_TAG, auth_cmpl.bd_addr, sizeof(auth_cmpl.bd_addr));
  			ESP_LOGI(LOG_TAG, "address type = %d", auth_cmpl.addr_type);
  		}
        ESP_LOGI(LOG_TAG, "pair status = %s", auth_cmpl.success ? "success" : "fail");
  	}
  };

class MainBLEServer: public Task {
	void run(void *data) {
		ESP_LOGD(LOG_TAG, "Starting BLE work!");

		task = new MyTask();
		  BLEDevice::init("ESP32");
		  BLEServer *pServer = BLEDevice::createServer();
		  pServer->setCallbacks(new MyCallbacks());
		  pService = pServer->createService((uint16_t)0x180a);
		  pService1 = pServer->createService((uint16_t)0x1812, 30);
		pServer->setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);
		pServer->setSecurityCallbacks(new MySecurity());

		  BLECharacteristic *manufacturer = pService->createCharacteristic(
		                                       (uint16_t)0x2a29,
		                                       BLECharacteristic::PROPERTY_READ
		                                     );
		  std::string name = "chegewara";
		  manufacturer->setValue(name);

		  BLECharacteristic *pnpIDChar = pService->createCharacteristic(
		                                       (uint16_t)0x2a50,
		                                       BLECharacteristic::PROPERTY_READ
		                                     );
		  const uint8_t pnp[] = {0x01,0xe5,0x02,0xcd,0xab,0x01,0x00};
		  pnpIDChar->setValue((uint8_t*)pnp, sizeof(pnp));

		  BLECharacteristic *hidInfoChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4a,
		                                       BLECharacteristic::PROPERTY_READ
		                                     );
		  const uint8_t val1[] = {0x01,0x11,0x00,0x03};
		  hidInfoChar->setValue((uint8_t*)val1, 4);

		  BLECharacteristic *reportMapChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4b,
		                                       BLECharacteristic::PROPERTY_READ
		                                     );
		  reportMapChar->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);
		  const uint8_t val2[] = {
		    0x05,0x01,
			0x09,0x06,
			0xA1,0x01,
			0x05,0x07,
		    0x19,0xE0,
			0x29,0xE7,
			0x15,0x00,
			0x25,0x01,
		    0x75,0x01,
			0x95,0x08,
			0x81,0x02,
			0x95,0x01,
		    0x75,0x08,
			0x81,0x01,
			0x95,0x05,
			0x75,0x01,
		    0x05,0x08,
			0x19,0x01,
			0x29,0x05,
			0x91,0x02,
		    0x95,0x01,
			0x75,0x03,
			0x91,0x01,
			0x95,0x06,
		    0x75,0x08,
			0x15,0x00,
			0x25,0x65,
			0x05,0x07,
		    0x19,0x00,
			0x29,0x65,
			0x81,0x00,
			/*
			0x09, 0x05,                 //     Usage (Vendor Defined)
			0x15, 0x00,                 //     Logical Minimum (0)
			0x26, 0xFF, 0x00,           //     Logical Maximum (255)
			0x75, 0x08,                 //     Report Count (2)
			0x95, 0x02,                 //     Report Size (8 bit)
			0xB1, 0x02,
			*/
			0xC0}; //TODO
		  reportMapChar->setValue((uint8_t*)val2, sizeof(val2));

		  BLECharacteristic *hidControlChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4c,
		                                       BLECharacteristic::PROPERTY_WRITE_NR
		                                     );
		  hidControlChar->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);
		  reportChar1 = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4d,
		                                       BLECharacteristic::PROPERTY_READ   |
		                                       BLECharacteristic::PROPERTY_NOTIFY
		                                     );
		  BLEDescriptor *desc1 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
		  const uint8_t desc1_val[] = {0x01, 0};
		  desc1->setValue((uint8_t*)desc1_val, 1);
		  reportChar1->addDescriptor(desc1);
		  reportChar1->addDescriptor(new BLE2902());
		  reportChar1->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);

		  reportChar2 = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4d,
		                                       BLECharacteristic::PROPERTY_READ   |
		                                       BLECharacteristic::PROPERTY_WRITE
		                                     );
		  BLEDescriptor *desc2 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
		  const uint8_t desc2_val[] = {0x02, 0};
		  desc2->setValue((uint8_t*)desc2_val, 1);
		  reportChar2->addDescriptor(desc2);
		  reportChar2->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

		  reportChar3 = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4d,
		                                       BLECharacteristic::PROPERTY_READ     |
		                                       BLECharacteristic::PROPERTY_WRITE    |
		                                       BLECharacteristic::PROPERTY_WRITE_NR
		                                     );
		  BLEDescriptor *desc3 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
		  const uint8_t desc3_val[] = {0x03, 0};
		  desc3->setValue((uint8_t*)desc3_val, 1);
		  reportChar3->addDescriptor(desc3);
		  reportChar3->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

		  BLECharacteristic *protocolModeChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a4e,
		                                       BLECharacteristic::PROPERTY_WRITE_NR
		                                     );
		  protocolModeChar->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);

		  BLECharacteristic *bootInputChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a22,
		                                       BLECharacteristic::PROPERTY_NOTIFY
		                                     );
		  bootInputChar->addDescriptor(new BLE2902());


		  BLECharacteristic *bootOutputChar = pService1->createCharacteristic(
		                                       (uint16_t)0x2a32,
		                                       BLECharacteristic::PROPERTY_READ     |
		                                       BLECharacteristic::PROPERTY_WRITE    |
		                                       BLECharacteristic::PROPERTY_WRITE_NR
		                                     );
		  bootOutputChar->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);



		  pService->start();
		  pService1->start();
		  BLEAdvertising *pAdvertising = pServer->getAdvertising();
		  pAdvertising->setAppearance(961);
		  pAdvertising->addServiceUUID((uint16_t)0x1812);
		  pAdvertising->start();

		BLESecurity *pSecurity = new BLESecurity();
		pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
		pSecurity->setCapability(ESP_IO_CAP_NONE);
		pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
		//pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK | ESP_BLE_CSR_KEY_MASK);


		ESP_LOGD(LOG_TAG, "Advertising started!");
		delay(1000000);
	}
};


void SampleHID(void)
{
	//esp_log_level_set("*", ESP_LOG_DEBUG);
	MainBLEServer* pMainBleServer = new MainBLEServer();
	pMainBleServer->setStackSize(20000);
	pMainBleServer->start();

} // app_main
