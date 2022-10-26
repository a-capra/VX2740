#ifndef __RAWCHANNEL__
#define __RAWCHANNEL__

#include <vector>
#include <iostream>
#include <iomanip>
#include <typeinfo>

class TRawChannel
{
protected:
   // Board number
   int fBoardNumber;
   // Channel number
   int fChannelNumber;
   // index number
   int fIndex;
  
   std::vector<double> fWaveform;

public:
   TRawChannel(int board,
               int channel, int num_samples):fBoardNumber(board),
                                             fChannelNumber(channel)
   {
      fWaveform.resize(num_samples);
   }
  
   TRawChannel(int channel, int num_samples):fBoardNumber(-1),
                                             fChannelNumber(channel)
   {
      fWaveform.resize(num_samples);
   }

   // Copy constructor
   TRawChannel(const TRawChannel &right):fBoardNumber(right.fBoardNumber),
                                         fChannelNumber(right.fChannelNumber),
                                         fIndex(right.fIndex),
                                         fWaveform(right.fWaveform)
   {   }

   void operator=(const TRawChannel &right)
   {
      fBoardNumber=right.fBoardNumber;
      fChannelNumber=right.fChannelNumber;
      fIndex=right.fIndex;
      fWaveform=right.fWaveform;
   }

   virtual ~TRawChannel() { fWaveform.clear(); }

   

   inline void SetBoardNumber(int b)   { fBoardNumber = b;  }
   inline int GetBoardNumber() const   { return fBoardNumber;  }
   inline int GetChannelNumber() const { return fChannelNumber;  }
   inline void SetIndex(int i)         { fIndex=i; }
   inline int GetIndex() const         { return fIndex; }

   /// Get the ADC sample for a particular bin (for uncompressed data).
   inline int GetNSamples() const { return (int) fWaveform.size(); }

   /// Get the ADC sample for a particular bin (for uncompressed data).
   double GetADCSample(int i) const
   {
      if (i >= 0 && i < (int) fWaveform.size())
         return fWaveform[i];

      std::cerr<<typeid(this).name()<<"::GetADCSample "<<i<<" is out of range "<<GetNSamples()<<std::endl;
      // otherwise, return error value.
      return -1.;
   }

   /// Returns true for objects with no ADC samples or ZLE pulses.
   inline int IsEmpty() const { return fWaveform.empty();  }
 
   /// Set ADC sample
   inline void SetADCSample(int i, double sample) { fWaveform[i] = sample; }
   inline void AddADCSample(double sample) { fWaveform.push_back(sample); }

   // Get Waveform
   inline const double* GetWaveform() const { return fWaveform.data(); }
   inline const std::vector<double>* GetMeasurement() const { return &fWaveform; }

   void Print() const
   {
      std::cout<<"TRawChannel board: "<<std::setw(3)<<GetBoardNumber()
               <<" ch: "<<std::setw(3)<<GetChannelNumber()
               <<" idx: "<<std::setw(3)<<GetIndex()
               <<" # of samples: "<<std::setw(6)<<GetNSamples()<<std::endl;
   }
  
};

#endif


/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
