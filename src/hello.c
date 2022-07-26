#include <string.h>
#include <stdio.h> // printf, puts, etc.
#include <stdlib.h> // malloc

#include <emscripten.h>
#include <emscripten/html5.h> // https://emscripten.org/docs/api_reference/html5.h.html
#include <emscripten/fetch.h> // https://emscripten.org/docs/api_reference/fetch.html#fetch-api



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

void* one_iter2(double time, void* userData) {
  printf("one_iter2 state:%d\n", state);

  return userData;
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
  long timerId = emscripten_set_interval(one_iter2, 1000, 0);

  // does eval
  //int num = emscripten_run_script_int("document.title = 'emscripten'; 3");
  //printf("got back %d\n", num);

  emscripten_run_script("thisAlreadyExisted()");

  return 0;
}
