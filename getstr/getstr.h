/*
 * getstr.h - get string from stdin
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-12-05
 *
 *
 * In main compilation unit; define GETSTR_IMPLEMENT
 *
 *
 * These are the available functions:
 *
 * char* getstr(const char* format, ...)
 *
 *
 * Uses va_list for argument parsing, like in debug.h
 */

/*
 * From here on, until GETSTR_IMPLEMENT,
 * it is like a normal header file with declarations
 */

#ifndef GETSTR_H
#define GETSTR_H

extern char* getstr(const char* format, ...);

#endif // GETSTR_H

/*
 * This header library file uses _IMPLEMENT guards
 *
 * If GETSTR_IMPLEMENT is defined, the definitions will be included
 */

#ifdef GETSTR_IMPLEMENT

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/*
 * Parse va_list argument and print it to a buffer
 *
 * PARAMS
 * - char* buffer          | Buffer to store printed argument
 * - const char* specifier | Argument format specifier
 * - va_list args          | va_list argument list
 *
 * RETURN (int amount, same as sprintf)
 * - >=0 | Number of printed characters
 * -  -1 | Format specifier does not exist, or sprintf error
 */
static inline int gstr_specifier_append(char* buffer, const char* specifier, va_list args)
{
  if(strncmp(specifier, "d", 1) == 0)
  {
    int arg = va_arg(args, int);

    return sprintf(buffer, "%d", arg);
  }
  else if(strncmp(specifier, "ld", 2) == 0)
  {
    long int arg = va_arg(args, long int);

    return sprintf(buffer, "%ld", arg);
  }
  else if(strncmp(specifier, "lld", 2) == 0)
  {
    long long int arg = va_arg(args, long long int);

    return sprintf(buffer, "%lld", arg);
  }
  else if(strncmp(specifier, "c", 1) == 0)
  {
    // ‘char’ is promoted to ‘int’ when passed through ‘...’
    int arg = va_arg(args, int);

    return sprintf(buffer, "%c", arg);
  }
  else if(strncmp(specifier, "f", 1) == 0)
  {
    // ‘float’ is promoted to ‘double’ when passed through ‘...’
    double arg = va_arg(args, double);

    return sprintf(buffer, "%lf", arg);
  }
  else if(strncmp(specifier, "s", 1) == 0)
  {
    const char* arg = va_arg(args, const char*);

    return sprintf(buffer, "%s", arg);
  }
  else return -1; // Specifier does not exist
}

/*
 * Formats just a single format specifier argument from va_list
 *
 * RETURN (int amount, same as sprintf)
 * - >=0 | Number of printed characters
 * -  -1 | Format specifier does not exist, or sprintf error
 */
static inline int gstr_arg_append(char* buffer, const char* format, int f_length, int* f_index, va_list args)
{
  char specifier[f_length + 1];

  for(int s_index = 0; (*f_index)++ < f_length; s_index++)
  {
    specifier[s_index] = format[*f_index];
    specifier[s_index + 1] = '\0';

    int amount = gstr_specifier_append(buffer, specifier, args);

    // If a valid format specifier has been found and parsed,
    // return the status of the appended specifier
    if(amount > 0) return amount;
  }

  return -1;
}

/*
 * sprintf, but with va_list as arguments
 *
 * RETURN (same as sprintf)
 * - >=0 | Number of printed characters
 * -  -1 | Format specifier does not exist, or sprintf error
 */
static inline int gstr_prompt_create(char* prompt, const char* format, va_list args)
{
  const size_t f_length = strlen(format);

  int p_index = 0;

  for(int f_index = 0; f_index < f_length; f_index++)
  {
    if(format[f_index] == '%')
    {
      int amount = gstr_arg_append(prompt + p_index, format, f_length, &f_index, args);

      if(amount < 0) return -1;

      p_index += amount;
    }
    else prompt[p_index++] = format[f_index];
  }

  prompt[p_index] = '\0';

  return p_index;
}

/*
 * getstr - get string from stdin
 *
 * The following takes place:
 * 1. The prompt is printed to stdout
 * 2. The user inputs from stdin
 *
 * RETURN (char* string)
 * - NULL | An error occured
 *
 * Maybe: Assign errno a suitable value
 */
char* getstr(const char* format, ...)
{
  // 1. Create prompt
  va_list args;

  va_start(args, format);

  char prompt[1024];

  if(gstr_prompt_create(prompt, format, args) == -1)
  {
    va_end(args);

    return NULL;
  }

  va_end(args);


  // 2. Output the prompt to stdout
  if(fprintf(stdout, "%s", prompt) < 0)
  {
    return NULL;
  }


  // 3. Input string from stdin
  char buffer[1024];

  if(fgets(buffer, sizeof(buffer), stdin) == NULL)
  {
    return NULL;
  }

  buffer[strlen(buffer) - 1] = '\0';

  return strdup(buffer);
}

#endif // GETSTR_IMPLEMENT
