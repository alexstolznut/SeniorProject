/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

//getOutputChannelNames
//getInputChannelNames

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Display.h"
#include "AudioVisual.h"


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
            sampleRate(0), nextSampleNum (0), activeWriter(nullptr)
    
    {
        backgroundThread.startThread();
        
        trackOne.setSize(2, 0, false, true, false);
    }
    
    ~recorda()
    {
        stop();
    }
    
    
    void startAD()
    {
        deviceManager.addAudioCallback(this);
        deviceManager.initialise(2, 2, 0, true, "Headphones", nullptr);
    }
    void startRec()
    {
        
        startAD();
        trackOne.clear();
        playing=false;
        recording = true;
    }
    void closeAD()
    {
        deviceManager.closeAudioDevice();
        audioDeviceStopped();
        playing = false;
        recording=false;
        std::cout<<"recording: " << recording<<std::endl;
        std::cout<<"playing " << playing<<std::endl;
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
       std::cout<<"recording: " << recording<<std::endl;
       std::cout<<"playing " << playing<<std::endl;
       
       
       int oldSampleSize = trackOne.getNumSamples();
       int channelNumber = trackOne.getNumChannels();
if (recording == true)
{
    
       trackOne.setSize( trackOne.getNumChannels(),
                        oldSampleSize + numSamples,
                        true,
                        true,
                        false);
       trackOneOutput.setSize(trackOneOutput.getNumChannels(),
                              oldSampleSize + numSamples,
                              true,
                              true,
                              false);
       
       for(int j = 0; j < channelNumber; ++j){
           
           trackOne.copyFrom(j, oldSampleSize, inputChannelData[j], numSamples);
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

   
      for (int i = 0; i < numOutputChannels; ++i)
            {
               if (outputChannelData[i] != nullptr)
                {
                    for(int samples=0; samples<numSamples; ++samples)
                    {
                        if(position >= trackOne.getNumSamples())
                        {
                            
                            position = 0;
                        }
                        float sample = trackOne.getSample(i, samples+position);
                        
                        outputChannelData[i][samples] = sample;
                        //std::cout<<sample;
                    }
                
                }
          }
           position += numSamples;
    std::cout<<position<<std::endl;
     }
    }
int position;
    
private:
    AudioFormatManager formatManager;
    AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter;
    ScopedPointer<AudioFormatReaderSource> currentRecordingSource;
    double sampleRate;
    int64 nextSampleNum;
//       track[0];
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
      
        
        
        
        
        
        

        setSize (800, 600);
        
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
    
     void startRecording()
    {
        
        recorder.playing=false;
        recorder.recording=true;
        recorder.startRec();
        
    }
    void stopRecording()
    {
        recorder.stop();
        recorder.recording = false;
    }
    
   
    
    void buttonClicked (Button* button) override
    {
        if(button == &playButton1) playButtonClicked();
        if(button == &stopButton1) stopButtonClicked();
        if (button == &recordingButton1) startRecording();
        if (button == &stopRecordingButton1) stopRecording();
    }
    
 
    
  
    
    
            
    
    void playButtonClicked()
    {
        setAudioChannels(2,2);
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
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
