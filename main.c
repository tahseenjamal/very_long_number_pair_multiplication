#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static const char END_OF_LINE = '\0';

static const char ZERO_ASCII = '0';

/*
 * Reverses the string. String is passed pointer. So technically don't need a return char*
 * But still return type char* used so that it can also be directly used with return statement
 */
char* reverse_string(char *string_to_reverse)
{
      char *pointer1, *pointer2;

      if (! string_to_reverse || ! *string_to_reverse)
            return string_to_reverse;
      for (pointer1 = string_to_reverse,
              pointer2 = string_to_reverse + strlen(string_to_reverse) - 1;
              pointer2 > pointer1; ++pointer1, --pointer2) {

            *pointer1 ^= *pointer2;
            *pointer2 ^= *pointer1;
            *pointer1 ^= *pointer2;
      }

      return string_to_reverse;
}

    /*
     * Multiplying first number by each digit of second number by passing digit of second number one by one
     */
void single_step_multiplication(char *primary_number_as_string,
                                int secondary_number_digit, char *wip_answer, int secondary_number_digit_position) {

    char carry_forward = 0, single_digit_number = 0 , wip_length_counter = 0;

    /*
     * Depending on which layer it is, add tailing zero. As you understand, with digit position at unit
     * ten, hundred, thousand etc position, you need to accordingly add tailing zeros to
     */
    while(wip_length_counter < secondary_number_digit_position)

        wip_answer[wip_length_counter++] = ZERO_ASCII;
    /*
     * so work in progress answer has tailing 0 basis secondary number digit position in second_number
     */

    int primary_number_length_counter = strlen(primary_number_as_string) -1 ;

    /*
     * Now traverse the first number and multiply its each digit with second number's digit passed
     */
    for(; primary_number_length_counter >= 0 ; primary_number_length_counter--) {

        /*
         * Simple maths of multiplying two numbers and if result is greater than single digit then
         * carry forward the second digit to add in the next cycle of digit multiplication
         */
        single_digit_number = primary_number_as_string[primary_number_length_counter] - ZERO_ASCII;

        single_digit_number = single_digit_number * secondary_number_digit + carry_forward;

        carry_forward = single_digit_number / 10;

        single_digit_number -= carry_forward * 10;

        wip_answer[wip_length_counter] = single_digit_number + ZERO_ASCII;

        wip_length_counter++;
    }

    /*
     * The layer can be longer so continue processing it by checking if there is a carry forward to
     * add to it. And if so do it. Because work in progress length counter has already incremented
     * towards the end of loop, it would be pointing to an empty cell. Note entry processing is
     * done in this function from left to right as the number string is reverse
     */
    if (carry_forward != 0 )
        wip_answer[wip_length_counter] = carry_forward + ZERO_ASCII;


}

    /*
     * When you multiply first number with a digit of second number, you have one layer of number
     * And when you multiply first number with another digit of second number, then you have one more layer
     * So you need to keep adding these layers with a shift, as you do it conventionally
     */
void paired_layer_addition(char* wip_answer, char* final_answer) {

    int wip_answer_counter = 0;

    char final_answer_digit, wip_answer_digit, net_of_digits, carry_forward = 0;

    for(; wip_answer_counter < strlen(wip_answer); wip_answer_counter++) {

        /*
         * final answer holds values from previous paired layer addition, so we check if final answer
         * and work in progress aligned digits are added, carry forward taken to next and all finsihed
         */
        if(wip_answer_counter < strlen(final_answer)) {

            final_answer_digit = final_answer[wip_answer_counter] - ZERO_ASCII;

            wip_answer_digit = wip_answer[wip_answer_counter] - ZERO_ASCII;

            net_of_digits = final_answer_digit + wip_answer_digit + carry_forward;

            carry_forward = (net_of_digits / 10);

            net_of_digits = net_of_digits - 10 * carry_forward;

            final_answer[wip_answer_counter] = net_of_digits + ZERO_ASCII;

        /*
         * Then start with just work in progress layer digits, if carry forward, then add to it, and if it
         * again results in carry forward continue until last digit is done
         */
        } else {

            wip_answer_digit = wip_answer[wip_answer_counter] - ZERO_ASCII;

            net_of_digits = wip_answer_digit + carry_forward;

            carry_forward = (net_of_digits / 10);

            net_of_digits = net_of_digits - 10 * carry_forward;

            final_answer[wip_answer_counter] = net_of_digits + ZERO_ASCII;
        }
    }

    /*
     * Because loop ends with the length of work in progress answer but there can be a carry forward
     * left to adjust in the final number, this extra step is performed
     */
    if(carry_forward)

        final_answer[wip_answer_counter] = carry_forward + ZERO_ASCII;

}

void digit_by_digit_processing(char* first_number, char* second_number, char* wip_answer, char* final_answer) {

    /*
     * We reverse the second number string so that we pick digits left to right instead of right to left
     * as it is easier to traverse left to right instead of right to left
     */
    reverse_string(second_number);

    /*
     * This counter helps us keep track of up to which position we have used digits of second number
     * and also this same variable is used to add zeros to the layer depending on which position digit
     * it was
     */
    int second_number_length_counter = 0;

    /*
     * Variables to hold single digits extracted from second number to pass for multiple with first number
     */
    char multiplier_digit;

    for(;second_number_length_counter < strlen(second_number); second_number_length_counter++) {

        wip_answer = (char*)malloc(sizeof(char) * (strlen(first_number) + second_number_length_counter + 1));

        multiplier_digit = second_number[second_number_length_counter] - ZERO_ASCII;

        /*
         * Fill the work in progress answer with end of line character
         */
        memset(wip_answer, END_OF_LINE, strlen(wip_answer));

        single_step_multiplication(first_number,
                                   multiplier_digit, wip_answer,
                                   second_number_length_counter);

        /*
         * Check if we have two layers to start addition, if not copy the starting layer into final answer
         * and thereafter new layers would be added to final answer, result stored back in final answer
         */
        final_answer[0] == END_OF_LINE ? strcpy(final_answer, wip_answer) : paired_layer_addition(wip_answer, final_answer);

        /*
         * Must free the work in progress variable as it is not used beyond this point
         */
        free(wip_answer);

    }

}

char* multiply(char* first_passed_number, char* second_passed_number) {

    // Initialized variable to create a program copy
    char* first_number, *second_number, *wip_answer, *final_answer;

    // Allocating memory equivalent to the length of first string value passed on command line
    first_number = (char*)malloc(sizeof(char) * strlen(first_passed_number));

    // Allocating memory equivalent to the length of second string value passed on command line
    second_number = (char*)malloc(sizeof(char) * strlen(second_passed_number));

    /*
     * Multiplication results in total number of digits, which as much as n + 2
     * where n is the sum of total digits in first number and second number.
     * As we have to hold the final multiplication answer spanning over n + 2 digits
     * we need to allocate that space for final answer
     */
    final_answer = (char*)malloc(sizeof(char) * (strlen(first_passed_number) +
                            strlen(second_passed_number) + 4));

    /*
     * Fill the variable with end of line characters, so that during operation, when used,
     * if partially occupied and string length to be calculated, random memory garbage should
     * not get counted
     */
    memset(final_answer, END_OF_LINE, strlen(final_answer));

    strcpy(first_number, first_passed_number);

    strcpy(second_number, second_passed_number);

    /*
     * This function plans out executing multiplication by using first number and multiply
     * each digit of second number. Multiplying first number with each digit of second number results
     * in layers of results that have to added with carry forwards like traditional multiplication
     */
    digit_by_digit_processing(first_number, second_number, wip_answer, final_answer);

    /*
     * All done, so no need of first and second number, free them up
     */
    free(first_number);

    free(second_number);

    /*
     * Return the reverse of the string as we have been operating from left to right
     * And it is time to show to the correct flipped string
     */
    return reverse_string(final_answer);

}

int main(int argc, char **argv) {

    // command line arguments, which are as string are passed as pointers to the multiply function
    char* result = multiply(argv[1], argv[2]);

    // print the result
    printf("Result: %s\n",result);

    // because result is a pointed and has been passed a malloc variable, it should be freed before exiting
    free(result);

    return 0;

}
