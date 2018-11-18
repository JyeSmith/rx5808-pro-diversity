#include <Arduino.h>
#include <WString.h>

#include "settings_eeprom.h"

#include "state_home.h"

#include "receiver.h"
#include "channels.h"
#include "state.h"
#include "ui.h"
#include "temperature.h"
#include "voltage.h"
#include "touchpad.h"

using StateMachine::HomeStateHandler;

void HomeStateHandler::onEnter() {
    displayActiveChannel = Receiver::activeChannel;
//    Ui::clear();
}

void HomeStateHandler::onUpdate() {
//    Ui::needUpdate();
}

void HomeStateHandler::onInitialDraw() {
    this->onUpdateDraw();
}

void HomeStateHandler::onUpdateDraw() {

    if (TouchPad::touchData.buttonPrimary) {
      this->doTapAction();
    }
    
    if (isInBandScanRegion()) {
        bandScanUpdate();
        wasInBandScanRegion = true;
    } else {
        if (wasInBandScanRegion) {
            Receiver::setChannel(displayActiveChannel);
        }
        displayActiveChannel = Receiver::activeChannel;
        wasInBandScanRegion = false;
    }
    
//    Ui::clear();
//
    // Mode
//    Ui::display.setTextSize(1);
    Ui::display.setTextColor(100);
    Ui::display.setCursor( 8, 4);
//    Ui::display.print(PSTR2("Mode: ")); 
    Ui::display.print("Mode: ");    
    if (!EepromSettings.quadversity) {
//        Ui::display.print(PSTR2("Diversity"));
        Ui::display.print("Diversity");
    }
    if (EepromSettings.quadversity) {
//        Ui::display.print(PSTR2("Quadversity"));
        Ui::display.print("Quadversity");
    }    


//    #ifdef FENIX_QUADVERSITY
        // Voltage
//        Ui::display.setCursor( 280, 4);
        Ui::display.setCursor( 200, 4);
        Ui::display.print("7");   
//        Ui::display.print(Voltage::voltage);        
        Ui::display.print(".");        
        Ui::display.print("6");
//        Ui::display.print(Voltage::voltageDec);
        Ui::display.print("V");
        
        Ui::display.print(" / "); 
//    #endif

    #ifdef REALACC_RX5808_PRO_PLUS_OSD
        Ui::setCursor( 322, 4);
    #endif

    // Temperature
    Ui::display.print(Temperature::getTemperature());
    Ui::display.print("C"); 
    
    Ui::display.print(" / "); 

    // On Time
    uint8_t hours = millis() / 1000 / 60 / 60;
    uint8_t mins  = millis() / 1000 / 60 - hours * 60 * 60;
    uint8_t secs  = millis() / 1000 - hours * 60 * 60 - mins * 60;
    Ui::display.print(hours);
    Ui::display.print(":"); 
    if(mins < 10) {
        Ui::display.print("0");       
    }
    Ui::display.print(mins);
    Ui::display.print(":"); 
    if(secs < 10) {
        Ui::display.print("0");       
    }
    Ui::display.print(secs);

    // Horixontal line
//    Ui::drawFastHLine( 0, 14, 447, WHITE);
    Ui::display.line( 0, 14, SCREEN_WIDTH, 14, 100);
    
    // Display Band and Channel
    Ui::display.setCursor( 4, 16);
    Ui::display.printLarge(Channels::getName(displayActiveChannel), 12, 15);
           
    // Display Frequency
    Ui::display.setCursor( 4, 110);
    Ui::display.printLarge(Channels::getFrequency(displayActiveChannel), 5, 5);
    
//    Ui::display.print(Channels::getFrequency(displayActiveChannel));

//    Ui::drawBigNumber( 8, 100, 
//                       Channels::getFrequency(displayActiveChannel) / 1000, 
//                       5, 3);
//    Ui::drawBigNumber( 48, 100, 
//                       (Channels::getFrequency(displayActiveChannel) % 1000 ) / 100, 
//                       5, 3);
//    Ui::drawBigNumber( 88, 100, 
//                       (Channels::getFrequency(displayActiveChannel) % 100 ) / 10, 
//                       5, 3);
//    Ui::drawBigNumber( 128, 100, 
//                       Channels::getFrequency(displayActiveChannel) % 10, 
//                       5, 3);
//
//    // Channel labels
//    if (!EepromSettings.quadversity) {
    Ui::display.setCursor( 190, 23);
    Ui::display.print("A");
    Ui::display.setCursor( 190, 76);
    Ui::display.print("B");
//        Ui::drawBigCharacter( 190, 23, 
//                              'A', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28*2, 
//                              'B', 
//                              3, 2);
//      
//    }
//    if (EepromSettings.quadversity) {
//        Ui::drawBigCharacter( 190, 23, 
//                              'A', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28, 
//                              'B', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28*2, 
//                              'C', 
//                              3, 2);
//        Ui::drawBigCharacter( 190, 23 + 28*3, 
//                              'D', 
//                              3, 2);      
//    }
//    
//    // Channel selected square
//    if (!EepromSettings.quadversity) {
//      if (Receiver::activeReceiver == Receiver::ReceiverId::A) {
//          Ui::drawRoundRect(185, 20, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::B) {
//          Ui::drawRoundRect(185, 20 + 28*2, 34, 24, 2, WHITE);
//      }
//    }
//    if (EepromSettings.quadversity) {
//      if (Receiver::activeReceiver == Receiver::ReceiverId::A) {
//          Ui::drawRoundRect(185, 20, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::B) {
//          Ui::drawRoundRect(185, 20 + 28, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::C) {
//          Ui::drawRoundRect(185, 20 + 28*2, 34, 24, 2, WHITE);
//      }
//      if (Receiver::activeReceiver == Receiver::ReceiverId::D) {
//          Ui::drawRoundRect(185, 20 + 28*3, 34, 24, 2, WHITE);
//      }
//    }
//
//    // On percentage
//    if (!EepromSettings.quadversity) {
//        Ui::setCursor(225, 25);
//        Ui::display.print( (100 * Receiver::antennaAOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28*2);
//        Ui::display.print( (100 * Receiver::antennaBOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//    }
//    if (EepromSettings.quadversity) {
//        Ui::setCursor(225, 25);
//        Ui::display.print( (100 * Receiver::antennaAOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28);
//        Ui::display.print( (100 * Receiver::antennaBOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28*2);
//        Ui::display.print( (100 * Receiver::antennaCOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//        Ui::setCursor(225, 25+28*3);
//        Ui::display.print( (100 * Receiver::antennaDOnTime) / (millis() / 1000) );
//        Ui::display.print(PSTR2("%"));
//    }
//
//    // Draw RSSI Plots
//    if (!EepromSettings.quadversity) {
          Ui::display.rect(195, 20 + 28*0 + 3, 64*2, 28*2-1, 100);
          Ui::display.rect(195, 20 + 28*2 + 3, 64*2, 28*2-1, 100);
        for (uint8_t i=0; i < RECEIVER_LAST_DATA_SIZE-1; i++) {
          Ui::display.line(195+1*i, (20 + 28*2)-Receiver::rssiALast[i]/2, 195+1*(i+1), (20 + 28*2)-Receiver::rssiALast[i+1]/2, 100);
          Ui::display.line(195+1*i, (20 + 28*4)-Receiver::rssiBLast[i]/2, 195+1*(i+1), (20 + 28*4)-Receiver::rssiBLast[i+1]/2, 100);
        }
//    }
//    if (EepromSettings.quadversity) {
//          Ui::drawRect(250, 20 + 28*0 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*1 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*2 + 3, 64*3-1, 28*1-1, WHITE);
//          Ui::drawRect(250, 20 + 28*3 + 3, 64*3-1, 28*1-1, WHITE);
//        for (uint8_t i=0; i < RECEIVER_LAST_DATA_SIZE; i++) {
//          Ui::drawLine(250+3*i, (20 + 28*1)-Receiver::rssiALast[i]/4, 250+3*(i+1), (20 + 28*1)-Receiver::rssiALast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*2)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*2)-Receiver::rssiBLast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*3)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*3)-Receiver::rssiBLast[i+1]/4, WHITE);
//          Ui::drawLine(250+3*i, (20 + 28*4)-Receiver::rssiBLast[i]/4, 250+3*(i+1), (20 + 28*4)-Receiver::rssiBLast[i+1]/4, WHITE);
//        }
//    }
//

    // Plot Spectrum 324 x 224
    for (uint8_t i=0; i<CHANNELS_SIZE; i++) {
        Ui::display.fillRect(18+4*i, 214 - rssiData[i]*0.75, 4, rssiData[i]*0.75, rssiData[i]);
    }
    Ui::display.line(0, 213, 323, 213, 100);
    Ui::display.setCursor( 1, 215);
    Ui::display.print(Channels::getFrequency(Channels::getOrderedIndex(0)));
    Ui::display.setCursor( 299, 215);
    Ui::display.print(Channels::getFrequency(Channels::getOrderedIndex(CHANNELS_SIZE-1)));
//      
//    Ui::needDisplay();
}

void HomeStateHandler::doTapAction() {
            
  if (
      TouchPad::touchData.cursorX > 15  && TouchPad::touchData.cursorX < 85 &&
      TouchPad::touchData.cursorY > 20 && TouchPad::touchData.cursorY < 60
     ) {
          this->setChannel(-8);
        }
  else if (
      TouchPad::touchData.cursorX > 15  && TouchPad::touchData.cursorX < 85 &&
      TouchPad::touchData.cursorY > 60 && TouchPad::touchData.cursorY < 100
     ) {
          this->setChannel(8);
        }
  else if (
      TouchPad::touchData.cursorX > 85  && TouchPad::touchData.cursorX < 160 &&
      TouchPad::touchData.cursorY > 20 && TouchPad::touchData.cursorY < 60
     ) {
          this->setChannel(-1);
        }
  else if (
      TouchPad::touchData.cursorX > 85  && TouchPad::touchData.cursorX < 160 &&
      TouchPad::touchData.cursorY > 60 && TouchPad::touchData.cursorY < 100
     ) {
          this->setChannel(1);
        }
  else if ( // Select channel from spectrum
      HomeStateHandler::isInBandScanRegion()
     ) {
          Receiver::setChannel(
                              Channels::getOrderedIndex( (TouchPad::touchData.cursorX-40) / 5 )
                              );
          HomeStateHandler::centreFrequency();
          displayActiveChannel = Receiver::activeChannel;
          
          EepromSettings.startChannel = displayActiveChannel;
          EepromSettings.markDirty();
        }
}

void HomeStateHandler::setChannel(int channelIncrement) {

    int band = Receiver::activeChannel / 8;
    int channel = Receiver::activeChannel % 8;
    
    if (channelIncrement == 8) {
      band = band + 1;
    }
    
    if (channelIncrement == -8) {
      band = band - 1;
    }
    
    if (channelIncrement == 1 ) {
      channel = channel + 1;
      if (channel > 7) {
        channel = 0;
      }
    }
    
    if (channelIncrement == -1 ) {
      channel = channel - 1;
      if (channel < 0) {
        channel = 7;
      }
    }
    
    int newChannelIndex = band * 8 + channel;

    if (newChannelIndex >= CHANNELS_SIZE) {
      newChannelIndex = newChannelIndex - CHANNELS_SIZE;
    }
    if (newChannelIndex < 0) {
      newChannelIndex = newChannelIndex + CHANNELS_SIZE;
    }
    Receiver::setChannel(newChannelIndex);
    EepromSettings.startChannel = newChannelIndex;
    EepromSettings.markDirty();
    centred = false;
}

// Frequency 'Centring' function.
// The function walks up and then down from the currently Rx frequency 
// in 1 MHz steps until RSSI < threshold.  The Rx is then set to the 
// centre of these 2 frequencies.
void HomeStateHandler::centreFrequency() {

  uint16_t activeChannelFreq = Channels::getFrequency(Receiver::activeChannel);
  uint16_t centerFreq = Channels::getCenterFreq(activeChannelFreq);
  Receiver::setChannelByFreq(centerFreq);
  
  wasInBandScanRegion = false;
}

bool HomeStateHandler::isInBandScanRegion() {
    if (TouchPad::touchData.cursorY > 130 ) {
        return true;
    } else {
        return false;
    }
}

void HomeStateHandler::bandScanUpdate() {

    Ui::UiTimeOut.reset();
    
    if (!wasInBandScanRegion) {
        orderedChanelIndex = Channels::getOrderedIndexFromIndex(displayActiveChannel); // Start from currently selected channel to prevent initial spike artifact.
    }
    
    if (Receiver::isRssiStable() && Receiver::hasRssiUpdated) {
    
        if (!EepromSettings.quadversity) {
            rssiData[orderedChanelIndex] = max(Receiver::rssiA, Receiver::rssiB);
        }
        if (EepromSettings.quadversity) {
            rssiData[orderedChanelIndex] = max(Receiver::rssiA, max(Receiver::rssiB, max(Receiver::rssiC, Receiver::rssiD)));
        }
    
        orderedChanelIndex = orderedChanelIndex + 1;
        if (orderedChanelIndex == CHANNELS_SIZE) {
            orderedChanelIndex = 0;
        }
        Receiver::setChannel(Channels::getOrderedIndex(orderedChanelIndex));
    
    }
    
}

