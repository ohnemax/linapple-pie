#pragma once

extern class HVCard sg_HVCard;

class HVCard
{
public:
  HVCard();
  virtual ~HVCard();

  void Initialize(LPBYTE pCxRomPeripheral, UINT uSlot);
  void Destroy();

  BYTE TurnOn();
  BYTE TurnOff();
  BYTE SetVoltage(BYTE b);
  
  static BYTE Read(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);
  static BYTE Write(WORD PC, WORD uADDr, BYTE bWrite, BYTE uValue, ULONG nCyclesLeft);

private:
  BYTE voltage;
  bool enable;
  float realvoltage;
};
