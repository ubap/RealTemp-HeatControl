#pragma once

#include <Windows.h>
#include "process.h"

// powyzej blad odczytu
#define MAX_TEMP 150
#define MIN_TEMP 0
// adresy
#define CORE_0 (LPCVOID)0x0018FE7C
#define CORE_1 (LPCVOID)(0x0018FE7C+4)

bool ReadTemperature(unsigned int *temperature);