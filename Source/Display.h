/*
  ==============================================================================

    Display.h
    Created: 29 Apr 2016 7:22:07pm
    Author:  Alex Stolzoff

  ==============================================================================
*/

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Display    : public Component
                   
                   
{
public:
    Display()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        setSize(400,100);
        addAndMakeVisible(volumeSlider);
        addAndMakeVisible(button);
        button.setColour(TextButton::buttonColourId, Colours::red);
    }

    ~Display()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        int y = 75;
        int yCir = 72;
        g.setColour(Colours::black);
        g.fillRect(0, 0,400, 376);
        g.setColour(Colours::white);
        g.drawLine(0,y,400,y, 10);
        g.drawLine(0, y*2,400, y*2, 10);
        g.drawLine(0, y*3,400, y*3, 10);
        g.drawLine(0, y*4,400, y*4, 10);
        
        
        
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
         volumeSlider.setBounds(100,400,getWidth()-20,20);
         button.setBounds(10,10,20,20);
         button.getToggleStateValue();
         if(button.isDown() == true)
        {
            button.setColour(TextButton::buttonColourId, Colours::blue);
        }

    }

private:
Slider volumeSlider;
ToggleButton button;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Display)
};


#endif  // DISPLAY_H_INCLUDED
