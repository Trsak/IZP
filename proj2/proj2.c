/*
 * Created by Petr Šopf
 * xsopfp00@vutbr.cz
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Function:  isInt
 * --------------------
 * Checks if double is integer
 *
 *   x: double
 */
bool isInt(double x) {
    int i = (int) x; //Cast double to int.

    if ((double) i == x) { //Check if double is int.
        return true;
    }

    return false;
}

/*
 * Function:  isOddInteger
 * --------------------
 * Checks if double is odd integer
 *
 *   x: double
 */
bool isOddInteger(double x) {
    if (isInt(x) && x != 0.0) { //Check if double is int.
        while (x > 1) {
            x /= 2.0;
        }

        if (x < 1) {
            return true;
        }
    }

    return false;
}

/*
 * Function:  powSpecialValues
 * --------------------
 * Returns special values of pow function.
 *
 *   x: double
 *   y: double
 */
double powSpecialValues(double x, double y) {
    if (x == 1.0 || y == 0.0) {
        //If x is +1, the result is 1.0 (even if y is a NaN).
        //If y is 0, the result is 1.0 (even if x is a NaN).
        return 1.0;
    } else if (isnan(x) || isnan(y)) {
        //If x or y is a NaN, the result is a NaN.
        return NAN;
    } else if (x == 0 && isOddInteger(y) && y > 0) {
        //If x is +0 (-0), and y is an odd integer greater than 0, the result is +0 (-0).
        return 0;
    } else if (x == 0 && !(isOddInteger(y)) && y > 0) {
        //If x is 0, and y greater than 0 and not an odd integer, the result is +0.
        return 0;
    } else if (fabs(x) < 1 && isinf(y) && y < 0) {
        //If the absolute value of x is less than 1, and y is negative infinity, the result is positive infinity.
        return INFINITY;
    } else if (fabs(x) > 1 && isinf(y) && y < 0) {
        //If the absolute value of x is greater than 1, and y is negative infinity, the result is +0.
        return 0.0;
    } else if (fabs(x) < 1 && isinf(y) && y > 0) {
        //If the absolute value of x is less than 1, and y is positive infinity, the result is +0.
        return 0.0;
    } else if (fabs(x) > 1 && isinf(y) && y > 0) {
        //If the absolute value of x is greater than 1, and y is positive infinity, the result is positive infinity.
        return INFINITY;
    } else if (x < 0 && isinf(x) && y < 0 && isOddInteger(y)) {
        //If x is negative infinity, and y is an odd integer less than 0, the result is -0.
        return -0.0;
    } else if (x < 0 && isinf(x) && y < 0 && !isOddInteger(y)) {
        //If x is negative infinity, and y less than 0 and not an odd integer, the result is +0.
        return 0.0;
    } else if (x < 0 && isinf(x) && y > 0 && isOddInteger(y)) {
        //If x is negative infinity, and y is an odd integer greater than 0, the result is negative infinity.
        return -INFINITY;
    } else if (x < 0 && isinf(x) && y > 0 && !isOddInteger(y)) {
        //If x is negative infinity, and y greater than 0 and not an odd integer, the result is positive infinity.
        return INFINITY;
    } else if (x > 0 && isinf(x) && y < 0) {
        //If x is positive infinity, and y less than 0, the result is +0.
        return 0;
    } else if (x > 0 && isinf(x) && y > 0) {
        //If x is positive infinity, and y greater than 0, the result is positive infinity.
        return INFINITY;
    } else if (x == -1.0 || isinf(y)) {
        //If x is -1, and y is positive infinity or negative infinity, the result is 1.0.
        return 1.0;
    }

    return -1.0;
}

/*
 * Function:  logSpecialValues
 * --------------------
 * Returns special values of log function.
 *
 *   x: double
 */
double logSpecialValues(double x) {
    if (x == 0.0) {
        //If x is zero, negative infinity is returned.
        return -INFINITY;
    } else if (x == 1.0) {
        //If x is 1, the result is +0.
        return 0.0;
    } else if (isnan(x)) {
        //If x is a NaN, a NaN is returned.
        return NAN;
    } else if (x < 0) {
        //If x is negative (including negative infinity), a NaN (not a number) is returned.
        return NAN;
    } else if (isinf(x) && x > 0) {
        //If x is positive infinity, positive infinity is returned.
        return INFINITY;
    }

    return -1.0;
}

/*
 * Function:  cfrac_log
 * --------------------
 * Returns the log value of x in specified number of iterations using continued fractions.
 *
 *   x: double
 *   n: unsigned int - number of iterations
 */
double cfrac_log(double x, unsigned int n) {
    //Check for special values.
    double specialValue = logSpecialValues(x);
    if (specialValue != -1.0) {
        return specialValue;
    }

    //Formula is for log((1+z)/(1-z)), we have to get z
    double z = (x - 1) / (x + 1);

    //We are going from behind, so we have to define the last element.
    double result = (1 + (n - 1) * 2);

    for (unsigned int m = n - 1; m > 0; --m) {
        result = (1 + (m - 1) * 2) - (m * m * z * z / result);
    }

    return 2 * z / result;
}

/*
 * Function:  taylor_log
 * --------------------
 * Returns the log value of x in specified number of iterations using taylor polynom.
 *
 *   x: double
 *   n: unsigned int - number of iterations
 */
double taylor_log(double x, unsigned int n) {
    //Check for special values.
    double specialValue = logSpecialValues(x);
    if (specialValue != -1.0) {
        return specialValue;
    }

    double result = 0.0;

    //Use different formulas for x >= 1 and x < 1.
    if (x >= 1) {
        double last = (x - 1) / x;

        for (unsigned int m = 1; m <= n; m++) {
            result += last / m;
            last *= (x - 1) / x;
        }
    } else if (x > 0) {
        //We have to change x, because formula is for log(1-x).
        x = 1 - x;
        result = -x;
        double last = x;

        for (unsigned int m = 2; m <= n; m++) {
            last = x * last;
            result -= last / m;
        }
    }

    return result;
}

/*
 * Function:  taylor_pow
 * --------------------
 * Returns the pow value of x and y in specified number of iterations, using taylor_log fuction for log.
 *
 *   x: double
 *   y: double
 *   n: unsigned int - number of iterations
 */
double taylor_pow(double x, double y, unsigned int n) {
    //Check for special values.
    double specialValue = powSpecialValues(x, y);
    if (specialValue != -1.0) {
        return specialValue;
    }

    double result = 1.0, temp = 1.0;
    double ln = taylor_log(x, n);

    for (unsigned int i = 1; i <= n; ++i) {
        temp = (temp * y * ln) / i;
        result = result + temp;
    }

    return result;
}

/*
 * Function:  taylorcf_pow
 * --------------------
 * Returns the pow value of x and y in specified number of iterations, using cfrac_log fuction for log.
 *
 *   x: double
 *   y: double
 *   n: unsigned int - number of iterations
 */
double taylorcf_pow(double x, double y, unsigned int n) {
    //Check for special values.
    double specialValue = powSpecialValues(x, y);
    if (specialValue != -1.0) {
        return specialValue;
    }

    double result = 1.0, temp = 1.0;
    double ln = cfrac_log(x, n);

    for (unsigned int i = 1; i <= n; ++i) {
        temp = (temp * y * ln) / i;
        result = result + temp;
    }

    return result;
}

/*
 * Function:  getEps
 * --------------------
 * Returns modified eps for 8 significant digits;
 *
 *   value: double
 */
double getEps(double value) {
    value = fabs(value);

    //Default eps if there is just one digit before the decimal point.
    double eps = 1e-8;
    double preRes;

    preRes = value / 10.0;

    while (preRes >= 10) { //Only if result is bigger or same as 10.
        //Multiply eps by ten for every another digit before the decimal point.
        eps *= 10.0;
        preRes /= 10.0;
    }

    while (preRes < 1.0) { //Only if result is lower then 1.
        //Divide eps by ten for every zero after the decimal point.
        eps /= 10.0;
        preRes *= 10.0;
    }

    return eps;
}

/*
 * Function:  mylog
 * --------------------
 * Returns the log value of x with precision of 8 significant digits.
 *
 *   x: double
 */
double mylog(double x) {
    //Check for special values.
    double specialValue = logSpecialValues(x);
    if (specialValue != -1.0) {
        return specialValue;
    }

    double taylor = 0.0, cfrac = 0.0, taylorLast, cfracLast, last = 0.0, pX = x;
    double result = 0.0;
    bool done = false;

    if (x >= 1) {
        last = (x - 1) / x;

    } else if (x > 0) {
        x = 1 - x;
        taylor = -x;
        last = x;
    }

    //We will call both taylor and cfrac. When we get value with needed precision, we will end the loop and return value.
    for (unsigned int i = 1; !done; ++i) {
        taylorLast = taylor;

        if (x >= 1) {
            taylor += last / i;
            last *= (x - 1) / x;
        } else if (pX > 0) {
            last = x * last;
            taylor -= last / (i + 1);
        }

        cfracLast = cfrac;
        cfrac = cfrac_log(x, i);

        //If taylor value has needed precision, then end loop and save it as result.
        if (fabs(taylorLast - taylor) <= getEps(taylor)) {
            done = true;
            result = taylor;
        }

        //If taylor value has needed precision, then end loop. We have to check if taylor didn't find value aswell - if yes, return the one better value.
        if (fabs(cfracLast - cfrac) <= getEps(cfrac)) {
            if (done != true || (fabs(cfracLast - cfrac) < fabs(taylorLast - taylor))) {
                done = true;
                result = cfrac;
            }
        }
    }

    return result;
}

/*
 * Function:  mypow
 * --------------------
 * Returns the pow value of x and y with precision of 8 significant digits.
 *
 *   x: double
 *   y: double
 */
double mypow(double x, double y) {
    //Check for special values.
    double specialValue = powSpecialValues(x, y);
    if (specialValue != -1.0) {
        return specialValue;
    }

    double result = 1.0, resultLast, diff, temp = 1.0;
    double eps;
    double ln = mylog(x);
    bool same = false;

    for (unsigned int i = 1; !same; ++i) {
        resultLast = result;
        temp = (temp * y * ln) / i;
        result = result + temp;

        diff = fabs(resultLast - result);

        eps = getEps(result);
        if (diff <= eps) { //If result is in needed precision, we have to end loop.
            same = true;
        }
    }

    return result;
}

/*
 * Function:  validDouble
 * --------------------
 * Validates if given argument is double
 *
 *   X: string - argument
 *   name: string - name of double variable
 *   error: boolean
 */
double validDouble(char X[], char *name, bool *error) {
    char *string;
    double x = strtod(X, &string);
    if (strlen(string) != 0) { //If there is an text in argument, we have to print error.
        fprintf(stderr, "ERROR: %s must be double data type! ", name);
        *error = true;
    }

    return x;
}

/*
 * Function:  validN
 * --------------------
 * Validates if given argument is integer bigger then 0.
 *
 *   N: string - argument
 *   error: boolean
 */
unsigned int validN(char N[], bool *error) {
    char *string;
    long int value = strtoul(N, &string, 10);

    if (value < 1 || strlen(string) != 0) {
        fprintf(stderr, "ERROR: N must be integer bigger then 0! ");
        *error = true;
    }

    return (unsigned) value;
}

/*
 * Function:  main
 * --------------------
 * Main function, processes the arguments.
 *
 *   argc: number of arguments
 *   argv: arguments
 */
int main(int argc, char *argv[]) {
    if (argc == 4 && strcmp("--log", argv[1]) == 0) {
        bool error = false;

        double X = validDouble(argv[2], "X", &error);
        unsigned int N = validN(argv[3], &error);

        if (!error) {
            printf("       log(%g) = %.12g\n", X, log(X));
            printf(" cfrac_log(%g) = %.12g\n", X, cfrac_log(X, N));
            printf("taylor_log(%g) = %.12g\n", X, taylor_log(X, N));
        } else {
            return EXIT_FAILURE;
        }
    } else if (argc == 5 && strcmp("--pow", argv[1]) == 0) {
        bool error = false;

        double X = validDouble(argv[2], "X", &error);
        double Y = validDouble(argv[3], "Y", &error);
        int N = validN(argv[4], &error);

        if (!error) {
            printf("         pow(%g,%g) = %.12g\n", X, Y, pow(X, Y));
            printf("  taylor_pow(%g,%g) = %.12g\n", X, Y, taylor_pow(X, Y, (unsigned) N));
            printf("taylorcf_pow(%g,%g) = %.12g\n", X, Y, taylorcf_pow(X, Y, (unsigned) N));
        } else {
            return EXIT_FAILURE;
        }
    } else if (argc == 3 && strcmp("--mylog", argv[1]) == 0) {
        bool error = false;

        double X = validDouble(argv[2], "X", &error);

        if (!error) {
            printf("       log(%g) = %.7e\n", X, log(X));
            printf("     mylog(%g) = %.7e\n", X, mylog(X));
        } else {
            return EXIT_FAILURE;
        }
    } else if (argc == 4 && strcmp("--mypow", argv[1]) == 0) {
        bool error = false;

        double X = validDouble(argv[2], "X", &error);
        double Y = validDouble(argv[3], "Y", &error);

        if (!error) {
            printf("       pow(%g) = %.7e\n", X, pow(X, Y));
            printf("     mypow(%g) = %.7e\n", X, mypow(X, Y));
        } else {
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "ERROR: Bad arguments! \nUse --log X N or --pow X Y N");
        return EXIT_FAILURE;
    }

    return 0;
}