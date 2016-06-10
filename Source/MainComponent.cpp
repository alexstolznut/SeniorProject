/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

//getOutputChannelNames
//getInputChannelNames

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#define NUMBEROFTRACKS 3
#include "../JuceLibraryCode/JuceHeader.h"
#include "Display.h"
#include "AudioVisual.h"
#include "Measure.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class recorda   :   public AudioIODeviceCallback
{

public:
    recorda (AudioThumbnail& thumbnailToUpdate)
        :   thumbnail(thumbnailToUpdate),
            backgroundThread ("Audio Recorder Thread"),
            sampleRate(0), nextSampleNum (0)
    
    {
        backgroundThread.startThread();
        for (int i =0; i<3; ++i)
        {
            resetBuffer(i);
        }
    }
    
    ~recorda()
    {
        stop();
    }
    
    void resetBuffer(int trackNum)
    {
        track[trackNum].setSize(2, 0, false, true, false);
    }
    void startAD()
    {
        deviceManager.addAudioCallback(this);
        deviceManager.initialise(2, 2, 0, true, "Headphones", nullptr);
    }
    void startRec()
    {
        
        startAD();
        resetBuffer(trackNumber);
        playing=false;
        recording = true;
    }
    
    
    void closeAD()
    {
        deviceManager.closeAudioDevice();
        audioDeviceStopped();
        playing = false;
        recording=false;
    }
    
    void stop()
    {
        recording = false;
        closeAD();
       
    }
 

    
    bool playing;
    bool recording;
    bool masterplaying;
  
    
    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        
        sampleRate = device -> getCurrentSampleRate();
    }
    
    void audioDeviceStopped() override
    {
        sampleRate = 0;
    }
    

    //AudioIODevice, AudioDeviceManager, getCurrentSampleRate
        
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override
   {
       
       
       
       int oldSampleSize = track[trackNumber].getNumSamples();
       int channelNumber = track[trackNumber].getNumChannels();
       duration = oldSampleSize;

       if (recording == true && duration<=44100*5)
       {
           track[trackNumber].setSize( track[trackNumber].getNumChannels(),
                             oldSampleSize + numSamples,
                             true,
                             true,
                             false);
         
        
          
           
           
                    // MasterBuff.addFrom(track[trackNumber].getNumChannels(), 44100,track[trackNumber],channelNumber,0,numSamples);
           int bufferSamplesRemaining = track[trackNumber].getNumSamples()-position;
           int samplesThisTime = jmin(numSamples, bufferSamplesRemaining);
           position+=samplesThisTime;
           for(int j = 0; j < channelNumber; ++j)
           {
               track[trackNumber].copyFrom(j, oldSampleSize, inputChannelData[j], numSamples);
           }
           for(int i=0; i<numOutputChannels; ++i)
           {
               if(outputChannelData[i] != nullptr)
               {
                   FloatVectorOperations::clear(outputChannelData[i], numSamples);
               }
           }
           if(trackNumber==0)
           {
               thumbnail.reset(channelNumber, oldSampleSize);
               thumbnail.addBlock(nextSampleNum,track[0], 0, oldSampleSize);
           }
           else if(trackNumber==1)
           {
               thumbnail.reset(channelNumber, oldSampleSize);
               thumbnail.addBlock(nextSampleNum,track[1], 0, oldSampleSize);
               std::cerr<<"1";
           }
           else if(trackNumber==2)
           {
               thumbnail.reset(channelNumber, oldSampleSize);
               thumbnail.addBlock(nextSampleNum,track[2], 0, oldSampleSize);
           }
           else if(trackNumber==3)
           {
               thumbnail.reset(channelNumber, oldSampleSize);
               thumbnail.addBlock(nextSampleNum,track[3], 0, oldSampleSize);
           }
           
          
       }
         
       
       if(playing == true)
       {
           
           
           
//           for(int track=0;track<3;++track)
//           {
//               for(int bufferSample=0; bufferSample<numSamples;++bufferSample)
//               {
//                   int sampleToPlay = bufferSample+position+AudioSampleBuffer(track[trackNumber], 2, 44100,numSamples);
//                   if(track[trackNumber].isNotBypassed && numSamples < track[trackNumber].getNumSamples)
//                   {
//                       if(outputChannelData[i] != nullptr)
//                       {
//                           
//                        outputBuffer[position] += myTracks[track].get(sampleToPlay)
//                       }
//                   }
//               }
//               
//           }
           for(int samples=0; samples<numSamples; ++samples)
           {
               int bufferSamplesRemaining = track[trackNumber].getNumSamples()-position;
               int samplesThisTime = jmin(numSamples, bufferSamplesRemaining);
             
               if(++position > track[trackNumber].getNumSamples()-numSamples)
               {
                   position = 0;
               }
               
               for (int i = 0; i < numOutputChannels; ++i)
               {
                   if (outputChannelData[i] != nullptr)
                   {
                       
                       float sample = track[trackNumber].getSample(i, samples+position);
                       
                       outputChannelData[i][samples] = sample;
                       
                   }
               }
           }
       }
           
           
           if(masterplaying==true)
           {
//               std::cerr<<samples<<std::endl;
               for(int somesamples=0; somesamples<numSamples; ++somesamples)
               {
                   
                   if(++masterPosition1 > track[0].getNumSamples()-numSamples)
                   {
                       masterPosition1 = 0;
                   }
                   else if(++masterPosition2 > track[1].getNumSamples()-numSamples)
                   {
                       masterPosition2 = 0;
                   }
                   else if(++masterPosition3 > track[2].getNumSamples()-numSamples)
                   {
                       masterPosition3 = 0;
                   }
                   else if(++masterPosition4 > track[3].getNumSamples()-numSamples)
                   {
                       masterPosition4 = 0;
                   }
                   for (int i = 0; i < numOutputChannels; ++i)
                   {
                       if (outputChannelData[i] != nullptr)
                       {
                           
                          
                           
                           float sample[5];
                           

                           sample[0] = track[0].getSample(i, somesamples+masterPosition1);
                           sample[1] = track[1].getSample(i, somesamples+masterPosition2);
                           sample[2] = track[2].getSample(i, somesamples+masterPosition3);
                           sample[3] = track[3].getSample(i, somesamples+masterPosition4);
                           sample[4] = sample[0]+sample[1]+sample[2]+sample[3];
                         
                           
                           outputChannelData[i][somesamples] = sample[4];
                          
                           
//                           outputChannelData[i][somesamples] = sample2;
//                           outputChannelData[i][somesamples] = sample3;
//                           outputChannelData[i][somesamples] = sample4;
                           
                       }
                   }
                   
               }
           }
//
       
//       if(playing == true)
//       {
//           int newPositionValue = position;
//           for (int i = 0; i < numOutputChannels; ++i)
//           {
//               if (outputChannelData[i] != nullptr)
//               {
//                   position = newPositionValue;
//                   for(int samples=0; samples<numSamples; ++samples)
//                   {
//                       if(++position > track[trackNumber].getNumSamples()-numSamples)
//                       {
//                           position = 0;
//                       }
//                       
//                       float sample = track[trackNumber].getSample(i, samples+position);
//                       outputChannelData[i][samples] = sample;
//                   }
//               }
//           }
//           //           std::cout<<position<<std::endl;
//       }

    }

           int duration;
int position = 1;
           int masterPosition1=0;
           int masterPosition2=0;
           int masterPosition3=0;
           int masterPosition4=0;
int trackNumber;
AudioSampleBuffer track[4];
AudioSampleBuffer MasterBuff;
private:
    AudioFormatManager formatManager;
    AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter;
    ScopedPointer<AudioFormatReaderSource> currentRecordingSource;
    double sampleRate;
    int64 nextSampleNum;
               Time time;

    AudioSampleBuffer trackOne, trackOneOutput, trackTwo, trackThree, trackFour;
    AudioDeviceManager deviceManager;
    
    
    
    
    
               
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(recorda)
};
    
class RecordingThumb    : public Component,
                          private ChangeListener
{
public:
    RecordingThumb()
        :thumbnailCache (10),
         thumbnail (512, formatManager, thumbnailCache),
         displayFullThumb(false)
         
    {
        
        formatManager.registerBasicFormats();
        thumbnail.addChangeListener (this);
    }
    
    ~RecordingThumb()
    {
        thumbnail.removeChangeListener (this);
    }
    
    AudioThumbnail& getAudioThumbnail() {return thumbnail;}
    
    void displayFullThumbnail (bool displayFull)
    {
        displayFullThumb = displayFull;
        repaint();
    }
    
    void paint (Graphics& g)override
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::lightgrey);
        
        if (thumbnail.getTotalLength() > 0.0)
        {
//            const double endTime = displayFullThumb ? thumbnail.getTotalLength() : jmax (5.0, thumbnail.getTotalLength());
            
            Rectangle<int> thumbArea (getLocalBounds());
            thumbnail.drawChannels (g, thumbArea.reduced (2), 0.0, 1.0, 6.0f);
        }
        else
        {
            g.setFont(14.0f);
            g.drawFittedText("(No audio recorded)", getLocalBounds(), Justification::centred, 2);
        }
    }
    
private:

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;

    bool displayFullThumb;

    void changeListenerCallback(ChangeBroadcaster* source) override
        {
            if(source == &thumbnail)
                repaint();
        }
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingThumb)
};
               

class masterPlayer   :   public AudioIODeviceCallback
{
   
public:
   masterPlayer (AudioThumbnail& masterThumbnailToUpdate)
   :
    masterThumbnail(masterThumbnailToUpdate),
    recorder(thumbToRecord.getAudioThumbnail()),
    backgroundThread ("Audio Recorder Thread"),
    masterSampleRate(0), masterNextSampleNum (0)
   
   {
       backgroundThread.startThread();
       for (int i =0; i<3; ++i)
       {
           resetBuffer(i, i);
       }
   }
   
   ~masterPlayer()
   {
       stop();
   } 
   
   void resetBuffer(int trackNum, int indexNum)
   {
       for(int i=0;i<3;i++)
       {
           masterBuffer[i][i].setSize(2, 0, false, true, false);
       }
   }
   void startAD()
   {
       deviceManager.addAudioCallback(this);
       deviceManager.initialise(2, 2, 0, true, "Headphones", nullptr);
   }
   void startRecording()
   {
       
       startAD();
       for(int i=0;i<3;i++)
       {
       resetBuffer(i, i);
       }
       masterPlaying=false;
       masterRecording = true;
   }
   
   
   void closeAD()
   {
       deviceManager.closeAudioDevice();
       audioDeviceStopped();
       masterPlaying = false;
       masterRecording=false;
   }
    
   void stop()
   {
       masterRecording = false;
       closeAD();
       
   }
   
   
   
   bool masterPlaying;
   bool masterRecording;
   
   
   void audioDeviceAboutToStart (AudioIODevice* device) override
   {
       
       masterSampleRate = device -> getCurrentSampleRate();
   }
   
   void audioDeviceStopped() override
   {
       masterSampleRate = 0;
   }
   
   
   //AudioIODevice, AudioDeviceManager, getCurrentSampleRate
   
   void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                               float** outputChannelData, int numOutputChannels,
                               int numSamples) override
   {
     
       
       int oldSampleSize = masterBuffer[0][0].getNumSamples();
       int channelNumber = masterBuffer[0][0].getNumChannels();
       
       if (masterRecording == true)
       {
           std::cerr<<"master Recordind: "<<masterRecording<<std::endl;
           masterBuffer[0][0].setSize( recorder.track[0].getNumChannels(),
                                          oldSampleSize + numSamples,
                                          true,
                                          true,
                                          false);
           masterBuffer[0][0].makeCopyOf(recorder.track[0]);
//           for(int index=0;index<3;index++)
//           {
//               for(int channel=0; channel<channelNumber;++channel)
//               {
//                   
//                   masterBuffer[0][index].setSize( recorder.track[0].getNumChannels(),
//                                              oldSampleSize + numSamples,
//                                              true,
//                                              true,
//                                              false);
//                   masterBuffer[0][index].makeCopyOf(recorder.track[0]);
//                   masterBuffer[1][index].setSize( recorder.track[1].getNumChannels(),
//                                                        oldSampleSize + numSamples,
//                                                        true,
//                                                        true,
//                                                        false);
//                   masterBuffer[1][index].makeCopyOf(recorder.track[1]);
//
//                   masterBuffer[2][index].setSize( recorder.track[2].getNumChannels(),
//                                                        oldSampleSize + numSamples,
//                                                        true,
//                                                        true,
//                                                        false);
//                   masterBuffer[2][index].makeCopyOf(recorder.track[0]);
//
//                   masterBuffer[3][index].setSize( recorder.track[3].getNumChannels(),
//                                                        oldSampleSize + numSamples,
//                                                        true,
//                                                        true,
//                                                        false);
//                   masterBuffer[3][index].makeCopyOf(recorder.track[0]);
//
//               }
//           }
           
//           for(int j = 0; j < channelNumber; ++j)
//           {
//               masterBuffer[0][0].copyFrom(j, oldSampleSize, inputChannelData[j], numSamples);
//           }
           for(int i=0; i<numOutputChannels; ++i)
           {
               if(outputChannelData[i] != nullptr)
               {
                   FloatVectorOperations::clear(outputChannelData[i], numSamples);
               }
           }
      
       
           if(masterPlaying==true)
       {
           std::cerr<<"Master Play Got Called"<<std::endl;
           for(int samples=0; samples<numSamples; ++samples)
           {
               if(++masterPosition-1> masterBuffer[0][0].getNumSamples()-numSamples)
               {
                   masterPosition = 0;
               }
               for (int i = 0; i < numOutputChannels; ++i)
               {
                   if (outputChannelData[i] != nullptr)
                   {
                       float sample = masterBuffer[0][0].getSample(i, samples+masterPosition);
                       outputChannelData[i][samples] = sample;
                   }
               }
           }
       }
       
       
       //       if(playing == true)
       //       {
       //           int newPositionValue = position;
       //           for (int i = 0; i < numOutputChannels; ++i)
       //           {
       //               if (outputChannelData[i] != nullptr)
       //               {
       //                   position = newPositionValue;
       //                   for(int samples=0; samples<numSamples; ++samples)
       //                   {
       //                       if(++position > track[trackNumber].getNumSamples()-numSamples)
       //                       {
       //                           position = 0;
       //                       }
       //                       
       //                       float sample = track[trackNumber].getSample(i, samples+position);
       //                       outputChannelData[i][samples] = sample;
       //                   }
       //               }
       //           }
       //           //           std::cout<<position<<std::endl;
       //       }
       
   }
}


public:
    int masterPosition;
    int trackNumber, indexNumber;
    AudioSampleBuffer masterBuffer[3][3];
    float offset;
    int SoundOneOffset[8];
    int SoundTwoOffset[8];
    int SoundThreeOffset[8];
    int SoundFourOffset[8];
   
private:
    AudioFormatManager formatManager;
    AudioThumbnail& masterThumbnail;
    TimeSliceThread backgroundThread;
    double masterSampleRate;
    int64 masterNextSampleNum;
    AudioDeviceManager deviceManager;
    RecordingThumb thumbToRecord;
    recorda recorder;
    
   
   
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(masterPlayer)
   
   
};

class MainContentComponent   : public AudioAppComponent,
                               public ChangeListener,
                               
                               public Button::Listener
{
public:


    //==============================================================================
    MainContentComponent()
    :
      recorder(recordingThumb1.getAudioThumbnail()),
      masterplayer(recordingThumb1.getAudioThumbnail()),
      state( Starting)
      //masterplayer(recordingThumb2.getAudioThumbnail())
    
      
    {
        setOpaque(true);
        addAndMakeVisible(audioVis);
        
        addAndMakeVisible(recordingThumb1);
        addAndMakeVisible(recordingThumb2);
        addAndMakeVisible(recordingThumb3);
        addAndMakeVisible(recordingThumb4);
        
        //Track One
        addAndMakeVisible (&playButton1);
        playButton1.setButtonText("Play");
        playButton1.addListener(this);
        playButton1.setColour(TextButton::buttonColourId, Colours::green);
        playButton1.setEnabled(false);
        

        
        addAndMakeVisible (recordingButton1);
        recordingButton1.setButtonText ("Start Recording");
        recordingButton1.addListener (this);
        recordingButton1.setColour (TextButton::buttonColourId, Colours::blue);
        recordingButton1.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton1.setEnabled(true);
        

        
        //Track Two
        addAndMakeVisible (&playButton2);
        playButton2.setButtonText("Play");
        playButton2.addListener(this);
        playButton2.setColour(TextButton::buttonColourId, Colours::green);
        playButton2.setEnabled(false);
        
        
        
        addAndMakeVisible (recordingButton2);
        recordingButton2.setButtonText ("Start Record");
        recordingButton2.addListener (this);
        recordingButton2.setColour (TextButton::buttonColourId, Colours::blue);
        recordingButton2.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton2.setEnabled(true);
        
       
        
        //Track Three
        addAndMakeVisible (&playButton3);
        playButton3.setButtonText("Play");
        playButton3.addListener(this);
        playButton3.setColour(TextButton::buttonColourId, Colours::green);
        playButton3.setEnabled(false);
        
       
        
        addAndMakeVisible (recordingButton3);
        recordingButton3.setButtonText ("Start Record");
        recordingButton3.addListener (this);
        recordingButton3.setColour (TextButton::buttonColourId, Colours::blue);
        recordingButton3.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton3.setEnabled(true);
        
       
        
        //Track Four
        addAndMakeVisible (&playButton4);
        playButton4.setButtonText("Play");
        playButton4.addListener(this);
        playButton4.setColour(TextButton::buttonColourId, Colours::green);
        playButton4.setEnabled(false);
        
        
        addAndMakeVisible (recordingButton4);
        recordingButton4.setButtonText ("Start Record");
        recordingButton4.addListener (this);
        recordingButton4.setColour (TextButton::buttonColourId, Colours::blue);
        recordingButton4.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton4.setEnabled(true);
        
        addAndMakeVisible(masterPlay);
        masterPlay.setButtonText("Play");
        masterPlay.addListener(this);
        masterPlay.setColour(TextButton::buttonColourId, Colours::green);
        masterPlay.setEnabled(true);
        
        addAndMakeVisible(masterStop);
        masterStop.setButtonText("Stop");
        masterStop.addListener(this);
        masterStop.setColour(TextButton::buttonColourId, Colours::red);
        masterStop.setEnabled(true);
        
        
       
        
        
        addAndMakeVisible(measure);
        measure.button1.addListener(this);
        measure.button2.addListener(this);
        measure.button3.addListener(this);
        measure.button4.addListener(this);
        measure.button5.addListener(this);
        measure.button6.addListener(this);
        measure.button7.addListener(this);
        measure.button8.addListener(this);
        measure.button9.addListener(this);
        measure.button10.addListener(this);
        measure.button11.addListener(this);
        measure.button12.addListener(this);
        measure.button13.addListener(this);
        measure.button14.addListener(this);
        measure.button15.addListener(this);
        measure.button16.addListener(this);
        measure.button17.addListener(this);
        measure.button18.addListener(this);
        measure.button19.addListener(this);
        measure.button20.addListener(this);
        measure.button21.addListener(this);
        measure.button22.addListener(this);
        measure.button23.addListener(this);
        measure.button24.addListener(this);
        measure.button25.addListener(this);
        measure.button26.addListener(this);
        measure.button27.addListener(this);
        measure.button28.addListener(this);
        measure.button29.addListener(this);
        measure.button30.addListener(this);
        measure.button31.addListener(this);
        measure.button32.addListener(this);
        
        

        setSize (800, 800);
        
        formatManager.registerBasicFormats();
        deviceManager.removeAudioCallback(&audioVis);
        deviceManager.removeAudioCallback(&recorder);
        
        recordingMarked1 = true;
        recordingMarked2 = true;
        recordingMarked3 = true;
        recordingMarked4 = true;
        playMarker1 = true;
        playMarker2 = true;
        playMarker3 = true;
        playMarker4 = true;
        trackMarked1 = true;
        trackMarked2 = true;
        trackMarked3 = true;
        trackMarked4 = true;
        trackMarked5 = true;
        trackMarked6 = true;
        trackMarked7 = true;
        trackMarked8 = true;
        trackMarked9 = true;
        trackMarked10 = true;
        trackMarked11 = true;
        trackMarked12 = true;
        trackMarked13 = true;
        trackMarked14 = true;
        trackMarked15 = true;
        trackMarked16 = true;
        trackMarked17 = true;
        trackMarked18 = true;
        trackMarked19 = true;
        trackMarked20 = true;
        trackMarked21 = true;
        trackMarked22 = true;
        trackMarked23 = true;
        trackMarked24 = true;
        trackMarked25 = true;
        trackMarked26 = true;
        trackMarked28 = true;
        trackMarked29 = true;
        trackMarked30 = true;
        trackMarked31 = true;
        trackMarked32 = true;
        
        initThumbnail();
        

        
        // specify the number of input and output channels that we want to open
        
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        deviceManager.removeAudioCallback (&recorder);
        deviceManager.removeAudioCallback(&audioVis);
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
       
        
    }
    
    

    //=======================================================================
   void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
       
        
        
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
       
       
      
    }


    void releaseResources() override
    {
       
        
        
    }
    void initThumbnail()
    {
        recordingThumb2.getAudioThumbnail();
        recordingThumb3.getAudioThumbnail();
        recordingThumb4.getAudioThumbnail();
        
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::lightblue);

        g.setColour(Colours::white);
        g.drawLine(700,85,300,85,2);
        g.drawLine(700,170,300,170,2);
        g.drawLine(700,255,300,255,2);
        // You can add your drawing code here!
    }

    void resized() override
    {

        Rectangle<int> rect,rect2,rect3,rect4;
        rect.setBounds(290,0,420,200);
        rect2.setBounds(290,85,420,200);
        rect3.setBounds(290,170,420,200);
        rect4.setBounds(290,255,420,200);
        
        
        if(recordingMarked1==true && recorder.duration>=44100)
        {
            changeState(StopRecording1);
            std::cerr<<"change State called"<<std::endl;
        }
        measure.setBounds(300,400,450,400);
        
        recordingThumb1.setBounds(rect.removeFromTop(80).removeFromRight(600).reduced(8));
        
        
        
        recordingThumb2.setBounds(rect2.removeFromTop(80).removeFromRight(600).reduced(8));
        
        
        recordingThumb3.setBounds(rect3.removeFromTop(80).removeFromRight(600).reduced(8));
        
        
        recordingThumb4.setBounds(rect4.removeFromTop(80).removeFromRight(600).reduced(8));
        
      
        
        playButton1.setBounds(10, 10, (getWidth()/4) - 20, 20);
        recordingButton1.setBounds (10, 45, (getWidth()/4) -20, 20);
        
        
        //Track Two
        //recordingThumb.setBounds(stuff.removeFromTop(250).removeFromRight(600).reduced(8));
        
        playButton2.setBounds(10, 95, (getWidth()/4) - 20, 20);
        recordingButton2.setBounds (10, 130, (getWidth()/4) -20, 20);
        
        
        //Track Three
        //recordingThumb.setBounds(stuff.removeFromTop(400).removeFromRight(600).reduced(8));
        
        playButton3.setBounds(10, 180, (getWidth()/4) - 20, 20);
        recordingButton3.setBounds (10, 215, (getWidth()/4) -20, 20);
       
        
        //Track Two
        //recordingThumb.setBounds(stuff.removeFromTop(550).removeFromRight(600).reduced(8));
        
        playButton4.setBounds(10, 265, (getWidth()/4) - 20, 20);
        recordingButton4.setBounds (10, 300, (getWidth()/4) -20, 20);
        
        masterPlay.setBounds(10,450,(getWidth()/4) - 20, 75);
        masterStop.setBounds(10,550,(getWidth()/4) - 20, 75);
    }
    
    
   
    
    void buttonClicked (Button* button) override
    {
        if(button == &playButton1)
        {
            if(playMarker1==true)
            {
                playButtonClicked(0);
                changeState(Playing1);
            }
            else if(playMarker1==false)
            {
                stopButtonClicked();
                changeState(StopPlaying1);

            }
        }
        else if(button == &playButton2)
        {
            if(playMarker2==true)
            {
                playButtonClicked(1);
                changeState(Playing2);
            }
            else if(playMarker2==false)
            {
                stopButtonClicked();
                changeState(StopPlaying2);
            }
        }
        else if(button == &playButton3)
        {
            if(playMarker3==true)
            {
                playButtonClicked(2);
                changeState(Playing3);
            }
            else if(playMarker3==false)
            {
                stopButtonClicked();
                changeState(StopPlaying3);
            }
        }
        else if(button == &playButton4)
        {
            if(playMarker4==true)
            {
                playButtonClicked(3);
                changeState(Playing4);
            }
            else if(playMarker4==false)
            {
                stopButtonClicked();
                changeState(StopPlaying4);
            }
        }
      
        if (button == &recordingButton1)
        {
            if(recordingMarked1==true)
            {
                recordingTrackNumber = 0;
                startRecording(recordingTrackNumber);
                changeState(Recording1);
            }
            else if(recordingMarked1==false /*|| recorder.duration<=44100*/)
            {
                std::cerr<<"Recording Stop Test";
                stopRecording();
                changeState(StopRecording1);
            }
           
        }
        else if(button == &recordingButton2)
        {
            if(recordingMarked2==true)
            {
                recordingTrackNumber = 1;
                startRecording(recordingTrackNumber);
                changeState(Recording2);
            }
            else if(recordingMarked2==false)
            {
                stopRecording();
                changeState(StopRecording2);
                
            }
        }
        else if(button == &recordingButton3)
        {
            if(recordingMarked3==true)
            {
                recordingTrackNumber=2;
                startRecording(recordingTrackNumber);
                changeState(Recording3);

            }
            else if(recordingMarked3==false)
            {
                stopRecording();
                changeState(StopRecording3);
                
            }
        }
        else if(button == &recordingButton4)
        {
            if(recordingMarked4==true)
            {
                recordingTrackNumber=3;
                startRecording(recordingTrackNumber);
                changeState(Recording4);
            }
            else if(recordingMarked4==false)
            {
                stopRecording();
                changeState(StopRecording4);
                
            }
        }
        
        if(button==&masterPlay)
        {
            playMasterTrack();
        }
        if(button==&masterStop)
        {
            stopMasterTrack();
        }


        if(button == &measure.button1)
        {   
            
            if(trackMarked1 == true)
            {
                setSoundOneOffset(0);
                measure.button1.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked1 = false;
            }
            else if (trackMarked1==false)
            {
                measure.button1.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked1 = true;
            }
        }
        
        if(button == &measure.button2)
        {   
            
            if(trackMarked2 == true)
            {
                measure.button2.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked2 = false;
            }
            else if (trackMarked2==false)
            {
                measure.button2.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked2 = true;
            }
        }
         if(button == &measure.button3)
        {   
            
            if(trackMarked3 == true)
            {
                measure.button3.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked3 = false;
            }
            else if (trackMarked3==false)
            {
                measure.button3.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked3 = true;
            }
        }
         if(button == &measure.button4)
        {   
            
            if(trackMarked4 == true)
            {
                measure.button4.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked4 = false;
            }
            else if (trackMarked4==false)
            {
                measure.button4.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked4 = true;
            }
        }
         if(button == &measure.button5)
        {   
            
            if(trackMarked5 == true)
            {
                measure.button5.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked5 = false;
            }
            else if (trackMarked5==false)
            {
                measure.button5.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked5 = true;
            }
        }
         if(button == &measure.button6)
        {   
            
            if(trackMarked6 == true)
            {
                measure.button6.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked6 = false;
            }
            else if (trackMarked6==false)
            {
                measure.button6.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked6 = true;
            }
        }
         if(button == &measure.button7)
        {   
            
            if(trackMarked7 == true)
            {
                measure.button7.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked7 = false;
            }
            else if (trackMarked7==false)
            {
                measure.button7.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked7 = true;
            }
        }
         if(button == &measure.button8)
        {   
            
            if(trackMarked8 == true)
            {
                measure.button8.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked8 = false;
            }
            else if (trackMarked8==false)
            {
                measure.button8.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked8 = true;
            }
        }
         if(button == &measure.button9)
        {   
            
            if(trackMarked9 == true)
            {
                measure.button9.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked9 = false;
            }
            else if (trackMarked9==false)
            {
                measure.button9.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked9 = true;
            }
        }
         if(button == &measure.button10)
        {   
            
            if(trackMarked10 == true)
            {
                measure.button10.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked10 = false;
            }
            else if (trackMarked10==false)
            {
                measure.button10.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked10 = true;
            }
        }
         if(button == &measure.button11)
        {   
            
            if(trackMarked11 == true)
            {
                measure.button11.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked11 = false;
            }
            else if (trackMarked11==false)
            {
                measure.button11.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked11 = true;
            }
        }
         if(button == &measure.button12)
        {   
            
            if(trackMarked12 == true)
            {
                measure.button12.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked12 = false;
            }
            else if (trackMarked12==false)
            {
                measure.button12.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked12 = true;
            }
        }
         if(button == &measure.button13)
        {   
            
            if(trackMarked13 == true)
            {
                measure.button13.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked13 = false;
            }
            else if (trackMarked13==false)
            {
                measure.button13.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked13 = true;
            }
        }
         if(button == &measure.button14)
        {   
            
            if(trackMarked14 == true)
            {
                measure.button14.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked14 = false;
            }
            else if (trackMarked14==false)
            {
                measure.button14.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked14 = true;
            }
        }
         if(button == &measure.button15)
        {   
            
            if(trackMarked15 == true)
            {
                measure.button15.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked15 = false;
            }
            else if (trackMarked15==false)
            {
                measure.button15.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked15 = true;
            }
        }
         if(button == &measure.button16)
        {   
            
            if(trackMarked16 == true)
            {
                measure.button16.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked16 = false;
            }
            else if (trackMarked16==false)
            {
                measure.button16.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked16 = true;
            }
        }
         if(button == &measure.button17)
        {   
            
            if(trackMarked17 == true)
            {
                measure.button17.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked17 = false;
            }
            else if (trackMarked17==false)
            {
                measure.button17.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked17 = true;
            }
        }
         if(button == &measure.button18)
        {   
            
            if(trackMarked18 == true)
            {
                measure.button18.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked18 = false;
            }
            else if (trackMarked18==false)
            {
                measure.button18.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked18 = true;
            }
        }
         if(button == &measure.button19)
        {   
            
            if(trackMarked19 == true)
            {
                measure.button19.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked19 = false;
            }
            else if (trackMarked19==false)
            {
                measure.button19.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked19 = true;
            }
        }
         if(button == &measure.button20)
        {   
            
            if(trackMarked20 == true)
            {
                measure.button20.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked20 = false;
            }
            else if (trackMarked20==false)
            {
                measure.button20.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked20 = true;
            }
        }
         if(button == &measure.button21)
        {   
            
            if(trackMarked21 == true)
            {
                measure.button21.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked21 = false;
            }
            else if (trackMarked21==false)
            {
                measure.button21.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked21 = true;
            }
        }
         if(button == &measure.button22)
        {   
            
            if(trackMarked22 == true)
            {
                measure.button22.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked22 = false;
            }
            else if (trackMarked22==false)
            {
                measure.button22.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked22 = true;
            }
        }
         if(button == &measure.button23)
        {   
            
            if(trackMarked23 == true)
            {
                measure.button23.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked23 = false;
            }
            else if (trackMarked23==false)
            {
                measure.button23.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked23 = true;
            }
        }
         if(button == &measure.button24)
        {   
            
            if(trackMarked24 == true)
            {
                measure.button24.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked24 = false;
            }
            else if (trackMarked24==false)
            {
                measure.button24.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked24 = true;
            }
        }
         if(button == &measure.button25)
        {   
            
            if(trackMarked25 == true)
            {
                measure.button25.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked25 = false;
            }
            else if (trackMarked25==false)
            {
                measure.button25.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked25 = true;
            }
        }
         if(button == &measure.button26)
        {   
            
            if(trackMarked26 == true)
            {
                measure.button26.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked26 = false;
            }
            else if (trackMarked26==false)
            {
                measure.button26.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked26 = true;
            }
        }
         if(button == &measure.button27)
        {   
            
            if(trackMarked27 == true)
            {
                measure.button27.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked27 = false;
            }
            else if (trackMarked27==false)
            {
                measure.button27.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked27 = true;
            }
        }
         if(button == &measure.button28)
        {   
            
            if(trackMarked28 == true)
            {
                measure.button28.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked28 = false;
            }
            else if (trackMarked28==false)
            {
                measure.button28.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked28 = true;
            }
        }
         if(button == &measure.button29)
        {   
            
            if(trackMarked29 == true)
            {
                measure.button29.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked29 = false;
            }
            else if (trackMarked29==false)
            {
                measure.button29.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked29 = true;
            }
        }
         if(button == &measure.button30)
        {   
            
            if(trackMarked30 == true)
            {
                measure.button30.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked30 = false;
            }
            else if (trackMarked30==false)
            {
                measure.button30.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked30 = true;
            }
        }
         if(button == &measure.button31)
        {   
            
            if(trackMarked31 == true)
            {
                measure.button31.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked31 = false;
            }
            else if (trackMarked31==false)
            {
                measure.button31.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked31 = true;
            }
        }
         if(button == &measure.button32)
        {   
            
            if(trackMarked32 == true)
            {
                measure.button32.setColour(TextButton::buttonColourId, Colours::red);
                trackMarked32 = false;
            }
            else if (trackMarked32==false)
            {
                measure.button32.setColour(TextButton::buttonColourId, Colours::blue);
                trackMarked32 = true;
            }
        }
        
    }

    
 
    void startRecording(int trackNumeral)
    {
        
        //setAudioChannels(2,0);
        recorder.duration=0;
        recorder.playing=false;
        recorder.recording=true;
        recorder.trackNumber = trackNumeral;
        std::cerr<<recorder.trackNumber;
        recorder.startRec();
        if(recorder.trackNumber==0)
        {
            recordingButton2.setEnabled(false);
            recordingButton3.setEnabled(false);
            recordingButton4.setEnabled(false);
        }
        else if(recorder.trackNumber==1)
        {
            recordingButton1.setEnabled(false);
            recordingButton3.setEnabled(false);
            recordingButton4.setEnabled(false);
        }
        else if(recorder.trackNumber==2)
        {
            recordingButton1.setEnabled(false);
            recordingButton2.setEnabled(false);
            recordingButton4.setEnabled(false);
        }
        else if(recorder.trackNumber==3)
        {
            recordingButton1.setEnabled(false);
            recordingButton2.setEnabled(false);
            recordingButton3.setEnabled(false);
        }
    }
    
    void setSoundOneOffset(int numeral)
    {
        masterplayer.startRecording();
    }
    void playMasterTrack()
    {
        
        recorder.recording=false;
        recorder.playing=false;
        recorder.masterplaying=true;
        recorder.startAD();
        recorder.masterPosition1 = 0;
        recorder.masterPosition2 = 0;
        recorder.masterPosition3 = 0;
        recorder.masterPosition4 = 0;
        setAudioChannels(2,2);
        
    }
    void stopMasterTrack()
    {
        recorder.masterplaying = false;
        recorder.closeAD();
        setAudioChannels(0,0);
        
    }
    void masterStopRecording()
    {
        masterplayer.stop();
        masterplayer.masterRecording = false;
    }
    
//    void setSoundOneOffset(int numeral)
//    {
//        int masterOffset[4];
//        int setOffset = 44100;
//        if(numeral==0 && trackMarked1==true)
//        {
//            masterOffset[numeral] = setOffset*numeral;
//            //call on function that appends track[trackNumber] to masterBuffer[0][numeral]
//        }
//        else if(numeral==0 && trackMarked1==false)
//        {
//            masterplayer.masterBuffer[0][numeral].clear();
//        }
//        else if(numeral==1 && trackMarked2==true)
//        {
//            masterOffset[numeral] = setOffset*numeral;
//        }
//        else if(numeral==1 && trackMarked2==false)
//        {
//            masterplayer.masterBuffer[0][numeral].clear();
//        }
//        else if(numeral==2 && trackMarked3==true)
//        {
//            masterOffset[numeral] = setOffset*numeral;
//        }
//        else if(numeral==2 && trackMarked3==false)
//        {
//            masterplayer.masterBuffer[0][numeral].clear();
//        }
//        else if(numeral==3 && trackMarked4==true)
//        {
//            masterOffset[numeral] = setOffset*numeral;
//        }
//        else if(numeral==3 && trackMarked4==false)
//        {
//            masterplayer.masterBuffer[0][numeral].clear();
//        }
//       
//    }
    void stopRecording()
    {
        recorder.stop();
        recorder.recording = false;
        
    }
   
    void setStopRecordingButtonGUI()
    {
        recordingButton1.setButtonText("Start Recording");
        recordingButton1.setColour(TextButton::buttonColourId, Colours::blue);
        recordingMarked1 = true;
        recordingButton2.setEnabled(true);
        recordingButton3.setEnabled(true);
        recordingButton4.setEnabled(true);
    }
    
    
    
    void playButtonClicked(int trackNumeral)
    {
        //setAudioChannels(2,2);
        recorder.recording=false;
        recorder.trackNumber = trackNumeral;
        recorder.startAD();
        recorder.playing=true;
        recorder.position = 0;
    
        
    }
    
    void stopButtonClicked()
    {
        recorder.playing = false;
        recorder.closeAD();
        setAudioChannels(0,0);
        //recordingThumb.displayFullThumbnail(true);
    }
   
   
public:
    int recordingTrackNumber;
private:
    //==============================================================================

    // Your private member variables go here...
    enum guiState{
        Starting,
        Recording1,
        StopRecording1,
        Recording2,
        StopRecording2,
        Recording3,
        StopRecording3,
        Recording4,
        StopRecording4,
        Playing1,
        StopPlaying1,
        Playing2,
        StopPlaying2,
        Playing3,
        StopPlaying3,
        Playing4,
        StopPlaying4
        
    };
    
    void changeState(guiState newState)
    {
        if(state != newState)
        {
            state = newState;
            
            switch (state)
            {
                case Starting:
                    recordingButton1.setButtonText("Start Recording");
                    recordingButton1.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked1 = true;
                    recordingButton2.setButtonText("Start Recording");
                    recordingButton2.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked2 = true;
                    recordingButton3.setButtonText("Start Recording");
                    recordingButton3.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked3 = true;
                    recordingButton4.setButtonText("Start Recording");
                    recordingButton4.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked4 = true;
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton1.setButtonText("Play");
                    playButton2.setButtonText("Play");
                    playButton3.setButtonText("Play");
                    playButton4.setButtonText("Play");
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    break;
                
                case Recording1:
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    recordingButton1.setButtonText("Stop Recording");
                    recordingButton1.setColour(TextButton::buttonColourId, Colour(0xffff5c6f));
                     recordingMarked1 = false;
                    break;
                
                case StopRecording1:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton1.setEnabled(true);
                    recordingButton1.setButtonText("Start Recording");
                    recordingButton1.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked1 = true;
                    trackDone1 = true;
                    if(trackDone2==true)
                    {
                        playButton2.setEnabled(true);
                    }
                    if(trackDone3==true)
                    {
                        playButton3.setEnabled(true);
                        
                    }
                    if(trackDone4==true)
                    {
                        playButton4.setEnabled(true);
                    }

                    break;
                    
                case Recording2:
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    recordingButton1.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    recordingButton2.setButtonText("Stop Recording");
                    recordingButton2.setColour(TextButton::buttonColourId, Colour(0xffff5c6f));
                    recordingMarked2 = false;
                    
                    break;
                
                case StopRecording2:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton2.setEnabled(true);
                    recordingButton2.setButtonText("Start Recording");
                    recordingButton2.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked2 = true;
                    trackDone2=true;
                        if(trackDone1==true)
                        {
                            playButton1.setEnabled(true);
                        }
                         if(trackDone3==true)
                        {
                            playButton3.setEnabled(true);
                            
                        }
                        if(trackDone4==true)
                        {
                            playButton4.setEnabled(true);
                        }
                    break;
                    
                case Recording3:
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    recordingButton3.setButtonText("Stop Recording");
                    recordingButton3.setColour(TextButton::buttonColourId, Colour(0xffff5c6f));
                    recordingMarked3 = false;
                    break;
                    
                case StopRecording3:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton3.setEnabled(true);
                    recordingButton3.setButtonText("Start Recording");
                    recordingButton3.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked3 = true;
                    trackDone3=true;
                    if(trackDone1==true)
                    {
                        playButton1.setEnabled(true);
                    }
                    if(trackDone2==true)
                    {
                        playButton2.setEnabled(true);
                        
                    }
                    if(trackDone4==true)
                    {
                        playButton4.setEnabled(true);
                    }

                    break;
                    
                case Recording4:
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setButtonText("Stop Recording");
                    recordingButton4.setColour(TextButton::buttonColourId, Colour(0xffff5c6f));
                    recordingMarked4 = false;
                    break;
                    
                case StopRecording4:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton4.setEnabled(true);
                    recordingButton4.setButtonText("Start Recording");
                    recordingButton4.setColour(TextButton::buttonColourId, Colours::blue);
                    recordingMarked4 = true;
                    trackDone4=true;
                    if(trackDone1==true)
                    {
                        playButton1.setEnabled(true);
                    }
                    else if(trackDone2==true)
                    {
                        playButton2.setEnabled(true);
                        
                    }
                    else if(trackDone3==true)
                    {
                        playButton3.setEnabled(true);
                    }

                    break;
                
                case Playing1:
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    playButton1.setButtonText("Stop");
                    playButton1.setColour(TextButton::buttonColourId, Colours::red);
                    playMarker1=false;
                    break;
                    
                case StopPlaying1:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton2.setEnabled(true);
                    playButton3.setEnabled(true);
                    playButton4.setEnabled(true);
                    playButton1.setButtonText("Play");
                    playButton1.setColour(TextButton::buttonColourId, Colours::green);
                    playMarker1=true;
                    break;
                    
                    
                case Playing2:
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    playButton1.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setEnabled(false);
                    playButton2.setButtonText("Stop");
                    playButton2.setColour(TextButton::buttonColourId, Colours::red);
                    playMarker2=false;
                    break;
                    
                case StopPlaying2:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton1.setEnabled(true);
                    playButton3.setEnabled(true);
                    playButton4.setEnabled(true);
                    playButton2.setButtonText("Play");
                    playButton2.setColour(TextButton::buttonColourId, Colours::green);
                    playMarker2=true;
                    break;
                    
                    
                case Playing3:
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton4.setEnabled(false);
                    playButton3.setButtonText("Stop");
                    playButton3.setColour(TextButton::buttonColourId, Colours::red);
                    playMarker3=false;
                    break;
                    
                case StopPlaying3:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton1.setEnabled(true);
                    playButton2.setEnabled(true);
                    playButton4.setEnabled(true);
                    playButton3.setButtonText("Play");
                    playButton3.setColour(TextButton::buttonColourId, Colours::green);
                    playMarker3=true;
                    break;
                    
                    
                case Playing4:
                    recordingButton1.setEnabled(false);
                    recordingButton2.setEnabled(false);
                    recordingButton3.setEnabled(false);
                    recordingButton4.setEnabled(false);
                    playButton1.setEnabled(false);
                    playButton2.setEnabled(false);
                    playButton3.setEnabled(false);
                    playButton4.setButtonText("Stop");
                    playButton4.setColour(TextButton::buttonColourId, Colours::red);
                    playMarker4=false;
                    break;
                    
                case StopPlaying4:
                    recordingButton1.setEnabled(true);
                    recordingButton2.setEnabled(true);
                    recordingButton3.setEnabled(true);
                    recordingButton4.setEnabled(true);
                    playButton1.setEnabled(true);
                    playButton2.setEnabled(true);
                    playButton3.setEnabled(true);
                    playButton4.setButtonText("Play");
                    playButton4.setColour(TextButton::buttonColourId, Colours::green);
                    playMarker4=true;
                    break;



                    
                    
                    



                
                    
                  
                    
            }
            
        }
    }
    
    bool trackMarked1,trackMarked2,trackMarked3,trackMarked4,trackMarked5,trackMarked6,trackMarked7,trackMarked8,
    trackMarked9,trackMarked10,trackMarked11,trackMarked12,trackMarked13,trackMarked14,trackMarked15,trackMarked16,
    trackMarked17,trackMarked18,trackMarked19,trackMarked20,trackMarked21,trackMarked22,trackMarked23,trackMarked24,
    trackMarked25,trackMarked26,trackMarked27,trackMarked28,trackMarked29,trackMarked30,trackMarked31,trackMarked32;
    bool recordingMarked1, recordingMarked2, recordingMarked3, recordingMarked4;
    bool playMarker1, playMarker2, playMarker3, playMarker4;
    bool trackDone1, trackDone2, trackDone3, trackDone4;
    AudioFormatManager formatManager;
    
    AudioVisual audioVis;
    RecordingThumb recordingThumb1, recordingThumb2, recordingThumb3, recordingThumb4;
    recorda recorder;
    masterPlayer masterplayer;
    //LiveScrollingAudioDisplay liveAudioScroller;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
   
   
    TextButton playButton1, playButton2, playButton3, playButton4;
    TextButton stopButton1, stopButton2, stopButton3, stopButton4;
    TextButton recordingButton1, recordingButton2, recordingButton3, recordingButton4;
    TextButton stopRecordingButton1, stopRecordingButton2, stopRecordingButton3, stopRecordingButton4;
    TextButton masterPlay, masterStop;
    
    Measure measure;
    TextButton
    //Sound One
    button1, button2, button3, button4, button5, button6, button7, button8,
    //Sound Two
    button9, button10, button11, button12, button13, button14, button15, button16,
    //Sound Three
    button17, button18, button19, button20,button21, button22, button23, button24,
    //Sound Four
    button25, button26, button27, button28, button29, button30, button31, button32;
    bool setRec;
    guiState state;

    
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }



#endif  // MAINCOMPONENT_H_INCLUDED
               
               

