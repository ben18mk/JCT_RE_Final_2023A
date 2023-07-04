# JCT Reverse Engineering Final Exam 2023A Solution
JCT Exam Solution

Note: The reversed code could be refactored. But the task was to write the code like the original program was coded.

# Write-Up
## Objective
1. Find what is missing which causes the error message and fix it.
2. Find an input that will provide a success message.
3. Write the program in C.

## Topics Covered
<ul>
  <li>CFF Explorer</li>
  <li>Static Analysis with IDA</li>
  <li>Dynamic Analysis with x32/x64 dbg (a.k.a x96dbg)</li>
  <li>C Coding</li>
</ul>

## 1
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/cdb036bb-66e2-4da4-a572-0f2fa345e1af)

By opening the executable in CFF Explorer, it is noticeable that the executable has TLS.</br>
Therefore, I suspect the message I am getting is originated from the TLS.</br>
Let’s open IDA to statically analyze the executable and have a closer look at the TLS callback.

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/b2bdc7f9-e2d3-4617-bdb5-db2feb1bcb25)

It is noticeable that the TLS callback checks if the file *C:\temp\test.txt exists*.</br>
If it does, the program will not print the error message and halt like it did, but it will print the initial success message and start running the main function.</br>

Therefore, in order to pass this check, I simply created a file named test.txt under *C:\temp*

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/16f570b9-b844-4834-b3e9-1f4f7ab36f70)

Note: The loading of the string *“abcd…0123…”* to the memory as shown in the TLS callback is a prep for the main function.

## 2
By dynamically analyzing the main function of the program, first I understand that the length of the username must be bigger or equal 6.

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/3b1da0fd-1340-45fa-bfc7-627306d1193b)

Second, the password is constructed based on the username and the “abcd…0123…” that we saw in the TLS.

### How is it done?
The program loops over the username and calculates the modulo of the division of the char’s ascii value with *0x24* (36 in decimal) which will be the index of where to grab a char from the *“abcd…0123…”* string.</br>
Therefore, the password will be constructed in a way, that in each iteration, the loop will add to it a new char from the *“abcd…0123…”* string based on the calculated index (*username[i]* % 36).</br>
This is the body of the loop that performs this action:

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/b6aa720a-e57e-424a-8f52-6f9631f945d5)

So, let’s give it a try.</br>
I will use the username “Benjamin Mamistvalov”</br>
This is the password that will be generated as explained above:

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/af952400-a12f-411a-afb0-c2bcac20cf81)

Username = *Benjamin Mamistvalov*</br>
Password = **4xzxmnkmincdahpzoor1**

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/6c37c19a-955d-49ab-9d5c-179a247c8033)

## 3
In this part I will start by coding the main and after the main is done I will code the functions.</br>
By statically analyzing the program, I understand that the main works as follows:
### Main
1.	Assigns destIdx with the value 0 and then calls the function *inputUsername()* to allow the user to input his username.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/610b1ca8-4ded-4c5d-aabc-3f156ccd6b2a)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/356c7ac6-e798-44e8-9654-02e70db01d17)

2.	If the length of the username is less than 6, the program exits.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/1b2ffcf6-6b68-4478-a312-09c9d4636b13)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/231b3fdd-0037-46a9-85c4-7cde33ae7531)

3.	Else, it loops over the username and generates the password as shown above in [Section #2](#2)</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/b2160de7-ceb1-429d-bd3d-69dedc2a977e)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/2fa14421-489c-4a9e-ac16-dd6e127aaecb)

4.	After the loop, the program assigns the length of the username into a new variable named *bufferLength*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/41c4ee2a-5b17-4220-8190-01452e0c48d9)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/01f974dc-9d3b-4071-8dd5-7b25edbaad6a)

5.	Compiler-added check, therefore I will not add it to the code.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/8cb4fd82-2153-4768-9d41-893590ec226e)

6.	Add null char to the generated password.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/8c24883d-5705-41bd-9bee-4b39ca6ee11f)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/60f9b05d-66ad-411f-b96a-af56d7d1bc33)

7.	Calls the function *inputPassword()* to allow the user to input his password.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/8ba37213-ad6c-4bd7-981d-3408e4645c57)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/d3465e8a-1ede-4937-94f5-049ace8a0536)

8.	Calls the function *checkPassword()* in order to check if the user’s password matches the generated password by the program. This function will print a message accordingly.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/36e2fa1b-9602-4b5c-92be-c6c9fcd3958e)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/e0b10520-9f8f-4827-94fb-7ddb922f3435)

### inputUsername()
No parameters.</br>
1.	Prints the string *“Enter username\n”*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/df286855-9aaf-4734-83b9-e451720d25e2)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/c33a0b18-7f04-4817-bb9c-31fc572864b5)

2.	Inputs the username to *Username*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/cf7c26cf-8f0d-4605-a931-d02b3e8fbfda)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/e0a20023-33f4-4d07-b8d0-de1a9254d4d3)

3.	Gets the length of the username without *‘\n’*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/e8d76340-0356-4c07-bc65-70a38c9c9a97)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/de236ded-6fbb-47e8-99b8-ec81096a5720)

4.	Replaces *‘\n’* with null char.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/b2d9d6c0-8f53-4aba-8379-1df921066c84)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/66e31ac1-e237-44dd-ac7d-a769b1acc1d4)

### inputPassword()
No parameters.
1.	Prints the string *“Enter password\n”*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a9c55105-a5b5-44dd-af5c-1775cc924125)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/34de415b-322e-4854-902d-cc4d0b43d4e3)

2.	Inputs the password to *UserPassword*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/6a62ed32-6458-4354-ac8e-335290321475)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/6eeb4f84-b768-4b91-be9f-3cbc58f7da0c)

3.	Gets the length of the password without *‘\n’*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/c97c0406-cb9b-474e-a213-46f45b826d47)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/26e2436b-6ab6-435f-80e3-51d8e347be06)

4.	Replaces *‘\n’* with null char.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/63b2c0cc-babe-44cf-b931-607da193357d)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/da33b5f4-e450-48d3-bd21-fb577b755254)

### checkPassword()
#### 2 parameters
<ul>
  <li>*password1*</li>
  <li>*password2*</li>
</ul>

1.	Compares between *password1* and *password2*.
2.	If they are not equal, the program prints *“Wrong\n”*.
3.	Else, the program prints *“Ace!\n”*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a726f863-c5be-49bd-a408-a4973d28403d)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/d29d7866-a16e-4ca0-9b16-0df639715d0d)


#### Notes
1.	I defined *BUFFER_SIZE* to be 100
2.	*Destination*, *Username*, *GeneratedPassword* and *UserPassword* are global variables because as seen in the assembly, we could access them from everywhere in the code and because they are defined in the memory.
