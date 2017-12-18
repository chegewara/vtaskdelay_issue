/**
 * Main file for running the BLE samples.
 */
extern "C" {
	void app_main(void);
}


// The list of sample entry points.
void Sample_MLE_15(void);
void Sample1(void);
void SampleClient(void);
void SampleClient_Notify(void);
void SampleClientAndServer(void);
void SampleClientDisconnect(void);
void SampleClientWithWiFi(void);
void SampleNotify(void);
void SampleRead(void);
void SampleScan(void);
void SampleSecureClient(void);
void SampleSecureServer(void);
void SampleSensorTag(void);
void SampleServer(void);
void SampleWrite(void);
void SampleHID(void);

//
// Un-comment ONE of the following
//            ---
void app_main(void) {
	//Sample_MLE_15();
	//Sample1();
	//SampleClient();
	//SampleClient_Notify();
	//SampleClientAndServer();
	//SampleClientDisconnect();
	//SampleClientWithWiFi();
	//SampleSecureClient();
	//SampleNotify();
	//SampleRead();
	//SampleSensorTag();
	//SampleScan();
	//SampleServer();
	//SampleSecureServer();
	//SampleWrite();
	SampleHID();
} // app_main
