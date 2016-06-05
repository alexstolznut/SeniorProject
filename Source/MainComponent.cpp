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
//class masterPlayer  :   public AudioIODeviceCallback
//{
//public:
//    masterPlayer
//    :   masterSampleRate(0), nextSampleNum(0)
//    {
//        void starMastertAD()
//        {
//            deviceManager.addAudioCallback(this);
//            deviceManager.initialise(2, 2, 0, true, "Headphones", nullptr);
//        }
//        
//        void closeMasterAD()
//        {
//            deviceManager.closeAudioDevice();
//            audioDeviceStopped();
//        }
//        void audioDeviceAboutToStart (AudioIODevice* device) override
//        {
//            
//            sampleRate = device -> getCurrentSampleRate();
//        }
//        
//        void audioDeviceStopped() override
//        {
//            sampleRate = 0;
//        }
//        
//        void AudioIODeviceCallback(const float** inputChannelData, int numInputChannels,
//                                   float** outputChannelData, int numOutputChannels,
//                                   int numSamples)override
//        {
//            int oldSampleSize[3];
//            int channelNumber[3];
//            
//            for(int i=0; i<3; ++i)
//            {
//                int masterOldSampleSize[i] = recorder.track[i].getNumSamples() * 4;
//                int masterChannelNumber[i] = recorder.track[i].getNumChannels();
//                
//                setSize.masterBuffer[i](masterBuffer[i].getNumChannels(),
//                                        masterOldSampleSize[i] + numSamples,
//                                        true,
//                                        true,
//                                        false);
//            }
//            for(int j=0; j<channelNumber[0]; ++j)
//            {
//                for(int masterTrackNumber=0; masterTrackNumber<3;++masterTrackNumber)
//                {
//                    addFrom.masterBuffer[masterTrackNumber](j, masterOldSampleSize, recorder.track[masterTrackNumber], recorder.track[0].getNumChannels(), 0, numSamples);
//                }
//
//            }
//            if(masterPlay == true)
//            {
//                for(int samples=0; samples<numSamples; ++samples)
//                {
//                    if(++masterPosition > masterBuffer[masterTrackNumber].getNumSamples()-numSamples)
//                    {
//                        masterPosition = 0;
//                    }
//                    for (int i = 0; i < numOutputChannels; ++i)
//                    {
//                        if (outputChannelData[i] != nullptr)
//                        {
//                            float sample = track[trackNumber].getSample(i, samples+position);
//                            outputChannelData[i][samples] = sample;
//                        }
//                    }
//                }
//                
//            }
//            
//        }
//        
//    }
//
//    bool masterPlay;
//private:
//    AudioDeviceManager masterDeviceManager;
//    double masterSampleRate;
//    int64 nextSampleNum;
//    AudioSampleBuffer masterBuffer[3];
//    TimeSliceThread masterBackgroundThread;
//    int masterTrackNumber;
//    recorda recorder;
//};
class recorda   :   public AudioIODeviceCallback
{

public:
    recorda (AudioThumbnail& thumbnailToUpdate)
        :   thumbnail(thumbnailToUpdate),
            backgroundThread ("Audio Recorder Thread"),
            sampleRate(0), nextSampleNum (0), activeWriter(nullptr)
    
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

       if (recording == true)
       {
           track[trackNumber].setSize( track[trackNumber].getNumChannels(),
                             oldSampleSize + numSamples,
                             true,
                             true,
                             false);
//       trackOneOutput.setSize(trackOneOutput.getNumChannels(),
//                              oldSampleSize + numSamples,
//                              true,
//                              true,
//                              false);
    
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
       }
       
       if(playing == true)
       {
           for(int samples=0; samples<numSamples; ++samples)
           {
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


int position;
int trackNumber;
AudioSampleBuffer track[3];
    
private:
    AudioFormatManager formatManager;
    AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter;
    ScopedPointer<AudioFormatReaderSource> currentRecordingSource;
    double sampleRate;
    int64 nextSampleNum;

    AudioSampleBuffer trackOne, trackOneOutput, trackTwo, trackThree, trackFour;
    AudioDeviceManager deviceManager;
    
    
    
    
    
    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
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
            const double endTime = displayFullThumb ? thumbnail.getTotalLength() : jmax (30.0, thumbnail.getTotalLength());
            
            Rectangle<int> thumbArea (getLocalBounds());
            thumbnail.drawChannels (g, thumbArea.reduced (2), 0.0, endTime, 1.0f);
        }
        else
        {
            g.setFont(14.0f);
            g.drawFittedText("(No file recorded)", getLocalBounds(), Justification::centred, 2);
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

class MainContentComponent   : public AudioAppComponent,
                               public ChangeListener,
                               
                               public Button::Listener
{
public:


    //==============================================================================
    MainContentComponent()
    :
      recorder(recordingThumb1.getAudioThumbnail())
    
      
    {
        setOpaque(true);
        addAndMakeVisible(audioVis);
        
        addAndMakeVisible(recordingThumb1);
        
        //Track One
        addAndMakeVisible (&playButton1);
        playButton1.setButtonText("Play");
        playButton1.addListener(this);
        playButton1.setColour(TextButton::buttonColourId, Colours::green);
        playButton1.setEnabled(true);
        
        addAndMakeVisible(&stopButton1);
        stopButton1.setButtonText("Stop");
        stopButton1.addListener(this);
        stopButton1.setColour(TextButton::buttonColourId, Colours::red);
        stopButton1.setEnabled(true);
        
        addAndMakeVisible (recordingButton1);
        recordingButton1.setButtonText ("Record");
        recordingButton1.addListener (this);
        recordingButton1.setColour (TextButton::buttonColourId, Colour (0xffff5c6f));
        recordingButton1.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton1.setEnabled(true);
        
        addAndMakeVisible(stopRecordingButton1);
        stopRecordingButton1.setButtonText("Stop Recording");
        stopRecordingButton1.addListener (this);
        stopRecordingButton1.setColour (TextButton::buttonColourId, Colour(0x44444444));
        stopRecordingButton1.setColour(TextButton::textColourOnId, Colours::white);
        
        //Track Two
        addAndMakeVisible (&playButton2);
        playButton2.setButtonText("Play");
        playButton2.addListener(this);
        playButton2.setColour(TextButton::buttonColourId, Colours::green);
        playButton2.setEnabled(true);
        
        addAndMakeVisible(&stopButton2);
        stopButton2.setButtonText("Stop");
        stopButton2.addListener(this);
        stopButton2.setColour(TextButton::buttonColourId, Colours::red);
        stopButton2.setEnabled(true);
        
        addAndMakeVisible (recordingButton2);
        recordingButton2.setButtonText ("Record");
        recordingButton2.addListener (this);
        recordingButton2.setColour (TextButton::buttonColourId, Colour (0xffff5c6f));
        recordingButton2.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton2.setEnabled(true);
        
        addAndMakeVisible(stopRecordingButton2);
        stopRecordingButton2.setButtonText("Stop Recording");
        stopRecordingButton2.addListener (this);
        stopRecordingButton2.setColour (TextButton::buttonColourId, Colour(0x44444444));
        stopRecordingButton2.setColour(TextButton::textColourOnId, Colours::white);
        
        //Track Three
        addAndMakeVisible (&playButton3);
        playButton3.setButtonText("Play");
        playButton3.addListener(this);
        playButton3.setColour(TextButton::buttonColourId, Colours::green);
        playButton3.setEnabled(true);
        
        addAndMakeVisible(&stopButton3);
        stopButton3.setButtonText("Stop");
        stopButton3.addListener(this);
        stopButton3.setColour(TextButton::buttonColourId, Colours::red);
        stopButton3.setEnabled(true);
        
        addAndMakeVisible (recordingButton3);
        recordingButton3.setButtonText ("Record");
        recordingButton3.addListener (this);
        recordingButton3.setColour (TextButton::buttonColourId, Colour (0xffff5c6f));
        recordingButton3.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton3.setEnabled(true);
        
        addAndMakeVisible(stopRecordingButton3);
        stopRecordingButton3.setButtonText("Stop Recording");
        stopRecordingButton3.addListener (this);
        stopRecordingButton3.setColour (TextButton::buttonColourId, Colour(0x44444444));
        stopRecordingButton3.setColour(TextButton::textColourOnId, Colours::white);
        
        //Track Four
        addAndMakeVisible (&playButton4);
        playButton4.setButtonText("Play");
        playButton4.addListener(this);
        playButton4.setColour(TextButton::buttonColourId, Colours::green);
        playButton4.setEnabled(true);
        
        addAndMakeVisible(&stopButton4);
        stopButton4.setButtonText("Stop");
        stopButton4.addListener(this);
        stopButton4.setColour(TextButton::buttonColourId, Colours::red);
        stopButton4.setEnabled(true);
        
        addAndMakeVisible (recordingButton4);
        recordingButton4.setButtonText ("Record");
        recordingButton4.addListener (this);
        recordingButton4.setColour (TextButton::buttonColourId, Colour (0xffff5c6f));
        recordingButton4.setColour (TextButton::textColourOnId, Colours::black);
        recordingButton4.setEnabled(true);
        
        addAndMakeVisible(stopRecordingButton4);
        stopRecordingButton4.setButtonText("Stop Recording");
        stopRecordingButton4.addListener (this);
        stopRecordingButton4.setColour (TextButton::buttonColourId, Colour(0x44444444));
        stopRecordingButton4.setColour(TextButton::textColourOnId, Colours::white);
      
        
        
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

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::lightblue);


        // You can add your drawing code here!
    }

    void resized() override
    {

        
        Rectangle<int> stuff (getLocalBounds());
        
        
        measure.setBounds(350,100,450,400);
        measure.button1.setBounds(50,70,10,10);
        measure.button2.setBounds(100,70,10,10);
        measure.button3.setBounds(150,70,10,10);
        measure.button4.setBounds(200,70,10,10);
        measure.button5.setBounds(250,70,10,10);
        measure.button6.setBounds(300,70,10,10);
        measure.button7.setBounds(350,70,10,10);
        measure.button8.setBounds(400,70,10,10);
        
        //Sound Two
        measure.button9.setBounds(50,145,10,10);
        measure.button10.setBounds(100,145,10,10);
        measure.button11.setBounds(150,145,10,10);
        measure.button12.setBounds(200,145,10,10);
        measure.button13.setBounds(250,145,10,10);
        measure.button14.setBounds(300,145,10,10);
        measure.button15.setBounds(350,145,10,10);
        measure.button16.setBounds(400,145,10,10);
        
        //Sound Three
        measure.button17.setBounds(50,220,10,10);
        measure.button18.setBounds(100,220,10,10);
        measure.button19.setBounds(150,220,10,10);
        measure.button20.setBounds(200,220,10,10);
        measure.button21.setBounds(250,220,10,10);
        measure.button22.setBounds(300,220,10,10);
        measure.button23.setBounds(350,220,10,10);
        measure.button24.setBounds(400,220,10,10);
        
        //Sound Four
        
        measure.button25.setBounds(50,295,10,10);
        measure.button26.setBounds(100,295,10,10);
        measure.button27.setBounds(150,295,10,10);
        measure.button28.setBounds(200,295,10,10);
        measure.button29.setBounds(250,295,10,10);
        measure.button30.setBounds(300,295,10,10);
        measure.button31.setBounds(350,295,10,10);
        measure.button32.setBounds(400,295,10,10);
        
        //Track One
        recordingThumb1.setBounds(stuff.removeFromTop(100).removeFromRight(600).reduced(8));
        
        playButton1.setBounds(10, 10, (getWidth()/4) - 20, 20);
        stopButton1.setBounds(10, 45, (getWidth()/4) -20, 20);
        recordingButton1.setBounds (10, 75, (getWidth()/4) -20, 20);
        stopRecordingButton1.setBounds (10, 105, (getWidth()/4) -20, 20);
        
        //Track Two
        //recordingThumb.setBounds(stuff.removeFromTop(250).removeFromRight(600).reduced(8));
        
        playButton2.setBounds(10, 165, (getWidth()/4) - 20, 20);
        stopButton2.setBounds(10, 195, (getWidth()/4) -20, 20);
        recordingButton2.setBounds (10, 225, (getWidth()/4) -20, 20);
        stopRecordingButton2.setBounds (10, 255, (getWidth()/4) -20, 20);
        
        //Track Three
        //recordingThumb.setBounds(stuff.removeFromTop(400).removeFromRight(600).reduced(8));
        
        playButton3.setBounds(10, 315, (getWidth()/4) - 20, 20);
        stopButton3.setBounds(10, 345, (getWidth()/4) -20, 20);
        recordingButton3.setBounds (10, 375, (getWidth()/4) -20, 20);
        stopRecordingButton3.setBounds (10, 405, (getWidth()/4) -20, 20);
        
        //Track Two
        //recordingThumb.setBounds(stuff.removeFromTop(550).removeFromRight(600).reduced(8));
        
        playButton4.setBounds(10, 465, (getWidth()/4) - 20, 20);
        stopButton4.setBounds(10, 495, (getWidth()/4) -20, 20);
        recordingButton4.setBounds (10, 525, (getWidth()/4) -20, 20);
        stopRecordingButton4.setBounds (10, 555, (getWidth()/4) -20, 20);
       
        
        
      
    }
    
    
    
   
    
    void buttonClicked (Button* button) override
    {
        if(button == &playButton1)
        {
            playButtonClicked(0);
        }
        else if(button == &playButton2)
        {
            playButtonClicked(1);
        }
        else if(button == &playButton3)
        {
            playButtonClicked(2);
        }
        else if(button == &playButton4)
        {
            playButtonClicked(3);
        }
        if(button == &stopButton1)
        {
            stopButtonClicked();
        }
        if(button == &stopButton2)
        {
            stopButtonClicked();
        }
        if(button == &stopButton3)
        {
            stopButtonClicked();
        }
        if(button == &stopButton4)
        {
            stopButtonClicked();
        }
        if (button == &recordingButton1)
        {
            startRecording(0);
           
        }
        else if(button == &recordingButton2)
        {
            startRecording(1);
        }
        else if(button == &recordingButton3)
        {
            startRecording(2);
        }
        else if(button == &recordingButton4)
        {
            startRecording(3);
        }
       
        if (button == &stopRecordingButton1)
        {
         stopRecording();
        }
        if (button == &stopRecordingButton2)
        {
            stopRecording();
        }
        if (button == &stopRecordingButton2)
        {
            stopRecording();
        }
        if (button == &stopRecordingButton3)
        {
            stopRecording();
        }
        if (button == &stopRecordingButton4)
        {
            stopRecording();
        }
        if(button == &measure.button1)
        {   
            
            if(trackMarked1 == true)
            {
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
        recorder.trackNumber = trackNumeral;
        recorder.playing=false;
        recorder.recording=true;
        recorder.startRec();
        
        
    }
    void stopRecording()
    {
        recorder.stop();
        recorder.recording = false;
        
    }
  
    
    
            
    
    void playButtonClicked(int trackNumeral)
    {
        //setAudioChannels(2,2);
        recorder.trackNumber = trackNumeral;
        recorder.startAD();
        recorder.playing=true;
        recorder.recording=false;
        recorder.position = 0;
        
    }
    
    void stopButtonClicked()
    {
        recorder.playing = false;
        recorder.closeAD();
        setAudioChannels(0,0);
        //recordingThumb.displayFullThumbnail(true);
    }


private:
    //==============================================================================

    // Your private member variables go here...

    
    bool trackMarked1,trackMarked2,trackMarked3,trackMarked4,trackMarked5,trackMarked6,trackMarked7,trackMarked8,
    trackMarked9,trackMarked10,trackMarked11,trackMarked12,trackMarked13,trackMarked14,trackMarked15,trackMarked16,
    trackMarked17,trackMarked18,trackMarked19,trackMarked20,trackMarked21,trackMarked22,trackMarked23,trackMarked24,
    trackMarked25,trackMarked26,trackMarked27,trackMarked28,trackMarked29,trackMarked30,trackMarked31,trackMarked32;
    AudioFormatManager formatManager;
    
    AudioVisual audioVis;
    RecordingThumb recordingThumb1, recordingThumb2, recordingThumb3, recordingThumb4;
    recorda recorder;
    //LiveScrollingAudioDisplay liveAudioScroller;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
   
   
    TextButton playButton1, playButton2, playButton3, playButton4;
    TextButton stopButton1, stopButton2, stopButton3, stopButton4;
    TextButton recordingButton1, recordingButton2, recordingButton3, recordingButton4;
    TextButton stopRecordingButton1, stopRecordingButton2, stopRecordingButton3, stopRecordingButton4;
    
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
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
