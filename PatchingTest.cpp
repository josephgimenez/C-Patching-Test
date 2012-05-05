#include <stdio.h> 
#include <stdlib.h>
#include <conio.h>
#include <string.h>
int main() 
{ 
		FILE *stream = NULL;
	int offsetDATAADDR = 0x020D;
	int offsetCODESIZE = 0x01E9;
	int offsetJNZ = 0x040F;
	int offsetPUSH = 0x0411;
	int offsetCALL = 0x0416;
	int offsetSTRINGASCII = 0x0CEC;
	int offsetSTRING = 0x0BF8;
	int offsetPRINTF = 0x0C04;

	char patchJNZ[] = { 0x90, 0x90};
	char patchCODESIZE[] = { 0x09 };
	char patchDATAADDR[] = { 0x30 };
	char patchPUSH[] = { 0xE9, 0xE2, 0x07, 0x00, 0x00 };
	char patchCALL[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	char patchSTRING[] = { 0x68, 0xEC, 0x20, 0x40, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	char patchSTRINGASCII[] = { 0x48, 0x65, 0x79, 0x20, 0x73, 0x65, 0x78, 0x79, 0x2E, 0x00};
	char patchPRINTF[] = {  0xFF, 0x15, 0xA0, 0x20, 0x40, 0x00, 0xE9, 0X07, 0XF8, 0XFF, 0XFF, 0X90, 0X90 };

	fopen_s(&stream,"blank.exe","r+");

	if (stream != NULL)
		printf("Opened c:\\blank.exe\n");
	else
	{
		printf("Error opening blank.exe...");
		return -1;
	}

	printf("Patching .text size in PE header\n");
	fseek(stream, offsetCODESIZE, SEEK_SET);
	fwrite(patchCODESIZE, 1, 1, stream);
	//fseek(stream, offsetDATAADDR, SEEK_SET);
	//fwrite(patchDATAADDR, 1, 1, stream);
	printf("Patching JNZ\n");
	fseek( stream, offsetJNZ , SEEK_SET);
	fwrite(patchJNZ,1,0x2,stream);
	
	printf("Patching push and making it a jmp\n");
	fseek( stream, offsetPUSH , SEEK_SET);
	fwrite(patchPUSH, 1, 0x5, stream);
	printf("NOP'ing call\n");
	fseek (stream, offsetCALL, SEEK_SET);
	fwrite(patchCALL, 1, 0x6, stream);
	printf("Adding our push for our string.\n");
	fseek(stream, offsetSTRING, SEEK_SET);
	fwrite(patchSTRING, 1, 0xC, stream);
	printf("Adding our own string to be pushed\n");
	fseek(stream, offsetSTRINGASCII, SEEK_SET);
	fwrite(patchSTRINGASCII, 1, 0xA, stream);
	
	printf("calling address of printf() function\n");
	fseek(stream, offsetPRINTF, SEEK_SET);
	fwrite(patchPRINTF, 1, 0xD, stream);
	
	fclose(stream);


	return 0;
}