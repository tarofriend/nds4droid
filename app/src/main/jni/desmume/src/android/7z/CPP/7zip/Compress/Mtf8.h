// Mtf8.h

#ifndef __COMPRESS_MTF8_H
#define __COMPRESS_MTF8_H

#include "../../Common/Types.h"

namespace NCompress {

struct CMtf8Encoder
{
  Byte Buf[256];

  int FindAndMove(Byte v)
  {
    int pos;
    for (pos = 0; Buf[pos] != v; pos++);
    int resPos = pos;
    for (; pos >= 8; pos -= 8)
    {
      Buf[pos] = Buf[pos - 1];
      Buf[pos - 1] = Buf[pos - 2];
      Buf[pos - 2] = Buf[pos - 3];
      Buf[pos - 3] = Buf[pos - 4];
      Buf[pos - 4] = Buf[pos - 5];
      Buf[pos - 5] = Buf[pos - 6];
      Buf[pos - 6] = Buf[pos - 7];
      Buf[pos - 7] = Buf[pos - 8];
    }
    for (; pos > 0; pos--)
      Buf[pos] = Buf[pos - 1];
    Buf[0] = v;
    return resPos;
  }
};

/*
struct CMtf8Decoder
{
  Byte Buf[256];

  void Init(int) {};
  Byte GetHead() const { return Buf[0]; }
  Byte GetAndMove(int pos)
  {
    Byte res = Buf[pos];
    for (; pos >= 8; pos -= 8)
    {
      Buf[pos] = Buf[pos - 1];
      Buf[pos - 1] = Buf[pos - 2];
      Buf[pos - 2] = Buf[pos - 3];
      Buf[pos - 3] = Buf[pos - 4];
      Buf[pos - 4] = Buf[pos - 5];
      Buf[pos - 5] = Buf[pos - 6];
      Buf[pos - 6] = Buf[pos - 7];
      Buf[pos - 7] = Buf[pos - 8];
    }
    for (; pos > 0; pos--)
      Buf[pos] = Buf[pos - 1];
    Buf[0] = res;
    return res;
  }
};
*/

#ifdef _WIN64
#define MODE_64BIT
#endif

#ifdef MODE_64BIT
typedef UInt64 CMtfVar;
#define MTF_MOVS 3
#else
typedef UInt32 CMtfVar;
#define MTF_MOVS 2
#endif

#define MTF_MASK ((1 << MTF_MOVS) - 1)


struct CMtf8Decoder
{
  CMtfVar Buf[256 >> MTF_MOVS];

  void StartInit() { memset(Buf, 0, sizeof(Buf)); }
  void Add(unsigned int pos, Byte val) { Buf[pos >> MTF_MOVS] |= ((CMtfVar)val << ((pos & MTF_MASK) << 3));  }
  Byte GetHead() const { return (Byte)Buf[0]; }
  Byte GetAndMove(unsigned int pos)
  {
    UInt32 lim = ((UInt32)pos >> MTF_MOVS);
    pos = (pos & MTF_MASK) << 3;
    CMtfVar prev = (Buf[lim] >> pos) & 0xFF;

    UInt32 i = 0;
    if ((lim & 1) != 0)
    {
      CMtfVar next = Buf[0];
      Buf[0] = (next << 8) | prev;
      prev = (next >> (MTF_MASK << 3));
      i = 1;
      lim -= 1;
    }
    for (; i < lim; i += 2)
    {
      CMtfVar next = Buf[i];
      Buf[i] = (next << 8) | prev;
      prev = (next >> (MTF_MASK << 3));
      next = Buf[i + 1];
      Buf[i + 1] = (next << 8) | prev;
      prev = (next >> (MTF_MASK << 3));
    }
    CMtfVar next = Buf[i];
    CMtfVar mask = (((CMtfVar)0x100 << pos) - 1);
    Buf[i] = (next & ~mask) | (((next << 8) | prev) & mask);
    return (Byte)Buf[0];
  }
};

/*
const int kSmallSize = 64;
class CMtf8Decoder
{
  Byte SmallBuffer[kSmallSize];
  int SmallSize;
  Byte Counts[16];
  int Size;
public:
  Byte Buf[256];

  Byte GetHead() const
  {
    if (SmallSize > 0)
      return SmallBuffer[kSmallSize - SmallSize];
    return Buf[0];
  }

  void Init(int size)
  {
    Size = size;
    SmallSize = 0;
    for (int i = 0; i < 16; i++)
    {
      Counts[i] = ((size >= 16) ? 16 : size);
      size -= Counts[i];
    }
  }

  Byte GetAndMove(int pos)
  {
    if (pos < SmallSize)
    {
      Byte *p = SmallBuffer + kSmallSize - SmallSize;
      Byte res = p[pos];
      for (; pos > 0; pos--)
        p[pos] = p[pos - 1];
      SmallBuffer[kSmallSize - SmallSize] = res;
      return res;
    }
    if (SmallSize == kSmallSize)
    {
      int i = Size - 1;
      int g = 16;
      do
      {
        g--;
        int offset = (g << 4);
        for (int t = Counts[g] - 1; t >= 0; t--, i--)
          Buf[i] = Buf[offset + t];
      }
      while(g != 0);
      
      for (i = kSmallSize - 1; i >= 0; i--)
        Buf[i] = SmallBuffer[i];
      Init(Size);
    }
    pos -= SmallSize;
    int g;
    for (g = 0; pos >= Counts[g]; g++)
      pos -= Counts[g];
    int offset = (g << 4);
    Byte res = Buf[offset + pos];
    for (pos; pos < 16 - 1; pos++)
      Buf[offset + pos] = Buf[offset + pos + 1];
    
    SmallSize++;
    SmallBuffer[kSmallSize - SmallSize] = res;

    Counts[g]--;
    return res;
  }
};
*/

}

#endif
