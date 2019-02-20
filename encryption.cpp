/*
	Alexander Alfonso (aja0167)
	CSCE 4550 Project 2: Modified AES Encryption
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <bitset> 	// for binary representation
#include <math.h>	//	ceil()

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

int processElement(int matrixElement, int n);		//	matrixElement is from parity grid
													//	n is the constant grid we do matrix multi. with
int columnAddition(int c0, int c1, int c2, int c3);	//	matrix columns to be added together

int main(int argc, char *argv[])
{
	//	usage statement
	if(argc != 4)
	{
		cout << "usage: ./encryption <plaintext_file> <key_file> <ciphertext_file>\n";
		exit(EXIT_FAILURE);
	}

	//	open all three files
	ifstream pt_file, k_file;
	ofstream ct_file;
	
	//	plaintext file
	pt_file.open(argv[1]);
	if(!pt_file.is_open())
	{
		cout << "ERROR: " << argv[1] << " failed to open.\n";
		exit(EXIT_FAILURE);
	}

	//	key file
	k_file.open(argv[2]);
	if(!k_file.is_open())
	{
		cout << "ERROR: " << argv[2] << " failed to open.\n";
		exit(EXIT_FAILURE);
	}

	//	output file
	ct_file.open(argv[3]);
	if(!ct_file.is_open())
	{
		cout << "ERROR: " << argv[3] << " failed to open.\n";
		exit(EXIT_FAILURE);
	}

	//	header
	cout 	<< "-------------------------	\n";
	cout 	<< "    Alexander Alfonso		\n";
	cout 	<< "  CSCE 4550: Project 2		\n";
	cout 	<< " Modified AES Encryption    \n";
	cout 	<< "-------------------------	\n";

	ct_file << "-------------------------	\n";
	ct_file << "     Alexander Alfonso		\n";
	ct_file << "   CSCE 4550: Project 2		\n";
	ct_file << " Modified AES Encryption    \n";
	ct_file << "-------------------------	\n";

	//	remove all spaces and punctuation from the text and place it in a buffer
	cout << "Preprocessing plaintext file... \n";
	ct_file << "Preprocessing plaintext file... \n";
	char c;
	int bufSize = 0;
	string processedBuffer;
	while(pt_file >> c)
	{
		if(isalpha(c) && isupper(c))
		{
			//cout << c;
			processedBuffer += c;
			bufSize++;
		}
	}

	//	read key file
	cout << "Reading key... \n";
	ct_file << "Reading key... \n";
	int keySize = 0;
	string keyBuffer;
	while(k_file >> c)
	{
		if(isalpha(c) && isupper(c))
		{
			keyBuffer += c;
		}
		keySize++;
	}

	//	check if it is a valid key (exactly 16 uppercase letters);
	if(keySize != 16)
	{
		cout << "key size: " << keySize << endl;
		cout << "ERROR: Invalid key format.\n";
		exit(EXIT_FAILURE);
	}

	cout << "Processing key...\n";
	ct_file << "Processing key...\n";

	string keyStream;	//	will match the length of characters
						//	the processed input has with key chars

	//	check if preprocessed input has more length than key
	if(keySize < bufSize)
	{
		keySize = 0;
		int j = 0;		//	index for the key to find the char to add to keystream
		int i = 0;

		//	while key buffer length is less than processed input length
		//	concatenate the keystream buffer to match the length of the
		//	input length buffer
		while(i != bufSize)
		{
			if (j == 16)
			{
				j = 0;
			}
			else
			{
				c = keyBuffer[j];
				keyStream += c;
				j++;
				i++;
				keySize++;
			}
		}
	}
	else
	{
		cout << "Key size is larger than provided plaintext\n";
		exit(EXIT_FAILURE);
	}
	
	cout << "Performing substitution...\n";
	ct_file << "Performing substitution...\n";

	//	process the cipher buffer
	string cipherBuf;
	int cipherSize = 0;
	for(int i = 0; i<keySize; i++)
	{
		char a, b, cipherChar;
		int x, y, z, w;

		//	add letters from keystream and processed input together
		a = keyStream[i];
		b = processedBuffer[i];

		x = a;
		y = b;
		z = x + y;

		//	find ascii value for ciphertext character
		z = z % 26;
		z += 65;

		cipherChar = z;

		cipherBuf += cipherChar;
		cipherSize++;	//	used for gridCount to determine how many 4x4 blocks there will be
	}

	cout << "key: \t\t" << keyBuffer << endl;
	cout << "keystream: \t" << keyStream << endl;
	cout << "preprocessing: \t" << processedBuffer << endl;
	cout << "substitution: \t" << cipherBuf << endl;

	ct_file << "preprocessing: \t" << processedBuffer << endl;
	ct_file << "substitution: \t" << cipherBuf << endl;

	//	determine how many 4x4 blocks there will be
	int blockCount = 0;
	float blockCeil = (cipherSize/16.0);	//	will be used to round a number up for the # of blocks
	blockCount = ceil(blockCeil);			//	round number up and fill extra spaces with 'A's

	//	make padded buffer 64-bits long
	cout << "\nPadding...\n";
	ct_file << "\nPadding...\n";
	string padBuf;
	int padCount = 0;
	int paddedCipherBuf = blockCount*16;
	while(padCount != paddedCipherBuf)
	{
		if (padCount < keySize)
		{
			padBuf += cipherBuf[padCount];
			padCount++;
		}
		else // i >= keySize
		{
			padBuf += 'A';
			padCount++;
		}	
	}
	
	//	from padded buffer, create four 4x4 blocks
	char padGrid[4][4][blockCount];	//	x, y, # block
	int gridCount = 0;
	
	for(int blockNum = 0; blockNum<blockCount; blockNum++)
	{
		for (int x = 0; x<4; x++)
		{
			for (int y = 0; y<4; y++)
			{
				padGrid[x][y][blockNum] = padBuf[gridCount++];
				cout << padGrid[x][y][blockNum];
				ct_file << padGrid[x][y][blockNum];
			}
			cout << endl;
			ct_file << endl;
		}
		cout << endl;
		ct_file << endl;
	}
	
	//	shift rows
	//	row 1 stay same, row 2 shift left once
	//	row 3 shift left twice, row 4 shift left thrice
	cout << "Shifting rows...\n";
	ct_file << "Shifting rows...\n";
	for(int blockNum = 0; blockNum<blockCount; blockNum++)	
	{
		for (int x = 0; x<4; x++)		//	x, row
		{
			char ch0 = padGrid[x][0][blockNum];	//	these characters will disappear
			char ch1 = padGrid[x][1][blockNum];	//	save it so it could be used to 
			char ch2 = padGrid[x][2][blockNum];	//	replace the last character

			for (int y = 0; y<4; y++)	//	y, col
			{
				if(x == 0)				// first row, NO LS
				{
										//	stays the same
				}
				else if (x == 1)		//	second row, LS 1
				{
					padGrid[x][y][blockNum] = padGrid[x][y+1][blockNum];

					if(y == 3)
					{
						padGrid[x][y][blockNum] = ch0;
					}
				}
				else if (x == 2)		//	third row, LS 2
				{
					padGrid[x][y][blockNum] = padGrid[x][y+2][blockNum];

					if(y == 2)
					{
						padGrid[x][y][blockNum] = ch0;
					}
					else if (y == 3)
					{
						padGrid[x][y][blockNum] = ch1;
					}
				}
				else if (x == 3)		//	fourth row, LS 3
				{
					padGrid[x][y][blockNum] = padGrid[x][y+3][blockNum];

					if(y == 1)
					{
						padGrid[x][y][blockNum] = ch0;
					}
					else if (y == 2)
					{
						padGrid[x][y][blockNum] = ch1;
					}
					else if (y == 3)
					{
						padGrid[x][y][blockNum] = ch2;
					}
				}
				cout << padGrid[x][y][blockNum];
				ct_file << padGrid[x][y][blockNum];
			}
			cout << endl;
			ct_file << endl;
		}
		cout << endl;
		ct_file << endl;
	}

	cout << "Parity Bit\n";
	ct_file << "Parity Bit\n";
	int parityGrid[4][4][blockCount];		//	new copy of the padded grid, but with parity bits flipped and int type
    for(int blockNum = 0; blockNum<blockCount; blockNum++)
    {
    	for(int x = 0; x<4; x++)
    	{
    		for(int y = 0;y<4; y++)
    		{
    			int hexVal = padGrid[x][y][blockNum];

    			string binString = std::bitset<8>(hexVal).to_string(); //	string to binary

    			int binCount = 0;
			    for(int z = 0; z!= binString.length();z++)
			    {
			    	if (binString[z] == '1')
			    	{
			    		binCount++;
			    	}			    	
			    }

			   	//	count # of 1s, if odd then add parity bit
			    if((binCount % 2) == 1)	//	odd parity
			    {
			    	parityGrid[x][y][blockNum] = hexVal + 128;
			    }
			    else	//	even parity
			    {
			    	
			    	parityGrid[x][y][blockNum] = hexVal;
			    }
			    
			    cout << std::hex << parityGrid[x][y][blockNum] << " ";
			    ct_file << std::hex << parityGrid[x][y][blockNum] << " ";
    		}
    		cout << endl;
    		ct_file << endl;
    	}
    	cout << endl;
    	ct_file << endl;
    }
    cout << std::dec;
	ct_file << std::dec;

    //	initialize constant grid padGrid will be multiplied with
    /*
		[	2	3	1	1	]	
		[	1	2	3	1	]
		[	1	1	2	3	]
		[	3	1	1	2	]
   	*/
    int constGrid[4][4];
    for(int x = 0; x<4; x++)
    {
    	for (int y = 0; y<4; y++)
    	{
    		constGrid[x][y] = 1;

    		if(x == y)
    		{
    			constGrid[x][y] = 2;
    		}
    		else if (x == 3 && y == 0)
    		{
    			constGrid[x][y] = 3;
    		}
    		else if (x != 3)	//	CHECK FOR OUT OF BOUNDS ERROR (SEG FAULT)
    		{
    			constGrid[x][x+1] = 3;
    		}
    		//cout << constGrid[x][y] << " ";
    	}
    	//cout << endl;
    }
    

	// Mix Columns
    cout << "MixColumns" << endl;
    ct_file << "MixColumns" << endl;

    //	matrix multiplication
    int mixGrid[4][4][blockCount];	//	will hold the new elements after matrix multiplication

    for(int blockNum = 0; blockNum<blockCount; blockNum++)
    {
    	for(int x = 0; x<4; x++)
    	{
    		for(int y=0; y<4; y++)
    		{
			    int addArr[4];	//	the 4 elements to add together to form a new element via mixColumns

			    for(int i = 0; i<4; i++)
			    {
			    	addArr[i] = processElement(parityGrid[i][y][blockNum], constGrid[x][i]);
			    }

			    mixGrid[x][y][blockNum] = columnAddition(addArr[0], addArr[1], addArr[2], addArr[3]);
			    cout << std::hex << mixGrid[x][y][blockNum] << " ";

			    ct_file << std::hex << mixGrid[x][y][blockNum] << " ";
    		}
    		ct_file << endl;
    		cout << endl;
    	}
    	ct_file << endl;
    	cout << endl;
    }
    cout << std::dec;	//	reset number display to decimal values

	pt_file.close();
	k_file.close();
	ct_file.close();

	return 0;
}

int processElement(int matrixElement, int n)
{
	//	does x*2 or x*3 operations

	int element = matrixElement;

	string shiftString;
	string bString = std::bitset<8>(element).to_string(); //	string to binary

	int bNum = std::bitset<8>(bString).to_ulong();

	switch(n)
	{
		case 1:							//	multiply by 1
		{
										//	# stays the same, do nothing
		}
			break;

		case 2:							//	multiply by 2
		{
			if(element >= 128)			//	msb = 1 (1000 0000)
			{
				bNum = bNum << 1;		//	left shift
				bNum -= 256;			//	drop MSB
				bNum = bNum ^ 27;		//	XOR with 00011011 = 27
			}
			else if (element < 128)		//	msb = 0
			{
				bNum = bNum << 1;
			}
		}
			break;

		case 3:							//	multiply by 3, x = x << 1 XOR x
		{
			int ogNum = bNum;			//	save original number to be XORed with x << 1

			if(element >= 128)			//	msb = 1 (1000 0000)
			{
				bNum = bNum << 1;		//	left shift
				bNum -= 256;			//	drop MSB
				bNum = bNum ^ 27;		//	XOR with 00011011 = 27
			}
			else if (element < 128)		//	msb = 0
			{
				bNum = bNum << 1;
			}

			//	XOR original x
			bNum = bNum ^ ogNum;
		}
			break;
	}

	shiftString = std::bitset<8>(bNum).to_string(); //	string to binary

	return bNum;
}

int columnAddition(int c0, int c1, int c2, int c3)
{
	//	XOR (adds) all the #s together
	int ans = 0;
	ans = c0 ^ c1;
	ans = ans ^ c2;
	ans = ans ^ c3;
	return ans;
}