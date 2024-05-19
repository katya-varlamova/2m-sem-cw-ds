#pragma once

#include "base_exception.h"

BADOON_EXCEPTION( LogicException, BaseException, "(Logic error)" )
BADOON_EXCEPTION(
  KafkaException,
  LogicException,
  "(Kafka error)"
)
