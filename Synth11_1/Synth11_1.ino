/*
  -----------------------------------------------
  Monosynth, idéal pour basse ou lead
  -----------------------------------------------
  
  3 oscillateurs réglables en dents de scie,triangle,sinus ou carré avec 1 enveloppe commune

  Génération du son à 44.1kHz, en raison d'un système de sous-échantillonage du reglage du filtre et de hauteur de note (4kHz)
  
  Filtre modulable par lfo, enveloppe ou avec la modwheel
  
  vibrato et pitchbends possibles, vibrato sur le même lfo que le filtre et maximum 2 demi-tons de chaque côté

  Répartition par coeur :
    - Coeur 0 : Gestion du midi et à l'avenir de l'affichage
    - Coeur 1 : Gestion du son, envoi des trames par i2s
  
  à l'avenir, configuration avec un écran

  pour la configuration en temps réel, penser à utiliser le mutex pour éviter que ça s'entremêle avec la génération du son
  if(synth.take())
  {
    [ configuration du synthé ici ]
    synth.give();
  }
*/

#include <driver/i2s.h>
#include "synth.h"
using namespace Synth;

#define MIDI_RXD1 5
#define MIDI_TXD1 18

i2s_port_t i2s_num; // Port i2s, par défaut sur port 0
int16_t bufferi2s[2]; // Buffer droite/gauche

WaveTableMonosynth synth;
MidiDecoder midi(Serial1,synth);
WaveTableOscillator osc(SAMPLE_RATE_FLOAT,sineTable);

#define MIDI_TASK_PERIOD 20
unsigned int task;

// Configuration synthé -------------------------------------------------------------------------
// voir "synth_mono.h" pour la documentation
void configureSynth()
{
  synth.setFloatParameter(GENERAL_VOLUME,0.5f);

  synth.setIntParameter(OSC1_ON,1);
  synth.setIntParameter(OSC1_WAVEFORM,WAVEFORM_SQUARE);
  synth.setFloatParameter(OSC1_MIX,1.f);
  synth.setFloatParameter(OSC1_TUNE,0.f);

  synth.setIntParameter(FILTER_ON,1);
  synth.setFloatParameter(VOLUME_ENVELOPPE_ATTACK,0.01f);
  synth.setFloatParameter(VOLUME_ENVELOPPE_DECAY,2.f);
  synth.setFloatParameter(VOLUME_ENVELOPPE_SUSTAIN,0.f);
  synth.setFloatParameter(VOLUME_ENVELOPPE_RELEASE,0.05f);

  synth.setFloatParameter(FILTER_CUTOFF,0.f);
  synth.setFloatParameter(FILTER_CUTOFF_MODULATION_ENVELOPPE,1.f);
  synth.setFloatParameter(FILTER_Q,0.1);

  synth.setFloatParameter(MOD_ENVELOPPE_ATTACK,0.05f);
  synth.setFloatParameter(MOD_ENVELOPPE_DECAY,0.1f);
  synth.setFloatParameter(MOD_ENVELOPPE_SUSTAIN,0.5f);
  synth.setFloatParameter(MOD_ENVELOPPE_RELEASE,0.1f);
  
  synth.setIntParameter(LEGATO_ON,1);
  
  synth.setFloatParameter(VIBRATO_RATE,6.f);
  synth.setFloatParameter(VIBRATO_AMPLITUDE,0.f);
  synth.setFloatParameter(VIBRATO_AMPLITUDE_MODWHEEL,2.f);

  synth.setFloatParameter(PITCHBEND_RANGE,2.f);
}


void setup() {
  task=0;
  Serial.begin(9600);
  Serial1.begin(31250, SERIAL_8N1, MIDI_RXD1, MIDI_TXD1);

  configureSynth();

  // Configuration i2s - copier/collé -------------------------------------------------------------------------
  i2s_num = (i2s_port_t) 0; // i2s port number
  i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
     .sample_rate = SAMPLE_RATE,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64   //Interrupt level 1
    };
    
  i2s_pin_config_t pin_config = {
    .bck_io_num = 26, //this is BCK pin
    .ws_io_num = 25, // this is LRCK pin
    .data_out_num = 22, // this is DATA output pin
    .data_in_num = -1   //Not used
  };

  i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)i2s_num, &pin_config);

  i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)i2s_num, &pin_config);
}


void loop() {
  float signal;
  int16_t signalEntier;

  if(task==0) midi.process();
  
  signal=synth.computeNext(); //Appel au synthé pour générer le son, avec un clipper
  signal=constrain(signal,-1.f,1.f);
  
  signalEntier=signal*32767.f; // Normalisation du signal pour entier de 16bits
  bufferi2s[0]=signalEntier; // Envoyer le signal à droite et à gauche
  bufferi2s[1]=signalEntier;

  size_t octetsEcrits;
  i2s_write((i2s_port_t) i2s_num, (void*) bufferi2s, sizeof(uint16_t)*2, &octetsEcrits, pdMS_TO_TICKS(4));
  
  task=(task+1)%MIDI_TASK_PERIOD;
}
