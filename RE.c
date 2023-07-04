#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

char Destination[] = "abcdefghijklmnopqrstuvwxyz0123456789";
char Username[BUFFER_SIZE + 1];
char GeneratedPassword[BUFFER_SIZE + 1] = { 0 };
char UserPassword[BUFFER_SIZE + 1] = { 0 };

void inputUsername()
{
	printf("Enter username\n");
	fgets(Username, BUFFER_SIZE, stdin);
	int usernameLength = strcspn(Username, "\n");
	Username[usernameLength] = 0;
}

void inputPassword()
{
	printf("Enter password\n");
	fgets(UserPassword, BUFFER_SIZE, stdin);
	int passwordLength = strcspn(UserPassword, "\n");
	UserPassword[passwordLength] = 0;
}

void checkPassword(char* password1, char* password2)
{
	if (strcmp(password1, password2))
		printf("Wrong\n");
	else
		printf("Ace!\n");
}

int main()
{
	int destIdx = 0;

	inputUsername();
	int usernameLength = strlen(Username);

	if (usernameLength < 6)
		return 0;

	for (int i = 0; i < usernameLength; i++)
	{
		char currentChar = Username[i];
		destIdx += currentChar;
		destIdx %= 36;
		GeneratedPassword[i] = Destination[destIdx];
	}

	int bufferLength = strlen(Username);
	GeneratedPassword[bufferLength] = 0;
	inputPassword();
	checkPassword(UserPassword, GeneratedPassword);

	return 0;
}