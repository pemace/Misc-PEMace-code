#include <Arduino.h>
#include <math.h>
#include "synth.h"

namespace Synth
{
  WaveTableMonosynth::WaveTableMonosynth() :
  
    filter(SAMPLE_RATE_FLOAT,4),
    glideRamp(SAMPLE_RATE_FLOAT/MONOSYNTH_MODULATION_DOWNSAMPLE),
    modEnveloppe(SAMPLE_RATE_FLOAT/MONOSYNTH_MODULATION_DOWNSAMPLE),
    vibratoLfo(SAMPLE_RATE_FLOAT/MONOSYNTH_MODULATION_DOWNSAMPLE,sineTable),
    modLfo(SAMPLE_RATE_FLOAT/MONOSYNTH_MODULATION_DOWNSAMPLE,sineTable),
    overdrive(32)
    
  {
    pushed=0;
    noteCode=-1;
    baseFrequency=A_TUNING;
    downsample=10;
    modWheel=0.f;
    pitchBendValue=0.f;

    init();
  }

  void WaveTableMonosynth::init()
  {
    if(take())
    {
      clear();
      give();
    }

    setFloatParameter(GENERAL_VOLUME,0.7f);
    setIntParameter(OSC1_ON,1);
    setFloatParameter(OSC1_MIX,1.f);
    setFloatParameter(FILTER_CUTOFF,1.f);
    setFloatParameter(FILTER_Q,0.2);
  }
  
  void WaveTableMonosynth::setIntParameter(unsigned int parameter,int value)
  {
      intParameter[parameter]=value;
  
      switch(parameter)
      {
        case OSC1_SEMI:
          floatParameter[OSC1_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC1_SEMI])/12.f);
          break;
        case OSC1_WAVEFORM:
          switch(value)
          {
            case WaveTableWaveform::WAVEFORM_SINE:
              osc[0].setWaveTable(sineTable);
              break;
            case WaveTableWaveform::WAVEFORM_TRIANGLE:
              osc[0].setWaveTable(triangleTable);
              break;
            case WaveTableWaveform::WAVEFORM_SAW:
              osc[0].setWaveTable(sawTable);
              break;
            case WaveTableWaveform::WAVEFORM_SQUARE:
              osc[0].setWaveTable(squareTable);
              break;
          }
          break;
        case OSC2_SEMI:
          floatParameter[OSC2_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC2_SEMI])/12.f);
          break;
        case OSC2_WAVEFORM:
          switch(value)
          {
            case WaveTableWaveform::WAVEFORM_SINE:
              osc[1].setWaveTable(sineTable);
              break;
            case WaveTableWaveform::WAVEFORM_TRIANGLE:
              osc[1].setWaveTable(triangleTable);
              break;
            case WaveTableWaveform::WAVEFORM_SAW:
              osc[1].setWaveTable(sawTable);
              break;
            case WaveTableWaveform::WAVEFORM_SQUARE:
              osc[1].setWaveTable(squareTable);
              break;
          }
          break;
        case OSC3_SEMI:
          floatParameter[OSC3_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC3_SEMI])/12.f);
          break;
        case OSC3_WAVEFORM:
          switch(value)
          {
            case WaveTableWaveform::WAVEFORM_SINE:
              osc[2].setWaveTable(sineTable);
              break;
            case WaveTableWaveform::WAVEFORM_TRIANGLE:
              osc[2].setWaveTable(triangleTable);
              break;
            case WaveTableWaveform::WAVEFORM_SAW:
              osc[2].setWaveTable(sawTable);
              break;
            case WaveTableWaveform::WAVEFORM_SQUARE:
              osc[2].setWaveTable(squareTable);
              break;
          }
        default:
          break;
      }
  }

 
  void WaveTableMonosynth::setFloatParameter(unsigned int parameter,float value)
  {
      floatParameter[parameter]=value;
  
      switch(parameter)
      {
        case OSC1_TUNE:
          floatParameter[OSC1_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC1_SEMI])/12.f);
          break;
        case OSC2_TUNE:
          floatParameter[OSC2_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC2_SEMI])/12.f);
          break;
        case OSC3_TUNE:
          floatParameter[OSC3_FREQUENCY_MULTIPLICATOR]=pow(2,(value+intParameter[OSC3_SEMI])/12.f);
          break;
          
        case VOLUME_ENVELOPPE_ATTACK:
        case VOLUME_ENVELOPPE_DECAY:
        case VOLUME_ENVELOPPE_SUSTAIN:
        case VOLUME_ENVELOPPE_RELEASE:
          volumeEnveloppe.set(floatParameter[VOLUME_ENVELOPPE_ATTACK],
                                floatParameter[VOLUME_ENVELOPPE_DECAY],
                                floatParameter[VOLUME_ENVELOPPE_SUSTAIN],
                                floatParameter[VOLUME_ENVELOPPE_RELEASE]);
          break;
          
        case MOD_ENVELOPPE_ATTACK:
        case MOD_ENVELOPPE_DECAY:
        case MOD_ENVELOPPE_SUSTAIN:
        case MOD_ENVELOPPE_RELEASE:
          modEnveloppe.set(floatParameter[MOD_ENVELOPPE_ATTACK],
                                floatParameter[MOD_ENVELOPPE_DECAY],
                                floatParameter[MOD_ENVELOPPE_SUSTAIN],
                                floatParameter[MOD_ENVELOPPE_RELEASE]);
          break;
          
        case PITCHBEND_RANGE:
          {
            float constrained=constrain(value,1.0,12.0);
            pitchPowerTable.init(-constrained,constrained);
            break;
          }

        case LFO_RATE:
        case LFO_RATE_SLIDER:
        case VIBRATO_RATE:
        case VIBRATO_RATE_SLIDER:
        case VIBRATO_RATE_MODWHEEL:
        case SLIDER_VALUE:
          {
          float frequency=floatParameter[LFO_RATE] +
                          floatParameter[LFO_RATE_SLIDER] * floatParameter[SLIDER_VALUE] +
                          floatParameter[LFO_RATE_MODWHEEL] * modWheel;
                              
          modLfo.setFrequency( constrain(frequency, 0.25f, 20.f));

          frequency=floatParameter[VIBRATO_RATE] +
                    floatParameter[VIBRATO_RATE_SLIDER] * floatParameter[SLIDER_VALUE] +
                    floatParameter[VIBRATO_RATE_MODWHEEL] * modWheel;
                    
          vibratoLfo.setFrequency( constrain(frequency , 0.25f, 20.f));
          }
          break;
        case OVERDRIVE_GAIN:
          overdrive.setGain(value);
          break;
      }
  }

  
  int WaveTableMonosynth::getIntParameter(unsigned int parameter)
  {
    return intParameter[parameter];
  }
  float WaveTableMonosynth::getFloatParameter(unsigned int parameter)
  {
    return floatParameter[parameter];
  }

  float WaveTableMonosynth::computeNext()
  {
    float signal=0.f;
    
      // Tous les 10 échantillons, mettre à jour l'enveloppe et le lfo
      if(downsample==0)
      {
        // Calcul du lfo et de l'enveloppe
        float modEnvValue=modEnveloppe.computeNext();
        float lfoValue=modLfo.computeNext();

        // Calcul du vibrato
        float vibratoValue=vibratoLfo.computeNext();
        float vibratoMultiplicator = pitchPowerTable.compute( vibratoValue* (floatParameter[VIBRATO_AMPLITUDE]  +
                                                                floatParameter[VIBRATO_AMPLITUDE_SLIDER] * floatParameter[SLIDER_VALUE] +
                                                                floatParameter[VIBRATO_AMPLITUDE_MODWHEEL] * modWheel ));
        float pitchBendMultiplicator = pitchPowerTable.compute( floatParameter[PITCHBEND_RANGE] * pitchBendValue  );

        // Glide
        if(intParameter[GLIDE_ON])
          baseFrequency=glideRamp.computeNext();


        // Mise à jour des fréquences des oscillateurs
        osc[0].setFrequency( baseFrequency *
                                floatParameter[OSC1_FREQUENCY_MULTIPLICATOR] *
                                pitchBendMultiplicator *
                                vibratoMultiplicator *
                                pitchPowerTable.compute(floatParameter[OSC1_FREQUENCY_MODULATION_LFO] * lfoValue +
                                                        floatParameter[OSC1_FREQUENCY_MODULATION_ENVELOPPE] * modEnvValue ) );

        osc[1].setFrequency( baseFrequency *
                                floatParameter[OSC2_FREQUENCY_MULTIPLICATOR] *
                                pitchBendMultiplicator *
                                vibratoMultiplicator *
                                pitchPowerTable.compute(
                                                        floatParameter[OSC2_FREQUENCY_MODULATION_LFO] * lfoValue +
                                                        floatParameter[OSC2_FREQUENCY_MODULATION_ENVELOPPE] * modEnvValue) );

        osc[2].setFrequency( baseFrequency *
                                floatParameter[OSC3_FREQUENCY_MULTIPLICATOR] *
                                pitchBendMultiplicator *
                                vibratoMultiplicator *
                                pitchPowerTable.compute(
                                                        floatParameter[OSC3_FREQUENCY_MODULATION_LFO] * lfoValue +
                                                        floatParameter[OSC3_FREQUENCY_MODULATION_ENVELOPPE] * modEnvValue) );

        float filterCutoff=floatParameter[FILTER_CUTOFF] + 
                                      floatParameter[FILTER_CUTOFF_SLIDER] * floatParameter[SLIDER_VALUE] +
                                      floatParameter[FILTER_CUTOFF_MODULATION_LFO] * lfoValue +
                                      floatParameter[FILTER_CUTOFF_MODULATION_ENVELOPPE] * modEnvValue +
                                      floatParameter[FILTER_CUTOFF_MODULATION_MODWHEEL] * modWheel +
                                      floatParameter[FILTER_CUTOFF_MODULATION_MODWHEEL_LFO] * modWheel * lfoValue ;
        if(intParameter[FILTER_FOLLOW_ON])
          filterCutoff+=invLogFilter.compute(baseFrequency);
          
        filter.set(logFilter.compute( filterCutoff ),floatParameter[FILTER_Q] );
        
      }
      // mixage des différents oscillateurs
      if(intParameter[OSC1_ON])
        signal += floatParameter[OSC1_MIX] * osc[0].computeNext();
      if(intParameter[OSC2_ON])
        signal += floatParameter[OSC2_MIX] * osc[1].computeNext();
      if(intParameter[OSC3_ON])
        signal += floatParameter[OSC3_MIX] * osc[2].computeNext();
      
      // volume contrôlé par l'enveloppe
      signal *= volumeEnveloppe.computeNext();

      // filtrage
      if( intParameter[FILTER_ON] )
      {
        signal=filter.computeNext(signal);
      }
      

      // overdrive
      if( intParameter[OVERDRIVE_ON] )
      {
        signal=overdrive.compute(signal);
      }

      signal*=floatParameter[GENERAL_VOLUME];

      // avance le compteur de samples
      downsample=(downsample+1)%MONOSYNTH_MODULATION_DOWNSAMPLE;
      
    
    return signal;
  }

  void WaveTableMonosynth::noteOn(int channel,int noteCode,int velocity)
  {
      float noteFrequency=frequencyTable[noteCode];
      
      if( (!pushed) || !( intParameter[LEGATO_ON] || intParameter[GLIDE_ON] ) )
      {
        volumeEnveloppe.noteOn();
        modEnveloppe.noteOn();
      }

      this->noteCode=noteCode;
  
      if(intParameter[GLIDE_ON])
      {
        if(pushed)
          glideRamp.set(floatParameter[GLIDE_TIME],baseFrequency,noteFrequency);
        else
          glideRamp.set(floatParameter[GLIDE_TIME],noteFrequency,noteFrequency);
        glideRamp.noteOn();
      } else
      {
        baseFrequency=noteFrequency;
      }

      pushed=1;
      
  }
  void WaveTableMonosynth::noteOff(int channel,int noteCode,int velocity)
  {
      if(noteCode==this->noteCode)
      {
        volumeEnveloppe.noteOff();
        modEnveloppe.noteOff();
        pushed=0;
      }
  }
  void WaveTableMonosynth::controlChange(int channel,int ccCode,int value)
  {
      if(ccCode==1)
      {
        modWheel=value/127.f;

        float frequency=floatParameter[LFO_RATE] +
                          floatParameter[LFO_RATE_SLIDER] * floatParameter[SLIDER_VALUE] +
                          floatParameter[LFO_RATE_MODWHEEL] * modWheel;
                              
        modLfo.setFrequency( constrain(frequency, 0.25f, 20.f));

        frequency=floatParameter[VIBRATO_RATE] +
                  floatParameter[VIBRATO_RATE_SLIDER] * floatParameter[SLIDER_VALUE] +
                  floatParameter[VIBRATO_RATE_MODWHEEL] * modWheel;
                  
        vibratoLfo.setFrequency( constrain(frequency , 0.25f, 20.f));
      }
  }
  void WaveTableMonosynth::pitchBend(int channel,int value)
  {
      pitchBendValue = 2.f * (value-0x2000) / ((float) 0X3FFF);
  }
}
