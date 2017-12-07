#pragma once

extern class IBXCard sg_IBXCard;

class IBXCard
{
public:
  IBXCard();
  virtual ~IBXCard();

  void Initialize(LPBYTE pCxRomPeripheral, UINT uSlot);
  void Destroy();

  BYTE ResetADC();
  BYTE ReadADC();
  BYTE ReadStatus();
  
  static BYTE Read(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);
  static BYTE Write(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);

private:
  BYTE statusbyte;
  
  BOOL adctrig;
  UINT statusrequests;
  float spectrum[256];
};
