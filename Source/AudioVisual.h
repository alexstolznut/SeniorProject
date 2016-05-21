/*
  ==============================================================================

    AudioVisual.h
    Created: 30 Apr 2016 1:46:12am
    Author:  Alex Stolzoff

  ==============================================================================
*/

#ifndef AUDIOVISUAL_H_INCLUDED
#define AUDIOVISUAL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AudioVisual    : public AudioVisualiserComponent,
                       public AudioIODeviceCallback
{
public:
    AudioVisual()   :   AudioVisualiserComponent(1)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        setSamplesPerBlock(256);
        setBufferSize(1024);

    }

    ~AudioVisual()
    {
    }
    
    void audioDeviceAboutToStart(AudioIODevice*) override
    {
        clear();
    }
    
    void audioDeviceStopped() override
    { 
        clear();
    }
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, 
                                int numberOfSamples) override
    {
        for (int i = 0; i < numberOfSamples; i++)
        { 
            float inputOfSample = 0;
            for (int chan = 0; chan < numInputChannels; ++chan)
                if (const float* inputChannel = inputChannelData[chan])
                    inputOfSample += inputChannel[i];
                    
            inputOfSample *= 10.0f;
            pushSample (&inputOfSample, 1);
        }
        
        for (int j = 0; j < numOutputChannels; ++j)
            if (float* outputChannel = outputChannelData[j])
                zeromem (outputChannel, sizeof (float) * (size_t) numberOfSamples);
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

           // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioVisual)
};


#endif  // AUDIOVISUAL_H_INCLUDED
