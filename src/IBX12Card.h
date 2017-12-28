#pragma once

extern class IBX12Card sg_IBX12Card;

class IBX12Card
{
public:
  IBX12Card();
  virtual ~IBX12Card();

  void Initialize(LPBYTE pCxRomPeripheral, UINT uSlot);
  void Destroy();

  BYTE ResetADC();
  BYTE ReadADCLowByte();
  BYTE ReadADCHighByte();
  BYTE ReadStatus();
  
  static BYTE Read(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);
  static BYTE Write(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);

private:
  BYTE statusbyte;
  
  BOOL adctrig;
  UINT statusrequests;
  float spectrum[256];
  WORD nextvalue;
};
