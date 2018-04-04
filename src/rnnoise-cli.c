/* Copyright (c) 2017 Mozilla */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <sndfile.h>
#include <rnnoise.h>

#define FRAME_SIZE 480

int main(int argc, char **argv) {
  int i;
  float x[FRAME_SIZE];
  SNDFILE *in, *out;
  SF_INFO sf_info;
  DenoiseState *st;
  st = rnnoise_create();
  if (argc!=3) {
    fprintf(stderr, "usage: %s <noisy speech> <output denoised>\n", argv[0]);
    return 1;
  }

  in = sf_open(argv[1], SFM_READ, &sf_info);
  int err = sf_error(in);
  if (SF_ERR_NO_ERROR != err) {
    fprintf(stderr, "Error: %s\n", sf_strerror(in));
    return err;
  }
  
  out = sf_open(argv[2], SFM_WRITE, &sf_info);
  err = sf_error(out);
  if (SF_ERR_NO_ERROR != err) {
    fprintf(stderr, "Error: %s\n", sf_strerror(out));
    return err;
  }
 
  while (1) {
    short tmp[FRAME_SIZE];
    sf_count_t r = sf_readf_short(in, tmp, FRAME_SIZE);
    for (i=0;i<FRAME_SIZE;i++) {
      x[i] = tmp[i];
      //fprintf(stderr, "R: %ld\t: %d\n", r, tmp[i]);
    }
    rnnoise_process_frame(st, x, x);
    for (i=0;i<FRAME_SIZE;i++) {
      tmp[i] = x[i]; 
    }

    sf_count_t w = sf_writef_short(out, tmp, r);

    // for (i=0;i<FRAME_SIZE;i++) {
    //   fprintf(stderr, "W: %ld\t: %d\n", w, tmp[i]);
    // }
    
    if (r != FRAME_SIZE) break;
  }
  rnnoise_destroy(st);
  
  sf_close(in);
  sf_close(out);
  return 0;
}
