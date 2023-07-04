/*
 * cciph.c
 *
 * The supplied source file is encoded or decoded using the entire printable portion of the ASCII table,
 * i.e. 0x20 (space) - 0x7E (tilde). It employs a modified version of the Caesar cipher in which the
 * shift value (key) applied to a given character is derived from the previous character by extracting its
 * low order nibble. This results in a new key that can range from 0x00 - 0x0F or 0 - 15 decimal. Thus a
 * character can pass through the cipher without encoding. The encoded character if wrapped when it
 * exceeds the bounds of the printable ASCII range.
 *
 * This process is illustrated in the following example:
 *
 * Characters to encode: Test - 0x54657374
 *
 * 1. An initial key of 3 is applied to the first character   'T': 0x54 + 0x03 = 0x57 = 'W'
 * 2. The low order nibble of 'W' (0x07) becomes the new key
 * 3. The new key is applied to the next character to encode  'e': 0x65 + 0x07 = 0x6C = 'l'
 * 4. The low order nibble of 'l' (0x0C) becomes the new key
 * 5. The new key is applied to the next character to encode  's': 0x73 + 0x0C = 0x7F = out of range
 * 6. This value is wrapped around to the beginning of the
 *    the printable ASCII range:                                   0x7F - 0x5F = 0x20 = ' ' (space)
 * 7. The low order nibble of ' ' (0x00) becomes the new key
 * 8. The key is applied to the final character to encode     't': 0x74 + 0x00 = 0x74 = 't'
 * 9. The resulting output is: wl t = 0x576C2074
 * 10. The same logic is used in reverse to decode characters
 *
 * The above method is executed on any printable character
 *
 * *------------------------------------------------------------------------------------------------------
 * Date       Author             Description
 *------------------------------------------------------------------------------------------------------
 * 06/30/23   Adam Hout          Original source
 * -----------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include "mcciph.h"

unsigned int encode(FILE *pSrc, FILE *pDst){

   short srcChr;
	short key = 3;
	unsigned int chrCnt = 0;

	//Loop through the source file
	while ((srcChr = fgetc(pSrc)) != EOF){
	   if (srcChr != '\n'){                                                //Maintain carriage returns
	      chrCnt++;
	      srcChr = srcChr + key;
	      if (srcChr > ASCII_TOP)                                          //Need to wrap?
	         srcChr -= WRAP_VAL;                                           //yep..
	      //Low order nibble becomes the new key
	      key = srcChr & 0x000F;
	   }
	   //Write the encoded character to the output file
		fputc(srcChr, pDst);
	}
	return chrCnt;
}

unsigned int decode(FILE *pSrc, FILE *pDst){

   short srcChr;
   short shiftVal = 3;
   short newKey;
   unsigned int chrCnt = 0;

   //Loop through the source file
   while ((srcChr = fgetc(pSrc)) != EOF){
      if (srcChr != '\n'){
         chrCnt++;
         newKey = srcChr & 0x000F;
         srcChr = (srcChr - shiftVal);
         shiftVal = newKey;
         if (srcChr < ASCII_BOT)                                          //Wrap in reverse when needed
            srcChr += WRAP_VAL;
      }

      fputc(srcChr, pDst);
   }
   return chrCnt;
}
