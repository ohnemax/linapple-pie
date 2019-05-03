#include "stdafx.h"

HVCard::HVCard() {
  voltage = 0;
  enable = false;
  realvoltage = 0;
}

HVCard::~HVCard() {
  
}

BYTE HVCard::TurnOn() {
  printf("HV Card: Enable High Voltage\n");
  printf("HV Card: Current voltage is %f V\n", realvoltage);
  enable = true;
}

BYTE HVCard::TurnOff() {
  printf("HV Card: Disable High Voltage\n");
  enable = false;
}

BYTE HVCard::SetVoltage(BYTE b) {
  voltage = b;
  realvoltage = b / 233.0 * 1000.0;
  if(enable) {
    printf("HV Card: Set Voltage to %f V\n", realvoltage);
  }
  else {
    printf("HV Card: Set possible Voltage to %f V (DISABLED!)\n", realvoltage);
  }
}


BYTE HVCard::Read(WORD PC, WORD uAddr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  UINT uSlot = ((uAddr & 0xff) >> 4) - 8;
  HVCard* ic = (HVCard*) MemGetSlotParameters(uSlot);

  printf("HV Card: Reading\n");
  switch (uAddr & 0xf) {
  case 0x0:
    printf("Only write to this address!\n");
    break;
  case 0x1: return ic->TurnOn();
  case 0x2: return ic->TurnOff();
  case 0x4: return ic->TurnOn();
  case 0x5: return ic->TurnOn();
  }
  return 0;

}
BYTE HVCard::Write(WORD PC, WORD uAddr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  UINT uSlot = ((uAddr & 0xff) >> 4) - 8;
  HVCard* ic = (HVCard*) MemGetSlotParameters(uSlot);
  printf("HV Card: Writing\n");
  switch (uAddr & 0xf) {
  case 0x0:
    return ic->SetVoltage(uValue);
    break;
  }
  return 0;
  
}

void HVCard::Initialize(LPBYTE pCxRomPeripheral, UINT uSlot) {
  RegisterIoHandler(uSlot, &HVCard::Read, &HVCard::Write, NULL, NULL, this, NULL);
}

void HVCard::Destroy() {
  
}

