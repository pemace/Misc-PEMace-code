#include <Arduino.h>
#include "synth.h"

namespace Synth
{
  Mutexable::Mutexable()
  {
    mutex=xSemaphoreCreateBinary();
    delay = pdMS_TO_TICKS(4);
    give();
  }
  boolean Mutexable::take()
  {
    return xSemaphoreTake( mutex, delay ) == pdTRUE;
  }
  void Mutexable::give()
  {
    xSemaphoreGive( mutex );
  }
}
