# CS344 Program Four
## Overview
In this assignment, you will be creating five small programs that encrypt and decrypt information using a one-time pad-like system. I believe that you will find the topic quite fascinating: one of your challenges will be to pull yourself away from the stories of real-world espionage and tradecraft that have used the techniques you will be implementing.

These programs serve as a capstone to what you have been learning in this course, and will combine the multi-processing code you have been learning with socket-based inter-process communication. Your programs will also accessible from the command line using standard UNIX features like input/output redirection, and job control. Finally, you will write a short compilation script.

## Specifications
All execution, compiling, and testing of this program should ONLY be done in the bash prompt on our class server!

Use the following link as your primary reference on One-Time Pads (OTP):

http://en.wikipedia.org/wiki/One-time_pad (Links to an external site.)

The following definitions will be important:

`Plaintext` is the term for the information that you wish to encrypt and protect. It is human readable.

`Ciphertext` is the term for the plaintext after it has been encrypted by your programs. Ciphertext is not human-readable, and in fact cannot be cracked, if the OTP system is used correctly.

A `Key` is the random sequence of characters that will be used to convert Plaintext to Ciphertext, and back again. It must not be re-used, or else the encryption is in danger of being broken.

The following excerpt from this Wikipedia article was captured on 2/21/2015:

“Suppose Alice wishes to send the message "HELLO" to Bob. Assume two pads of paper containing identical random sequences of letters were somehow previously produced and securely issued to both. Alice chooses the appropriate unused page from the pad. The way to do this is normally arranged for in advance, as for instance 'use the 12th sheet on 1 May', or 'use the next available sheet for the next message'.

The material on the selected sheet is the key for this message. Each letter from the pad will be combined in a predetermined way with one letter of the message. (It is common, but not required, to assign each letter a numerical value, e.g., "A" is 0, "B" is 1, and so on.)

In this example, the technique is to combine the key and the message using modular addition. The numerical values of corresponding message and key letters are added together, modulo 26. So, if key material begins with "XMCKL" and the message is "HELLO", then the coding would be done as follows:

`H       E       L       L       O  message`
 `  7 (H)   4 (E)  11 (L)  11 (L)  14 (O) message`
`+ 23 (X)  12 (M)   2 (C)  10 (K)  11 (L) key`
`= 30      16      13      21      25     message + key`
`=  4 (E)  16 (Q)  13 (N)  21 (V)  25 (Z) message + key (mod 26)`
 `     E       Q       N       V       Z  → ciphertext`
 
 If a number is larger than 26, then the remainder, after subtraction of 26, is taken [as the result]. This simply means that if the computations "go past" Z, the sequence starts again at A.

The ciphertext to be sent to Bob is thus "EQNVZ". Bob uses the matching key page and the same process, but in reverse, to obtain the plaintext. Here the key is subtracted from the ciphertext, again using modular arithmetic:

` E       Q       N       V       Z  ciphertext`
`    4 (E)  16 (Q)  13 (N)  21 (V)  25 (Z) ciphertext`
`-  23 (X)  12 (M)   2 (C)  10 (K)  11 (L) key`
`= -19       4      11      11      14     ciphertext – key`
`=   7 (H)   4 (E)  11 (L)  11 (L)  14 (O) ciphertext – key (mod 26)`
`      H       E       L       L       O  → message`

