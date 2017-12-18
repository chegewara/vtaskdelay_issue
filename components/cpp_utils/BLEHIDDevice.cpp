/*
 * BLEHIDDevice.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: chegewara
 */
//#include "BLEUUID.h"
#include "BLEHIDDevice.h"

BLEHIDDevice::BLEHIDDevice() {
	// TODO Auto-generated constructor stub

}

BLEHIDDevice::~BLEHIDDevice() {
	// TODO Auto-generated destructor stub
}

void BLEHIDDevice::begin() {

}

void BLEHIDDevice::setReportMap(uint8_t* map) {

}

bool BLEHIDDevice::createCharacteristics() {
	m_manufacturerCharacteristic = m_deviceInfoService->createCharacteristic((uint16_t)0x2a29, BLECharacteristic::PROPERTY_READ);
	m_pnpCharacteristic = m_deviceInfoService->createCharacteristic((uint16_t)0x2a50, BLECharacteristic::PROPERTY_READ);

	m_hidInfoCharacteristic = m_hidService->createCharacteristic((uint16_t)0x2a4a, BLECharacteristic::PROPERTY_READ);
	m_reportMapCharacteristic = m_hidService->createCharacteristic((uint16_t)0x2a4b, BLECharacteristic::PROPERTY_READ);
	m_hidControlCharacteristic = m_hidService->createCharacteristic((uint16_t)0x2a4c, BLECharacteristic::PROPERTY_WRITE_NR);
	m_inputReportCharacteristic = m_hidService->createCharacteristic((uint16_t)0x2a4d, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

	return true;
}

void BLEHIDDevice::startServices() {
	m_deviceInfoService->start();
	m_hidService->start();
	if(m_batteryService!=nullptr)
		m_batteryService->start();
}

void BLEHIDDevice::deviceInfo(BLEServer* server) {
	m_deviceInfoService = server->createService(BLEUUID((uint16_t) 0x180a), 10);
}

void BLEHIDDevice::hidService(BLEServer* server) {
	m_hidService = server->createService(BLEUUID((uint16_t) 0x1812), 10);
}

void BLEHIDDevice::batteryService(BLEServer* server) {
	m_batteryService = server->createService(BLEUUID((uint16_t) 0x180f), 10);
}

BLECharacteristic* 	BLEHIDDevice::manufacturer() {
	return m_manufacturerCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::pnp() {
	return m_pnpCharacteristic;
}

BLECharacteristic*	BLEHIDDevice::hidInfo() {
	return m_hidInfoCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::reportMap() {
	return m_reportMapCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::hidControl() {
	return m_hidControlCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::inputReport(void*) {
	return m_inputReportCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::outputReport(void*) {
	return m_outputReportCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::featureReport(void*) {
	return m_featureReportCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::protocolMode() {
	return m_protocolModeCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::bootInput() {
	return m_bootInputCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::bootOutput() {
	return m_bootOutputCharacteristic;
}

BLECharacteristic* 	BLEHIDDevice::batteryLevel(void*) {
	return m_batteryLevelCharacteristic;
}

BLEDescriptor*		BLEHIDDevice::inputReport() {
	return m_inputReportDescriptor;
}

BLEDescriptor*		BLEHIDDevice::outputReport() {
	return m_outputReportDescriptor;
}

BLEDescriptor*		BLEHIDDevice::featureReport() {
	return m_featureReportDescriptor;
}

BLEDescriptor*		BLEHIDDevice::batteryLevel() {
	return m_batteryLevelDescriptor;
}
