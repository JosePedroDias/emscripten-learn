// STANDARD
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <regex>
#include <ctime>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <thread>
#include <chrono>
#include <random>
#include <string.h>
#include <map>
#include <vector>

// EMSCRIPTEN
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/fetch.h>
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>

using namespace std;
using namespace emscripten;



int state = 0;

void incOne() {
  state = state + 1;
}

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}



EM_BOOL one_iter(double time, void* userData) {
  printf("one_iter state:%d\n", state);

  return EM_TRUE; // return true to keep the loop running
}

void one_iter2(void *userData) {
  printf("one_iter2 state:%d\n", state);
}



void downloadSucceeded(emscripten_fetch_t *fetch) {
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

  char *data = (char*)malloc(fetch->numBytes);
  strcpy(data, fetch->data);

  printf("body: \n%s \n", data);

  free(data);
  
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
  emscripten_fetch_close(fetch); // Also free data on failure.
}

void doFetch() {
  emscripten_fetch_attr_t attr;

  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess = downloadSucceeded;
  attr.onerror = downloadFailed;

  // https://matchmaker.krunker.io/generate-token
  // https://baconipsum.com/api/?type=meat-and-filler
  emscripten_fetch(&attr, "https://baconipsum.com/api/?type=meat-and-filler");
}



int main() {
  puts("hello from main");
  
  // every frame
  //emscripten_request_animation_frame_loop(one_iter, 0);

  // every second
  long timerId = emscripten_set_interval(one_iter2, 1000, (void*)0);



  // change a global var
  val::global("window").set("BATATAS", 42);

  // does eval
  //int num = emscripten_run_script_int("document.title = 'emscripten'; 3");
  //printf("got back %d\n", num);

  emscripten_run_script("thisAlreadyExisted()");

  

  puts("ahem");

  return 0;
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("lerp",    &lerp);
  emscripten::function("incOne",  &incOne);
  emscripten::function("doFetch", &doFetch);
}
// this exposes Module.lerp(1,5,0.5)
