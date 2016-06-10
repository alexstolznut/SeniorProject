/*
  ==============================================================================

    MasterButton.h
    Created: 9 Jun 2016 6:23:40pm
    Author:  Alex Stolzoff

  ==============================================================================
*/

#ifndef MASTERBUTTON_H_INCLUDED
#define MASTERBUTTON_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
//#include "MeasureButton.h"

class MasterButton   : public Component
                      // public Button::Listener
{
public:
   MasterButton()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        setSize(450,375);
       
            setOpaque(false);
            addAndMakeVisible(button1);
            button1.setColour(TextButton::buttonColourId, Colours::white);
        
        
       
        //button.addListener(this);
    
       
    }

    ~MasterButton()
    {
        //button.removeListener(this);
    }

    void paint (Graphics& g) override
    {
     
     
       g.setColour(Colours::green); 
       Path play;
       play.addTriangle(193,235,326,180,193,125);
       g.fillPath(play);
        
    }

    void resized() override
    {
        
        button1.setBounds(100,100,300,150);
     
        
    }
		

private:
TextButton button1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterButton)
};









#endif  // MASTERBUTTON_H_INCLUDED
