#pragma once
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <math.h> 
#include <direct.h> 

#include "api.h"
#include "process.h"
#include "generator.h"

#define URL_MAX_LENGTH 256
#define MAX_RETRIES 3
#define RETRY_DELAY 2 // seconds