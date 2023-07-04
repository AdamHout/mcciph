/*
 * cciph.h
 *
 *  Created on: Jun 26, 2023
 *      Author: adam
 */

#ifndef MCCIPH_H_
#define MCCIPH_H_

//Top value of the printable portion of the ASCII table '~' (tilde)
#define ASCII_TOP 126
//Bottom value of the printable portion of the ASCII table ' ' (Space)
#define ASCII_BOT 32
//Value to wrap around the printable portion of the ASCII table
#define WRAP_VAL  95

//                      Function declarations

/*
 * Input:  Source and destination file pointers
 * Output: Count of characters processed
 */
unsigned int encode(FILE *, FILE *);
unsigned int decode(FILE *, FILE *);

#endif /* MCCIPH_H_ */
