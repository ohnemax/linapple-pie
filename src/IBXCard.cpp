#include "stdafx.h"

IBXCard::IBXCard() {
  statusbyte = 0;
  adctrig = false;
  statusrequests = 0;

  FILE *f;
  f = fopen("spectrum.dat", "r");
  for(int i = 0; i<256; i++) {
    fscanf(f,"%f",&spectrum[i]);
    if(i > 0) {
      spectrum[i] += spectrum[i-1];
    }
    //    printf("%f ", spectrum[i]);
  }
  //  printf("\n");
  fclose(f);
}

IBXCard::~IBXCard() {
  
}

BYTE IBXCard::ResetADC() {
  printf("Reset ADC\n");
  statusrequests = 0;
  statusbyte = 0;
  adctrig = false;
  return 0;
}

BYTE IBXCard::ReadADC() {
  printf("Reading ADC value\n");
  float no = 0;
  if (adctrig) {
    no = 1.0 * rand() / RAND_MAX;
    int i = 0;
    while(spectrum[i] <= no) {
      i++;
    }
    return i;
  }
  return 0;
}

BYTE IBXCard::ReadStatus() {
  printf("Reading Status Value\n");
  statusrequests += 1;
  // After 10 statusrequsts interrupt will be low
  if (statusrequests > 10) {
    statusbyte = 1;
    adctrig = true;
    statusrequests = 0;
    return statusbyte;
  }
  // After 2 requests there will be new event
  if (statusrequests > 2) {
    statusbyte = 3;
    adctrig = true;
    statusrequests = 0;
    return statusbyte;
  }
  return 0;
}

BYTE IBXCard::Read(WORD PC, WORD uAddr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  UINT uSlot = ((uAddr & 0xff) >> 4) - 8;
  IBXCard* ic = (IBXCard*) MemGetSlotParameters(uSlot);

  printf("Tried reading from card\n");
  switch (uAddr & 0xf) {
  case 0x0:
    printf("Reading LED value (not implemented)\n");
    break;
  case 0x1: return ic->ResetADC();
  case 0x2: return ic->ReadADC();
  case 0x3: return ic->ReadStatus();
  }
  return 0;

}
BYTE IBXCard::Write(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  
}

void IBXCard::Initialize(LPBYTE pCxRomPeripheral, UINT uSlot) {
  RegisterIoHandler(uSlot, &IBXCard::Read, &IBXCard::Write, NULL, NULL, this, NULL);
}

void IBXCard::Destroy() {
  
}

