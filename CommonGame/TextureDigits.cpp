//David Millman

#include "TextureDigits.h"

TextureDigits::TextureDigits()
{
	digitSetLength_ = 9-0 + 1;	//0 through 9
}

void TextureDigits::displayDigit(Uint n, Uint fontSet) const
{
	Uint num = n % digitSetLength_;

	displayFrameInRange(num + digitSetLength_*fontSet);
}

/*void TextureDigits::displayNumber(Uint n, Uint maxDigits, Uint fontSet, bool ignoreFrontZeros) const
{
	Uint i = 0;
	Uint mult = 0; 
	Uint value = 0;

	bool frontZero = true;
		
	glPushMatrix();

		// if(!maxDigits)	// this can't work
		// {					//find smallest power of 10 greater than n
		//	value = (Uint)pow(10, maxDigits);	// TODO: value = 0?
		//	while(value < n)
		//		++maxDigits;
		// }

		if(!maxDigits)		// maxDigits == 0
		{
			p = 10;
			maxDigits = 1;
			while(p < n)
			{
				++maxDigits;
				p *= 10;
			}
			//mult = p;
		}		

		for(mult = (Uint)pow(10, maxDigits); mult >= 10; mult /= 10)
		{								//goes from most significant to least significant digit
			value = (n % mult) / (mult/10);

			if(value != 0)
				frontZero = false;

			if(!frontZero || !ignoreFrontZeros || mult/10 < 10) //display front zeros if digit, or front zero if shown, or last digit
				displayDigit(value, fontSet);		
			glTranslated(getDigitWidth(), 0, 0);

			if(n > mult/10)				//accounts for 0's, like 1001, where 001 is just 1
				n -= mult / 10;
		}

	glPopMatrix();
}*/

void TextureDigits::displayNumber(Uint num, Uint maxDigits, Uint fontSet, bool ignoreFrontZeros) const
{
	Uint p = 0;
	Uint digits = 0;
	Uint value = 0;
	Uint maxNum = 0;

	if(!maxDigits)
	{
		p = 10;
		maxDigits = 1;
		while(p < num)
		{
			++maxDigits;
			p *= 10;
		}
	}	
	
	glPushMatrix();
		glTranslated(getDigitWidth() * maxDigits, 0, 0);

		maxNum = pow(10, maxDigits);
		for(p = 1; p <= maxNum && digits < maxDigits; p *= 10, ++digits)
		{
			value = (num % (p*10)) / p;

			if(p != 1 && ignoreFrontZeros && value==0 && p > num)		// if leading zeros, stop
				break;
	
			glTranslated(-(double)getDigitWidth(), 0, 0);
			displayDigit(value, fontSet);				
		}
	glPopMatrix();	
}

// TODO: make this functions algorithm better

/*
displayNumber
{
	string num = itoa(n);

	if(!digits)
		displayString(num);
	else if(digits > num.size())
		zeroes = filledString(num.size() - digits, '0');
		displayString(zeroes);
		displayString(num);
	else 
		displayString(num.substr(0, digits));
}
*/