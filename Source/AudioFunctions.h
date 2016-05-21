/*
  ==============================================================================

    AudioFunctions.h
    Created: 14 May 2016 11:17:28pm
    Author:  Alex Stolzoff

  ==============================================================================
*/

#ifndef AUDIOFUNCTIONS_H_INCLUDED
#define AUDIOFUNCTIONS_H_INCLUDED

class recordingTransportSource  :   public PositionableAudioSource,
                                    public ChangeBroadcaster
                                    
{
public:
    recordingTransportSource();
    
    ~recordingTransportSource();
    
    void setRecordingSource(PositionableRecordingSource* recordingSource, 
                            int readUpcomingBufferSize = 0,
                            TimeSliceThread* readUpcomingThread = nullptr,
                            double recordingSampleRate = 0.0,
                            int maxNumberOfChannels = 2);
                            
           
    void setPosition() const;
    
    double getCurrentPosition() const;
    
    double getLengthInSeconds() const;
    
    bool hasStreamFinished() const noexcept    {return inputStreamEOF;}
    
    void start();
    
    void stop();
    
    bool isPlaying() const noexcept {return playing;}
    
    void setGain(float newGain) noexcept;
    
    float getGain() const noexcept  {return gain;}
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    void releaseResources() override;
    
    void getNextAudioBlock(const AudioSourceChannelInfo&) override;
    
    void setNextReadPosition(int64 newPosition) override;
    
    int64 getNextReadPosition() const override;
    
    bool isLooping const override;
    
private:
    PositionableAudioSource* source;
    ResamplingAudioSource* resamplerSource;
    BufferingAudioSource* bufferingSource;
    PositionableAudioSource* positionableSource;
    AudioSource* masterSource;
    
    CriticalSection callbackLock;
    float volatile gain, lastGain;
    bool volatile playing, stopped;
    double sampleRate, sourceSampleRate;
    int blockSize, readAheadBufferSize;
    bool volatile isPrepared, inputStreamEOF;
    
    void releaseMasterResources();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordingTransportSource)
};
    
    
    



#endif  // AUDIOFUNCTIONS_H_INCLUDED
