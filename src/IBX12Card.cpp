#include "stdafx.h"

IBX12Card::IBX12Card(double maxvoltage) {
  statusbyte = 0;
  adctrig = false;
  statusrequests = 0;

  double absolutemaxvoltage = 10;
  float filespectrum[4096];

  FILE *f;
  f = fopen("4096-spectrum.dat", "r");
  for(int i = 0; i<4096; i++) {
    fscanf(f,"%f",&filespectrum[i]);
  }
  double sum = 0;
  for(int i = 0; i<4096; i++) {
    spectrum[i] = filespectrum[i];
    sum += filespectrum[i];
  }
  for(int i = 0; i<4096; i++) {
    spectrum[i] = spectrum[i] / sum;
    if(i > 0) {
      spectrum[i] += spectrum[i-1];
    }
    // printf("%f ", spectrum[i]);
  }
  // printf("\n");
  fclose(f);
  nextvalue = 0;
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
  nextvalue = i;
  // printf("%d\n", i);
  // nextvalue += 1;
  // if (nextvalue == 4096) {
  //   nextvalue = 0;
  // }
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
  statusrequests += 1;
  statusbyte = 0;
  // After 1 requests there will be new event
  // if (statusrequests > 1) {
  if (statusrequests > 1) {
    statusbyte = 1;
    adctrig = true;
  // After more status requests STS will be on
  if (statusrequests > 3) {
    statusbyte = 3;
  }
  }
  // After even more status requests, conversion will be done
  if (statusrequests > 4) {
    statusbyte = 1;
    statusrequests = 0;
  }
  printf("Status Read %d, Value %d\n", statusrequests, statusbyte);
  return statusbyte;
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

