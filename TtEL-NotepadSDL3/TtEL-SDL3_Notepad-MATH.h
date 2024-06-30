#pragma once
// 
/**
* @file TtEL-SDL3_Notepad-MATH.h
* @brief A version of TtEL Math Extended made for the GUI Notepad,
* based on the version of TtEL Math Extended made for the GUI Calcuator,
* itself based on the one made for the SRT Synchronized Lyric Player
* @author Team theEeveeLovers Sean
* @date May 23rd, 2024
*/

#ifndef SDL3_CALC_MATH
#define SDL3_CALC_MATH
#include "math.h"

// These functions were loaned from f2c.h which is publically available.
// Handle them with care. They have been commented for your convienience.
#define min(a,b) ((a) <= (b) ? (a) : (b)) // get the lowest value of two inputs
#define max(a,b) ((a) >= (b) ? (a) : (b)) // get the highest value of two inputs


namespace mathext {
	// Parity functions
	/**
	* @brief Checks the parity of an integer to see if it is even.
	* @param NumToTest - The integer to check for even parity
	* @returns A bool, with true meaning the integer was even.
	*/
	bool isIntEven(int NumToTest) { return (NumToTest & 1) == 0; }
	/**
	* @brief Checks the parity of an integer to see if it is odd.
	* @param NumToTest - The integer to check for odd parity
	* @return A bool, with true meaning the integer was odd.
	*/
	bool isIntOdd(int NumToTest) { return (NumToTest & 1) != 0; }



	// Threshold/Clamping functions
	/**
	* @brief Checks if a double is between two bounds.
	*
	* @param numToTest The double to test
	* @param lowerBound The lower bound, being the minimum value.
	* @param upperBound The upper bound, being the maximum value.
	* @return A bool, with true meaning the input double was between the two bounds.
	*/
	bool thresholdDouble(double numToTest, double lowerBound, double upperBound) {
		if (numToTest > upperBound || numToTest < lowerBound) { return false; } // if the number is greater than the upper bound or lower than the lower bound
		else { return true; } // if neither of those are the case
	}

	/**
	* @brief Checks if a float is between two bounds.
	*
	* @param numToTest The float to test
	* @param lowerBound The lower bound, being the minimum value.
	* @param upperBound The upper bound, being the maximum value.
	* @return A bool, with true meaning the input float was between the two bounds.
	*/
	bool thresholdFloat(float numToTest, float lowerBound, float upperBound) {
		if (numToTest > upperBound || numToTest < lowerBound) { return false; } // if the number is greater than the upper bound or lower than the lower bound
		else { return true; } // if neither of those are the case
	}
	/**
	* @brief Checks if a long is between two bounds.
	* 
	* @param numToTest The long to test
	* @param lowerBound The lower bound, being the minimum value.
	* @param upperBound The upper bound, being the maximum value.
	* @return A bool, with true meaning the input long was between the two bounds.
	*/
	bool thresholdLong(long numToTest, long lowerBound, long upperBound) {
		if (numToTest > upperBound || numToTest < lowerBound) { return false; } // if the number is greater than the upper bound or lower than the lower bound
		else { return true; } // if neither of those are the case
	}
	/**
	* @brief Checks if a int is between two bounds.
	* 
	* @param numToTest The int to test
	* @param lowerBound The lower bound, being the minimum value.
	* @param upperBound The upper bound, being the maximum value.
	* @return A bool, with true meaning the input int was between the two bounds.
	*/
	bool thresholdInt(int numToTest, int lowerBound, int upperBound) {
		if (numToTest > upperBound || numToTest < lowerBound) { return false; } // if the number is greater than the upper bound or lower than the lower bound
		else { return true; } // if neither of those are the case
	}
	/**
	* @brief Clamps a double between two bounds.
	* 
	* @param dblToClamp The double to clamp
	* @param lowerBound The double is increased if it is below this bound.
	* @param upperBound The double is decreased if it is above this bound.
	* @sa clampFloat
	* @return The input double, clamped between the two bounds.
	*/
	double clampDouble(double dblToClamp, double lowerBound, double upperBound) {
		if (dblToClamp > upperBound) {
			return upperBound; // If the input is higher than wanted
		}
		else if (dblToClamp < lowerBound) {
			return lowerBound; // If the input is lower than wanted
		}
		else {
			return dblToClamp; // If neither of those are true
		}
	}
	/**
	* @brief Clamps a float between two bounds.
	*
	* @param fltToClamp - The float to clamp
	* @param lowerBound - The float is increased if it is below this bound.
	* @param upperBound - The float is decreased if it is above this bound.
	* @returns The input float, clamped between the two bounds.
	*/
	float clampFloat(float fltToClamp, float lowerBound, float upperBound) {
		if (fltToClamp > upperBound) {
			return upperBound; // If the input is higher than wanted
		}
		else if (fltToClamp < lowerBound) {
			return lowerBound; // If the input is lower than wanted
		}
		else {
			return fltToClamp; // If neither of those are true
		}
	}
	/**
	* @brief Clamps a integer between two bounds.
	*
	* @param intToClamp - The integer to clamp
	* @param lowerBound - The integer is increased if it is below this bound.
	* @param upperBound - The int is decreased if it is above this bound.
	* @returns The input int, clamped between the two bounds.
	*/
	int clampInt(int intToClamp, int lowerBound, int upperBound) {
		if (intToClamp > upperBound) {
			return upperBound; // If the input is higher than wanted
		}
		else if (intToClamp < lowerBound) {
			return lowerBound; // If the input is lower than wanted
		}
		else {
			return intToClamp; // If neither of those are true
		}
	}

	// Comparison Functions
	/**
	* @brief A function that gives the lowest float of its inputs.
	* 
	* @param float1 First float to compare
	* @param float2 Second float to compare
	* @sa std::min from <algorithm>
	* @return The input float with the lowest value.
	*
	*/
	float LowestFloat(float float1, float float2) {
		if (float1 <= float2) { // if float1 is less than or equal to float2
			return float1;
		}
		else if (float1 > float2) {
			return float2; // if float1 is greater than float2
		}
		else {
			// this should never get run, but I'm still defining something here 
			// else compiler warns "not all control paths return a value" (C4715)
			return 0.0f;
		}
	}
	/**
	* @brief A function that gives the highest float of its inputs.
	* 
	* @param float1 First float to compare
	* @param float2 Second float to compare
	* @sa std::max from <algorithm>
	* @return The input float with the highest value.
	*
	*/
	float HighestFloat(float float1, float float2) {
		if (float1 >= float2) { // if float1 is greater than or equal to float2
			return float1;
		}
		else if (float1 < float2) {
			return float2; // if float1 is less than float2
		}
		else {
			// this will never get run, but I'm still defining something here 
			// else compiler will warn "not all control paths return a value" (C4715)
			return 0.0f;
		}
	}
}
#endif
