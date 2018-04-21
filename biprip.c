/* FUER MEINEN LIEBEN FREUND MICHAEL KANNENBERG :-) */
/* Harry Basalamah 10.12.2017 */

#include <stdio.h>
#include <stdlib.h>

#define  MAXHITS         100

int main(int argc, char **argv) {
 unsigned char i;
 unsigned int filecount = 0, x, emfound;
 unsigned long int flen, posi[MAXHITS], count, count_two;
 char prefix[] = "kanne", suffix[] = ".bip", outfile[13];
 FILE *fileptr, *fileptr2;

 printf("\nKeyShot Scene File Ripper v1.03 by Harry Basalamah\n");
 printf("Written for Mr. Michael \"I've got no backup\" Kannenberg ;)\n");

 if (argc != 2) {
  fprintf(stderr, "Usage: %s filename\n", argv[0]);
  exit(1);
 }

 if ((fileptr = fopen(argv[1], "rb")) == NULL) {
  fprintf(stderr, "\nFATAL: File \"%s\" not found\n", argv[1]);
  exit(1);
 }

 printf("\nPre-Processing");
 fseek(fileptr, 0L, SEEK_END);  /* ans ende der datei springen */
 flen = ftell(fileptr);         /* dateilaenge bestimmen */
 rewind(fileptr);

 /* dateisignatur finden */
 for (count = 0; count < flen; count++) {
  i = fgetc(fileptr);
  if (i == '/') {
   i = fgetc(fileptr);
   if (i == '/' && count <= flen) {
    i = fgetc(fileptr);
    if (i == '-' && count <= flen) {
     i = fgetc(fileptr);
     if (i == '-' && count <= flen) {
      i = fgetc(fileptr);
      if (i == '-' && count <= flen) {
       i = fgetc(fileptr);
       if (i == 'l' && count <= flen) {
        i = fgetc(fileptr);
        if (i == 'u' && count <= flen) {
         i = fgetc(fileptr);
         if (i == 'x' && count <= flen) {
          i = fgetc(fileptr);
          if (i == 'i' && count <= flen) {
           i = fgetc(fileptr);
           if (i == 'o' && count <= flen) {
            i = fgetc(fileptr);
            if (i == 'n' && count <= flen) {
             /* start-rip offset merken */
             posi[filecount++] = ftell(fileptr) - 11;
             printf(".");
             fflush(stdout);
             emfound = 0;
             for (; count < flen; count++) {
             /* end marker finden (hoffentlich) */
              i = fgetc(fileptr);
              if (i == 0x0A && count <= flen) {
               i = fgetc(fileptr);
               if (i == 0x7D && count <= flen) {
                i = fgetc(fileptr);
                if (i == 0x0A && count <= flen) {
                 i = fgetc(fileptr);
                 if (i == 0x00 && count <= flen) {
                  i = fgetc(fileptr);
                  if (i == 0x00 && count <= flen) {
                  posi[filecount++] = ftell(fileptr) - 3;
                  emfound = 1;
                  break;
                  }
                 }
                }
               }
              }
             }
            if (!emfound)
             posi[filecount++] = flen - 1;
            }
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
 }

 if (filecount == 0) {
  fprintf(stderr, "\nSorry, no KeyShot files found. Exiting\n");
  exit(1);
 }

 printf("\nFound %d KeyShot file(s)\n", (filecount + 1) / 2);
 printf("\nExtracting...   ");
 for (x = 1; x <= filecount; x++) {
  sprintf(outfile,"%5s%02d%4s", prefix, x == 1 ? x : x + 1 - ((x + 1) / 2), suffix);
  if ((fileptr2 = fopen(outfile, "wb")) == NULL) {
   fprintf(stderr, "\nCould not write to output file! Bailing out\n");
   exit(1);
  }
  count = posi[x - 1];
  fseek(fileptr, count, SEEK_SET); /* an position zum rippen springen */
  printf("\b\b\b%03d", (x + 1) / 2);
  fflush(stdout);
  for ( ; count <= posi[x]; ++count) {
   i = fgetc(fileptr);         /* rip da shit! */
   fputc(i, fileptr2);
  }
  fclose(fileptr2);               /* geschriebene datei schliessen */
  x++;
 }
 fclose(fileptr);
 printf("\nFinished. Enjoy!\n");
 return 0;
}
