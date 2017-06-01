/*
 * Created by Petr Šopf
 * xsopfp00@vutbr.cz
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Function:  isInAscii
 * --------------------
 * Checks if char is in ASCII table
 *
 *   n: char's ASCII position
 */
bool isInAscii(int n) {
    if (n >= 0 && n <= 127) {
        return true;
    }

    return false;
}

/*
 * Function:  asciiNumber
 * --------------------
 * Returns DEC number
 *
 *   n: Position of char in ASCII table
 */
int asciiNumber(char n) {
    return (int) n - 48;
}

/*
 * Function:  hexToDec
 * --------------------
 * Returns DEC value from HEX char
 *
 *   buffer: char with HEX value
 */
int hexToDec(int buffer) {
    char b = (char) tolower(buffer);
    char c[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    for (int i = 0; i <= 5; ++i) {
        if (b == c[i]) {
            return 10 + i;
        }
    }

    return asciiNumber(b);
}

/*
 * Function:  isInt
 * --------------------
 * Checks if given char represents number
 *
 *   c: char
 */
bool isInt(char c) {
    int i = (int) c;

    if (i >= 48 && i <= 57) {
        return true;
    }

    return false;
}

/*
 * Function:  charsToInt
 * --------------------
 * Converts string to integer
 *
 *   s: array of chars
 */
int charsToInt(char s[]) {
    char c;
    bool done = false;
    int n = 0;

    for (int i = 0; !done; ++i) {
        c = s[i];

        if (c == 0) {
            done = true;
        } else {
            if (isInt(c)) {
                n *= 10;
                n += asciiNumber(c);
            } else {
                return -1;
            }
        }
    }
    return n;
}

/*
 * Function:  textDump
 * --------------------
 * Prints description of input data. Every line describes 16 bytes of input data.
 *
 *   M: Number of input data bytes to be ignored.
 *   N: Number of input data bytes to be processed.
 */
void textDump(int M, int N) {
    bool done = false;
    bool firstByte = true;
    int buffer[16];
    int z = 0 + M;

    while (M > 0) {
        if (getchar() == EOF) {
            return;
        }
        --M;
    }

    for (int i = 0; i <= 15; ++i) {
        if (N != -1) {
            if (N < 1) {
                done = true;
                if (i == 0) {
                    return;
                }
            }
            --N;
        }

        if (!done) {
            buffer[i] = getchar();

            if (buffer[i] == EOF) {
                done = true;
                if (i == 0) {
                    return;
                }
            }
        }
        if (!done) {
            if (firstByte) {
                printf("%08x  ", z);
                firstByte = false;
            }

            printf("%02x ", buffer[i]);
        } else {
            printf("   ");
            buffer[i] = -1;
        }

        if (i == 7) {
            printf(" ");
        }

        if (i == 15) {
            printf(" |");

            for (int x = 0; x <= 15; ++x) {
                if (buffer[x] == -1) {
                    printf(" ");
                } else if (isprint(buffer[x]) && isInAscii(buffer[x])) {
                    printf("%c", buffer[x]);
                } else {
                    printf(".");
                }
            }
            printf("|\n");

            if (!done) {
                i = -1;
                firstByte = true;
            }
        }

        ++z;
    }
}

/*
 * Function:  printSequences
 * --------------------
 * Finds text strings in input and prints them.
 *
 *   N: Minimal length of strings to print
 */
void printSequences(int N) {
    bool done = false;
    bool printSequence = false;
    int buffer[N];
    int i = 0;

    while (!done) {
        int c = getchar();

        if (c != EOF) {
            if (isblank(c) || isprint(c)) {
                buffer[i] = c;

                if (i == (N - 1)) {
                    for (int x = 0; x <= i; ++x) {
                        printf("%c", buffer[x]);
                    }

                    i = 0;
                    printSequence = true;
                } else if (printSequence) {
                    printf("%c", buffer[i]);
                } else {
                    ++i;
                }
            } else {
                if (printSequence) {
                    printf("\n");
                }
                printSequence = false;
                i = 0;
            }
        } else if (printSequence) {
            printf("\n");
            printSequence = false;
        } else {
            done = true;
        }
    }
}

/*
 * Function:  hexToText
 * --------------------
 * Converts HEX data input bytes to text
 */
int hexToText() {
    int dec = 0;
    bool done = false;
    bool firstDone = false;
    bool idOdd = false;

    while (!done) {
        int buffer = getchar();
        if (buffer == EOF) {
            if (idOdd) {
                dec /= 16;
                if (dec >= 0 && dec <= 15) {
                    char c = (char) dec;
                    printf("%c", c);
                } else {
                    fprintf(stderr, "ERROR: If number of chars is odd, last char must have DEC value 0-15!\n");
                    return EXIT_FAILURE;
                }
            }
            done = true;
        } else {
            if (!isblank(buffer) && isprint(buffer)) {
                int n = hexToDec(buffer);
                if (!(n >= 0 && n <= 15)) {
                    fprintf(stderr, "ERROR: Input must contain HEX digits only!\n");
                    return EXIT_FAILURE;
                }

                if (!firstDone) {
                    dec = 16 * n;
                    firstDone = true;
                    idOdd = true;
                } else {
                    dec += n;
                    char c = (char) dec;
                    printf("%c", c);
                    firstDone = false;
                    idOdd = false;
                }
            }
        }
    }

    return 0;
}

/*
 * Function:  textToHex
 * --------------------
 * Converts input data to HEX
 */
void textToHex() {
    bool done = false;

    while (!done) {
        int buffer = getchar();
        if (buffer == EOF) {
            done = true;
            printf("\n");
        } else {
            printf("%02x", buffer);
        }
    }
}

bool equals(char s1[], char s2[]) {
    bool equals = true;
    bool done = false;

    for (int i = 0; !done; ++i) {
        if (s1[i] != s2[i]) {
            equals = false;
            done = true;
        }

        if (s1[i] == 0) {
            done = true;
        }
    }

    return equals;
}

/*
 * Function:  printHelp
 * --------------------
 * Prints help
 */
void printHelp() {
    printf("------ HELP ------\n"
                   "Created by Petr Sopf (xsopfp00)\n"
                   "--- PROGRAM SYNTAX ---\n"
                   "./proj1 [-s M] [-n N] - Prints rows, every row describes 16 chars of input. \n- [-s M] - Defines the address to start\n- [-n N] - Defines number of chars to process.\n"
                   "./proj1 -x - Converts input data to HEX.\n"
                   "./proj1 -S N - Prints text sequences longer then N.\n"
                   "./proj1 -r - Converts HEX input data to text. \n");
}

/*
 * Function:  main
 * --------------------
 * Main function of program. Processes given arguments.
 */
int main(int argc, char *argv[]) {
    if (argc == 2 && equals(argv[1], "-x")) {
        textToHex();
    } else if (argc == 2 && equals(argv[1], "-r")) {
        return hexToText();
    } else if (argc == 3 && equals(argv[1], "-S")) {
        int N = charsToInt(argv[2]);
        if (N > 0 && N < 200) {
            printSequences(charsToInt(argv[2]));
        } else {
            fprintf(stderr, "ERROR: N must be integer in interval (0, 200)!\n");
            return EXIT_FAILURE;
        }
    } else if (argc == 1) {
        textDump(0, -1);
    } else if (argc == 3 || argc == 5) {
        bool definedM = false;
        bool definedN = false;
        int M = 0;
        int N = -1;

        for (int i = 1; i < argc; ++i) {
            if (equals(argv[i], "-n") && (i == 1 || i == 3)) {
                definedN = true;
                N = charsToInt(argv[i + 1]);
                if (N < 1) {
                    fprintf(stderr, "ERROR: N must be positive integer!\n");
                    return EXIT_FAILURE;
                }
            } else if (equals(argv[i], "-s") && (i == 1 || i == 3)) {
                definedM = true;
                M = charsToInt(argv[i + 1]);
                if (M <= -1) {
                    fprintf(stderr, "ERROR: M must be non-negative integer!\n");
                    return EXIT_FAILURE;
                }
            }
        }

        if (argc == 5 && definedN && definedM) {
            textDump(M, N);
        } else if (argc == 3 && (definedN || definedM)) {
            textDump(M, N);
        } else {
            printHelp();
        }

    } else {
        printHelp();
    }

    return (0);
}
