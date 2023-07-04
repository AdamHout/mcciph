/*
 * main.c
 *
 * SYNOPSIS:
 * mcciph [ARGUMENT] PATH/FileName
 *
 *    Valid arguments are -e for encode and -d for decode
 *
 * DESCRIPTION:
 * mcciph is a modified Caesar cipher. It accepts a text file specified by the user as input and produces
 * an encoded or decoded file as its output. Unlike a traditional Caesar cipher, mcciph uses the shift value,
 * or key, changes from character to character as described in cciph.c. This cipher also uses the entire
 * printable range of ASCII values.
 *
 *------------------------------------------------------------------------------------------------------
 * Date       Author             Description
 *------------------------------------------------------------------------------------------------------
 * 06/30/23   Adam Hout          Original source
 * -----------------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mcciph.h"

#define NEW_EXT_LEN 3                                                     //".dc" or ".ec"

int cleanup(FILE *, FILE *, char *, char *);

//---------------------------------------------------
// Begin mainline processing
//---------------------------------------------------
int main(int argc, char *argv[])
{
   FILE *pSrc = 0, *pDst = 0;
   char opt;
   char *pDstPath = 0;
   char *pExt, *pParse;
   int  pathLen = 0;
   int  extLen = 0;
   int  totLen;
   unsigned int chrCnt;


   //---------------------------------------------------
   //Process/validate the input
   //---------------------------------------------------
   //Ensure proper number of arguments
   if (argc != 3){
      fprintf(stderr, "Usage: %s -[d|e] </path/file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   //Validate option argument; '-e'ncode or '-d'ecode
   opt = getopt (argc, argv, "de:");

   if (opt != 'd' && opt != 'e'){
      fprintf(stderr, "Valid options are -d and -e\n");
      exit(EXIT_FAILURE);
   }

   //Derive the extension length (if any), and the path / file name length
   totLen = strlen(argv[2]);
   pExt = strrchr(argv[2], '.');

   if (pExt){
      pParse = pExt;
      while (*pParse++)
         extLen++;
   }

   pathLen = totLen - extLen;

   //-Only encode unencoded files
   //-Only decode previously encoded files
   if (extLen){
      if (opt == 'e' && strcmp(pExt, ".ec") == 0){
         fprintf(stderr, "File is already encoded\n");
         exit(EXIT_FAILURE);
      }
      else if (opt == 'd' && strcmp(pExt, ".dc") == 0){
         fprintf(stderr, "File is already decoded\n");
         exit(EXIT_FAILURE);
      }
   }

   //---------------------------------------------------
   //Open files and allocate memory
   //---------------------------------------------------
   //Open the source file
   if ((pSrc = fopen(argv[2], "r")) == NULL){
      fprintf(stderr, "Unable to open input file: %s\n", argv[2]);
      exit(EXIT_FAILURE);
   }

   //Allocate memory for the destination file path/name
   pDstPath = (char *) malloc(pathLen + NEW_EXT_LEN);

   if (pDstPath == NULL){
      cleanup(pSrc, pDst, pDstPath, argv[2]);
   	fprintf(stderr, "Memory allocation error\n");
   	exit(EXIT_FAILURE);
   }

   //Build the destination path / file name / extension
   memcpy(pDstPath, argv[2], pathLen);

   if (opt == 'd')
   	memcpy(pDstPath + pathLen, ".dc\0", 4);
   else
   	memcpy(pDstPath + pathLen, ".ec\0", 4);

   //Open the destination file
   if ((pDst = fopen(pDstPath, "w")) == NULL){
      cleanup(pSrc, pDst, pDstPath, argv[2]);
   	fprintf(stderr, "Unable to open output file: %s\n", pDstPath);
   	exit(EXIT_FAILURE);
   }

   //---------------------------------------------------
   //Execute the cipher
   //---------------------------------------------------
   if (opt == 'd')
      chrCnt = decode(pSrc, pDst);
   else
      chrCnt = encode(pSrc, pDst);

   if (chrCnt == 0){
      cleanup(pSrc, pDst, pDstPath, argv[2]);
      fprintf(stderr, "Source file: %s was empty. Nothing to process\n", argv[2]);
      exit(EXIT_FAILURE);
   }
   else{
      printf("Process complete: %u characters %s\n", chrCnt, opt == 'd' ? "decoded" : "encoded");
      printf("Output file: %s contains the %s\n", pDstPath, opt == 'd' ? "decoded text" : "encoded text");
   }

   //Clean up and exit
   cleanup(pSrc, pDst, pDstPath, argv[2]);
	return 0;
}

/*
 *         FUNCTIONS
 */

//Closes files and release heap
int cleanup(FILE *pSrc, FILE *pDst, char *pDstPath, char *arg){

   int rtnInd = 0;

   if (pDstPath)
      free(pDstPath);

   if (pSrc){
      if (fclose(pSrc) != 0){
         fprintf(stderr, "Unable to close source file: %s\n", arg);
         rtnInd++;
      }
   }

   if (pDst){
      if (fclose(pDst) != 0){
         fprintf(stderr, "Unable to close output file: %s\n", pDstPath);
         rtnInd++;
      }
   }

   return rtnInd;
}


