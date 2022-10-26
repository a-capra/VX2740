#ifndef TVX2740RawData_hxx_seen
#define TVX2740RawData_hxx_seen

#include <vector>
#include <map>

#include "TGenericData.hxx"
#include "TRawChannel.hxx"

struct TVX2740EventHeader {
   uint8_t format;
   uint32_t event_counter;
   uint32_t size_64bit_words; // includes size of 24 byte header
   uint16_t flags;
   uint8_t overlap;
   uint64_t trigger_time;
   uint64_t ch_enable_mask;

   TVX2740EventHeader(const uint64_t* buffer=NULL);

   uint32_t size_bytes();
   uint32_t samples_per_chan();
};


/// Class to store information from a single VX2740 channel.
class TVX2740RawChannel: public TRawChannel {

public:
  /// constructor
  TVX2740RawChannel(int channel=-1, int num_samples=0):
    TRawChannel(channel,num_samples)
  { }


};

/// Class to store data from CAEN VX2740, 125MHz FADC.
///
/// This class encapsulates the data from a single board (in a single MIDAS bank).
/// This decoder is for the default version of the firmware.
class TVX2740RawData: public TGenericData
{
public:
  /// Constructor
  TVX2740RawData(int bklen, int bktype, const char *name, void *pdata);
  /// Destructor
  ~TVX2740RawData()
   {
      fMeasurements.clear();
   }

  /// Get the number of 32-bit words in bank.
  inline TVX2740EventHeader GetHeader() const {
    return fHeader;
  }

  /// Get Number of channels in this bank.
  inline int GetNChannels() const {
    return fMeasurements.size();
  }

  /// Get Active Channels from header
   inline bool IsActive(int ch) {
      return (fHeader.ch_enable_mask & ((uint64_t)1 << ch));
   }

  /// Get Trigger Time stamp
  /// No documentation exists, but tests showed that this timestamp has
  /// 8 ns resolution thus a range of 625 h, i.e., 8 ns*(2^48-1).
  inline double GetTriggerTimeStamp() const { return double(GetHeader().trigger_time)*8.e-9;}

  /// Get Channel Data
  inline TVX2740RawChannel* GetChannelData(int i) {
     return &fMeasurements.at(i);
  }

  void Print();

private:
  TVX2740EventHeader fHeader;
  /// Vector of VX2740 measurements
  std::vector<TVX2740RawChannel> fMeasurements;

};

#endif

/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
