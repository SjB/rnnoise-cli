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
#include <stdlib.h>
#include <sndfile.h>
#include <rnnoise.h>

#define FRAME_SIZE 480

int main(int argc, char **argv) {
  int i;
  float x[FRAME_SIZE];
  SNDFILE *in, *out;
  SF_INFO sf_info;
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
 
    
  fprintf(stderr, "file info: channels %d\n", sf_info.channels);
  float* tmp = (float*)malloc(sizeof(float)*FRAME_SIZE*sf_info.channels);
  DenoiseState **st = (DenoiseState**)malloc(sizeof(DenoiseState*) * sf_info.channels);

  for (int c = 0; c < sf_info.channels; c++)
    st[c] = rnnoise_create();
  while (1) {
    // clear the buffer with zero
    for (i=0; i < FRAME_SIZE * sf_info.channels; i++) tmp[i]=0;

    sf_count_t r = sf_readf_float(in, tmp, FRAME_SIZE);

    for (int ch = 0; ch < sf_info.channels; ch++) {
      for (i=0;i<FRAME_SIZE;i++) {
	x[i] = tmp[sf_info.channels * i + ch] * UINT16_MAX;
      }

      rnnoise_process_frame(st[ch], x, x);

      for (i=0;i<FRAME_SIZE;i++) {
	tmp[sf_info.channels * i + ch] = x[i] / UINT16_MAX; 
      }
    }
    
    sf_writef_float(out, tmp, r);

    if (r != FRAME_SIZE) break;
  }
  free(tmp);
  for (int c = 0; c < sf_info.channels; c++)
    rnnoise_destroy(st[c]);
  
  free(st);
  sf_close(in);
  sf_close(out);
  return 0;
}
