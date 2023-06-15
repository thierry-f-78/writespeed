/*
 * Copyright 2010 Thierry FOURNIER
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License.
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PATTERN "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define BUFFER 8192

char buffer[BUFFER];

static long int now,prev,t0;
static long long int total=0, lastb=0;

void sig(int signum) {
   long int t = now-t0;
   now=time(NULL);
   //fprintf(stderr,"\r%lld octets en %d:%02d:%02d = %lld ko/s (%lld ko/s avg)       \r",total,
   //               t/3600,(t%3600)/60,t%60, ((total-lastb)/((now-prev)?:1))>>10, (total/(t?:1))>>10);
   fprintf(stderr,"%lld octets en %d:%02d:%02d = %lld ko/s ( %lld ko/s avg )           \n",total,
                  t/3600,(t%3600)/60,t%60, ((total-lastb)/((now-prev)?:1))>>10, (total/(t?:1))>>10);
   prev=now;  
   lastb=total;
   signal(signum, sig);
   alarm(1);
}

main() {
   int c;
   long int l=0;

   /* init buffer */
   while(l<BUFFER){
      memcpy(&buffer[l], PATTERN, (l+strlen(PATTERN)>=BUFFER?(BUFFER-1-l):strlen(PATTERN)));
      l += strlen(PATTERN);
      printf("%d\n", l);
   }

   t0=time(NULL);

   signal(SIGALRM, sig);
   alarm(1);

   while ((l=write(1,buffer,sizeof(buffer))) >0) {
        total += l;
   }
   fprintf(stderr,"\n");
   exit(0);
}
