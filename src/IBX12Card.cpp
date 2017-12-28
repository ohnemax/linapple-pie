#include "stdafx.h"

IBX12Card::IBX12Card() {
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
    // printf("%f ", spectrum[i]);
  }
  // printf("\n");
  fclose(f);
}

IBX12Card::~IBX12Card() {
  
}

BYTE IBX12Card::ResetADC() {
  printf("Reset ADC\n");
  statusrequests = 0;
  statusbyte = 0;
  adctrig = false;
  float no = 0;
  no = 1.0 * rand() / RAND_MAX;
  int i = 0;
  while(spectrum[i] <= no) {
    i++;
  }
  int offset = int(1.0 * rand() / RAND_MAX * 16);
  printf("i:%i o:%i\n", i, offset);
  nextvalue = i * 8 + offset; // it is 12 bit, but the card only uses 11
  return 0;
}

BYTE IBX12Card::ReadADCLowByte() {
  printf("Reading ADC low byte\n");
  if (adctrig) {
    // Lowest 4 bits of 12 Bit number, but padded with zeroes at lower nibble
    printf("Value: %d, Low: %d\n", nextvalue, (nextvalue & 0x000f) * 16);
    return (nextvalue & 0x000f) * 16;
  }
  return 0;
}

BYTE IBX12Card::ReadADCHighByte() {
  printf("Reading ADC high byte\n");
  if (adctrig) {
    printf("Value: %d, High: %d\n", nextvalue, (nextvalue >> 4));
    return (nextvalue >> 4);
  }
  return 0;
}

BYTE IBX12Card::ReadStatus() {
  printf("Reading Status Value\n");
  statusrequests += 1;
  // After 10 statusrequsts interrupt will be low
  // if (statusrequests > 10) {
  if (statusrequests > 2) {
    statusbyte = 1;
    adctrig = true;
    statusrequests = 0;
    return statusbyte;
  }
  // After 2 requests there will be new event
  // if (statusrequests > 2) {
  if (statusrequests > 1) {
    statusbyte = 2;
    adctrig = true;
    return statusbyte;
  }
  return 0;
}

BYTE IBX12Card::Read(WORD PC, WORD uAddr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  UINT uSlot = ((uAddr & 0xff) >> 4) - 8;
  IBX12Card* ic = (IBX12Card*) MemGetSlotParameters(uSlot);

  printf("Tried reading from card (%d)\n", uAddr & 0xf);
  switch (uAddr & 0xf) {
  case 0x0:
    printf("Reading LED value (not implemented)\n");
    break;
  case 0x1: return ic->ResetADC();
  case 0x2: return ic->ReadADCLowByte();
  case 0x3: return ic->ReadADCHighByte();
  case 0x4: return ic->ReadStatus();
  }
  return 0;

}
BYTE IBX12Card::Write(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft) {
  
}

void IBX12Card::Initialize(LPBYTE pCxRomPeripheral, UINT uSlot) {
  RegisterIoHandler(uSlot, &IBX12Card::Read, &IBX12Card::Write, NULL, NULL, this, NULL);
}

void IBX12Card::Destroy() {
  
}

