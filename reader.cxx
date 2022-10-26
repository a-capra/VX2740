/******************************************************************
 * Unpack CAEN VX2740 data *
 * 
 * A. Capra
 * October 2022
 *
 ******************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "midasio.h"
#include "TVX2740RawData.hxx"

bool Verbose = true;
int TotNumBoards = 1;
int numberChannelPerModule = 64;
unsigned gCounter = 0;
unsigned gError = 0;

void PrintWaveform(const std::vector<double>* wf)
{
   for(auto& s: *wf)
      std::cout<<s<<std::endl;
}

void Unpack(TMEvent* midasevent)
{
   if( midasevent->error ) // skip events with errors
      {
         ++gError;
         return;
      }
   int fe_idx = midasevent->trigger_mask;
   for( int board_idx = 0; board_idx<TotNumBoards; ++board_idx )
      {
         std::stringstream ss;
         ss<<"D"<<std::setfill('0')<<std::setw(3)<<std::to_string(board_idx);
         std::string bankname=ss.str();
         TMBank* bank = midasevent->FindBank(bankname.c_str());
         if( !bank ) continue;
         //int bid = std::stoi(bank->name+1);
         if( Verbose )
            std::cout<<"UnpackVX2740Module::Analyze VX2740 bank "<<bankname<<" found... Board: "<<board_idx<<std::endl;
         TVX2740RawData* raw = new TVX2740RawData((int)bank->data_size,
                                                  (int)bank->type,
                                                  bank->name.c_str(),
                                                  midasevent->GetBankData(bank));
         if( raw->GetHeader().format != 0x10 )
            {
               if( Verbose )
                  std::cout<<"Unpack Not scope-data format"<<std::endl;
               continue;
            }

         if( Verbose )
            {
               raw->Print();
               std::cout<<"Unpack VX2740 ETTT: "<<raw->GetTriggerTimeStamp()
                        <<" s\tEvent Counter: "<<raw->GetHeader().event_counter<<std::endl;
            }

         // loop over channels
         for( int chan = 0; chan < raw->GetNChannels(); ++chan )
            {
               TVX2740RawChannel* channelData = raw->GetChannelData(chan);
               if( channelData->IsEmpty() ) continue;
         
               channelData->SetBoardNumber(board_idx);
               channelData->SetIndex(board_idx*numberChannelPerModule
                                     +channelData->GetChannelNumber());
               if( Verbose ) channelData->Print();

               PrintWaveform( channelData->GetMeasurement() );
	  
            } // loop over channels
         delete raw;
      }// loop over boards
   ++gCounter;
}

void ProcessMidasFiles(std::vector<std::string> in)
{
   for(auto& f: in)
      {
         TMReaderInterface *reader = TMNewReader(f.c_str());
         while (1) 
            {
               TMEvent* event = TMReadEvent(reader);
	  
               if (!event) // EOF
                  break;
	  
               if (event->error) 
                  {
                     delete event;
                     break;
                  }
	  
               if (event->event_id == 0x8000) // begin of run event
                  {
                     int runno = event->serial_number;
                     std::cout<<"UnpackVXModule::BeginRun() run: "<<runno<<" midasfile: "<<f<<std::endl;
                  }
               else if (event->event_id == 0x8001) // end of run event
                  {
                     int runno = event->serial_number;
                     std::cout<<"UnpackVXModule::EndRun() run: "<<runno<<" events: "<<gCounter<<" errors: "<<gError<<std::endl;
                  }
               else if (event->event_id == 0x8002) // message event
                  {
            
                  }
               else
                  {
                     // analyze
                     Unpack(event);
                  }

               delete event;
            }//events loop
         reader->Close();
         delete reader;
      }//files loop
}


int main(int argc, char** argv)
{
   std::vector<std::string> files;
   for(int i=1; i<argc; ++i) files.push_back(argv[i]);

   ProcessMidasFiles(files);

   return 0;
}

/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
