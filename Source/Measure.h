/*
  ==============================================================================

    Measure.h
    Created: 24 May 2016 8:15:08pm
    Author:  Alex Stolzoff

  ==============================================================================
*/

#ifndef MEASURE_H_INCLUDED
#define MEASURE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "MeasureButton.h"

class Measure    : public Component
                   //public Button::Listener
{
public:
    Measure()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        setSize(450,375);
       // int Number;
       // for (int i = 1; i<=20;++i)
       // {
       //     Number = i;
       //     String(Number);
       //     String button = "Button"+String(Number);
       // }
        for(int i=0; i<32; i++)
        {
            addAndMakeVisible(button[i]);
            button[i].setColour(TextButton::buttonColourId, Colours::blue);
        }
        
            addAndMakeVisible(button1);
            button1.setColour(TextButton::buttonColourId, Colours::blue); 
            
            addAndMakeVisible(button2);
            button2.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button3);
            button3.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button4);
            button4.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button5);
            button5.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button6);
            button6.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button7);
            button7.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button8);
            button8.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button9);
            button9.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button10);
            button10.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button11);
            button11.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button12);
            button12.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button13);
            button13.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button14);
            button14.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button15);
            button15.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button16);
            button16.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button17);
            button17.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button18);
            button18.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button19);
            button19.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button20);
            button20.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button21);
            button21.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button22);
            button22.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button22);
            button22.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button23);
            button23.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button24);
            button24.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button25);
            button25.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button26);
            button26.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button27);
            button27.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button28);
            button28.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button29);
            button29.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button30);
            button30.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button31);
            button31.setColour(TextButton::buttonColourId, Colours::blue);
            
            addAndMakeVisible(button32);
            button32.setColour(TextButton::buttonColourId, Colours::blue);
            
        
        
       
        //button.addListener(this);
    
       
    }

    ~Measure()
    {
        //button.removeListener(this);
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
        g.fillRect(0, 0,450, 375);
        g.setColour(Colours::white);
        g.drawLine(0,y,450,y, 10);
        g.drawLine(0, y*2,450, y*2, 10);
        g.drawLine(0, y*3,450, y*3, 10);
        g.drawLine(0, y*4,450, y*4, 10);
        
        //Sound One
       // g.setColour(Colours::blue);
        //g.drawEllipse(50, 73, 5, 5, 20);
        //g.drawEllipse(100, 73, 5, 5, 20);
        //g.drawEllipse(150, 73, 5, 5, 20);
        //g.drawEllipse(200, 73, 5, 5, 20);
        //g.drawEllipse(250, 73, 5, 5, 20);
        //g.drawEllipse(300, 73, 5, 5, 20);
        //g.drawEllipse(350, 73, 5, 5, 20);
        
        //Sound Two
        //g.drawEllipse(50, 147, 5, 5, 20);
        //g.drawEllipse(100, 147, 5, 5, 20);
        //g.drawEllipse(150, 147, 5, 5, 20);
        //g.drawEllipse(200, 147, 5, 5, 20);
        //g.drawEllipse(250, 147, 5, 5, 20);
        //g.drawEllipse(300, 147, 5, 5, 20);
        //g.drawEllipse(350, 147, 5, 5, 20);
        
        //Sound Three
        //g.drawEllipse(50, 222, 5, 5, 20);
        //g.drawEllipse(100, 222, 5, 5, 20);
        //g.drawEllipse(150, 222, 5, 5, 20);
        //g.drawEllipse(200, 222, 5, 5, 20);
        //g.drawEllipse(250, 222, 5, 5, 20);
        //g.drawEllipse(300, 222, 5, 5, 20);
        //g.drawEllipse(350, 222, 5, 5, 20);
        
        //Sound Four
        //g.drawEllipse(50, 298, 5, 5, 20);
        //g.drawEllipse(100, 298, 5, 5, 20);
        //g.drawEllipse(150, 298, 5, 5, 20);
        //g.drawEllipse(200, 298, 5, 5, 20);
        //g.drawEllipse(250, 298, 5, 5, 20);
        //g.drawEllipse(300, 298, 5, 5, 20);
        //g.drawEllipse(350, 298, 5, 5, 20);
         
       
        
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
       
        //Sound One
        button1.setBounds(50,70,10,10);
        button2.setBounds(100,70,10,10);
        button3.setBounds(150,70,10,10);
        button4.setBounds(200,70,10,10);
        button5.setBounds(250,70,10,10);
        button6.setBounds(300,70,10,10);
        button7.setBounds(350,70,10,10);
        button8.setBounds(400,70,10,10);
        
        //Sound Two
        button9.setBounds(50,145,10,10);
        button10.setBounds(100,145,10,10);
        button11.setBounds(150,145,10,10);
        button12.setBounds(200,145,10,10);
        button13.setBounds(250,145,10,10);
        button14.setBounds(300,145,10,10);
        button15.setBounds(350,145,10,10);
        button16.setBounds(400,145,10,10);
        
        //Sound Three
        button17.setBounds(50,220,10,10);
        button18.setBounds(100,220,10,10);
        button19.setBounds(150,220,10,10);
        button20.setBounds(200,220,10,10);
        button21.setBounds(250,220,10,10);
        button22.setBounds(300,220,10,10);
        button23.setBounds(350,220,10,10);
        button24.setBounds(400,220,10,10);
        
        //Sound Four
        
        button25.setBounds(50,295,10,10);
        button26.setBounds(100,295,10,10);
        button27.setBounds(150,295,10,10);
        button28.setBounds(200,295,10,10);
        button29.setBounds(250,295,10,10);
        button30.setBounds(300,295,10,10);
        button31.setBounds(350,295,10,10);
        button32.setBounds(400,295,10,10);
        
    }
//#define NUMBEROFMEASUREBUTTONS 32
//    MeasureButton measureButton[32];
    TextButton
    
//    void buttonwasPressed(buttonthatwaspreseed* button)
//    {
//        for (int i = 0; i < NUMBEROFMEASUREBUTTONS; ++i)
//        {
//            if (buttonthatwaspressed == measure.button[i])
//            {
//                
//            }
//        }
//    }
button1, button2, button3, button4, button5, button6, button7, button8,
button9, button10, button11, button12, button13, button14, button15, button16, 
button17, button18, button19, button20,button21, button22, button23, button24,
button25, button26, button27, button28, button29, button30, button31, button32;

    TextButton button[32];

private:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Measure)
};





#endif  // MEASURE_H_INCLUDED
