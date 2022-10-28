#include <Arduino.h>
#include "synth.h"

namespace Synth
{
  const float frequencyTable[]=
  {
  8.175798916f,
  8.661957218f,
  9.177023997f,
  9.722718241f,
  10.30086115f,
  10.91338223f,
  11.56232571f,
  12.24985737f,
  12.9782718f,
  13.75f,
  14.56761755f,
  15.43385316f,
  16.35159783f,
  17.32391444f,
  18.35404799f,
  19.44543648f,
  20.60172231f,
  21.82676446f,
  23.12465142f,
  24.49971475f,
  25.9565436f,
  27.5f,
  29.13523509f,
  30.86770633f,
  32.70319566f,
  34.64782887f,
  36.70809599f,
  38.89087297f,
  41.20344461f,
  43.65352893f,
  46.24930284f,
  48.9994295f,
  51.9130872f,
  55.f,
  58.27047019f,
  61.73541266f,
  65.40639133f,
  69.29565774f,
  73.41619198f,
  77.78174593f,
  82.40688923f,
  87.30705786f,
  92.49860568f,
  97.998859f,
  103.8261744f,
  110.f,
  116.5409404f,
  123.4708253f,
  130.8127827f,
  138.5913155f,
  146.832384f,
  155.5634919f,
  164.8137785f,
  174.6141157f,
  184.9972114f,
  195.997718f,
  207.6523488f,
  220.f,
  233.0818808f,
  246.9416506f,
  261.6255653f,
  277.182631f,
  293.6647679f,
  311.1269837f,
  329.6275569f,
  349.2282314f,
  369.9944227f,
  391.995436f,
  415.3046976f,
  440.f,
  466.1637615f,
  493.8833013f,
  523.2511306f,
  554.365262f,
  587.3295358f,
  622.2539674f,
  659.2551138f,
  698.4564629f,
  739.9888454f,
  783.990872f,
  830.6093952f,
  880.f,
  932.327523f,
  987.7666025f,
  1046.502261f,
  1108.730524f,
  1174.659072f,
  1244.507935f,
  1318.510228f,
  1396.912926f,
  1479.977691f,
  1567.981744f,
  1661.21879f,
  1760.f,
  1864.655046f,
  1975.533205f,
  2093.004522f,
  2217.461048f,
  2349.318143f,
  2489.01587f,
  2637.020455f,
  2793.825851f,
  2959.955382f,
  3135.963488f,
  3322.437581f,
  3520.f,
  3729.310092f,
  3951.06641f,
  4186.009045f,
  4434.922096f,
  4698.636287f,
  4978.03174f,
  5274.040911f,
  5587.651703f,
  5919.910763f,
  6271.926976f,
  6644.875161f,
  7040.f,
  7458.620184f,
  7902.13282f,
  8372.01809f,
  8869.844191f,
  9397.272573f,
  9956.063479f,
  10548.08182f,
  11175.30341f,
  11839.82153f,
  12543.85395f
  };
}
