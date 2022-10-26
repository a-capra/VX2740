#include "TVX2740RawData.hxx"

#include <iomanip>
#include <iostream>

TVX2740EventHeader::TVX2740EventHeader(const uint64_t *data) {
  if (data) {
    format = data[0] >> 56;
    event_counter = (data[0] >> 32) & 0xFFFFFF;
    size_64bit_words = data[0] & 0xFFFFFFFF;
    flags = data[1] >> 52;
    overlap = (data[1] >> 48) & 0xF;
    trigger_time = data[1] & 0xFFFFFFFFFFFF;
    ch_enable_mask = data[2];
  }
}

uint32_t TVX2740EventHeader::size_bytes() {
  return size_64bit_words * 8;
}

uint32_t TVX2740EventHeader::samples_per_chan() {
  int num_chans = 0;

  for (int i = 0; i < 64; i++) {
    if (ch_enable_mask & ((uint64_t)1 << i)) {
      num_chans++;
    }
  }

  // 3 header words, 4 samples per word
  int total_num_samples = (size_64bit_words - 3) * 4;

  if (num_chans) {
    return total_num_samples / num_chans;
  } else {
    return 0;
  }
}

TVX2740RawData::TVX2740RawData(int bklen, int bktype,
                               const char *name, void *pdata) :
   TGenericData(bklen, bktype, name, pdata)
{
  fHeader = TVX2740EventHeader(GetData64());

  if (fHeader.format != 0x10) {
    // "Real" data events have format 0x10.
    // Don't try to parse other special events the board sends.
    return;
  }

  uint32_t num_samples = fHeader.samples_per_chan();

  for( int i = 0; i<64; ++i )
     {
        if( IsActive(i) )
             fMeasurements.emplace_back( i, num_samples);
     }

  // Data format is: header; 4 samples from channel 1; 4 samples from channel 2; ...
  const uint64_t* p = GetData64() + 3;
  for (uint32_t samp = 0; samp < num_samples; samp += 4)
     {
        for (auto it = fMeasurements.begin(); it != fMeasurements.end(); it++)
           {
              uint64_t samp_dcba = *p++;
              it->SetADCSample(samp + 0, double(samp_dcba & 0xFFFF));
              it->SetADCSample(samp + 1, double((samp_dcba >> 16) & 0xFFFF));
              it->SetADCSample(samp + 2, double((samp_dcba >> 32) & 0xFFFF));
              it->SetADCSample(samp + 3, double((samp_dcba >> 48) & 0xFFFF));
           }
     }
}

void TVX2740RawData::Print()
{
  std::cout << "VX2740 decoder for bank " << GetName().c_str() << std::endl;
  std::cout << std::setw(16) << "Num bytes: " << fHeader.size_bytes() << std::endl;

  std::cout << std::hex;
  std::cout << std::setw(16) << "Data Format: " << "0x" << (uint64_t) fHeader.format<< std::endl;
  std::cout << std::setw(16) << "Channel Mask: " << "0x" << fHeader.ch_enable_mask << std::endl;

  std::cout << std::dec;
  std::cout << std::setw(16) << "Event counter: " << fHeader.event_counter << std::endl;
  std::cout << std::setw(16) << "Trigger time: " << fHeader.trigger_time << std::endl;

  std::cout << std::setw(16) << "Num samples: " << fHeader.samples_per_chan() << std::endl;
  std::cout << std::setw(16) << "Num channels: " << GetNChannels() << std::endl;
}


/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
