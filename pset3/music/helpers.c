// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // extract numerator and denominator
    string numer = strtok(fraction, "/");
    string denom = strtok(NULL, "/");

    int numerator = atoi(numer);
    int denominator = atoi(denom);
    int number = (double) numerator / denominator / 0.125;

    return number;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    const char *NOTES[] = {"C", "-9", "D", "-7", "E", "-5", "F", "-4", "G", "-2", "A", "0", "B", "2"};
    double base_freq = 440;

    // Adjust according to letter
    for (int i = 0; i < 14; i++)
    {
        // find the corresponding letter
        if (strncmp(&note[0], NOTES[i], 1) == 0)
        {
            int semitones = atoi(NOTES[i + 1]);
            double power = (double) semitones * 1 / 12;

            // multiply if B, else divide
            if (semitones < 0)
            {
                base_freq /= pow(2, fabs(power));
            }
            else if (semitones > 0)
            {
                base_freq *= pow(2, power);
            }
            // if semitones = 0, do nothing
        }
    }

    // check for notes with no sharps/flats
    if (strlen(note) == 2)
    {
        // base case: A4 if returned
        if (strcmp(note, "A4") == 0)
        {
            return base_freq;
        }
        else
        {
            // find power of 2 to multiply/divide by
            int octave = atoi(&note[1]);
            int power = octave - 4;

            if (power < 0)
            {
                // divide if power is negative (<4)
                return (int) round(base_freq / pow(2, abs(power)));
            }
            else
            {
                // multiply if power is positive (>4)
                return (int) round(base_freq * pow(2, abs(power)));
            }
        }
    }
    else if (strlen(note) == 3)
    {
        char accidental = note[1];
        char octave = note[2];

        // adjust for sharp #
        if (strncmp(&accidental, "#", 1) == 0)
        {
            base_freq *= pow(2, 1 / (double)12);
        }
        // adjust for flat b
        else if (strncmp(&accidental, "b", 1) == 0)
        {
            base_freq /= pow(2, 1 / (double)12);
        }

        // adjust for octave
        int power = atoi(&octave) - 4;

        if (power < 0)
        {
            // divide if power is negative (<4)
            return (int) round(base_freq / pow(2, abs(power)));
        }
        else if (power > 0)
        {
            // multiply if power is positive (>4)
            return (int) round(base_freq * pow(2, abs(power)));
        }
        else
        {
            return (int) round(base_freq);
        }
    }
    return 1;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, "") != 0)
    {
        return false;
    }
    else if (strcmp(s, "") == 0)
    {
        return true;
    }
    return false;
}
