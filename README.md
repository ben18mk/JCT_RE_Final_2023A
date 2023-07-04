# JCT Reverse Engineering Final Exam 2023A Solution
JCT Exam Solution

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
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/62e491e5-dee6-4f92-9d68-f2a58c0094f9)

By opening the executable in CFF Explorer, it is noticeable that the executable has TLS.</br>
Therefore, I suspect the message I am getting is originated from the TLS.</br>
Let’s open IDA to statically analyze the executable and have a closer look at the TLS callback.

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/08bf444f-09e1-49b2-8750-04fbe743207a)

It is noticeable that the TLS callback checks if the file *C:\temp\test.txt exists*.</br>
If it does, the program will not print the error message and halt like it did, but it will print the initial success message and start running the main function.</br>

Therefore, in order to pass this check, I simply created a file named test.txt under *C:\temp*

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a0e2e565-1d7f-4c76-b274-22fab60c76f7)

Note: The loading of the string *“abcd…0123…”* to the memory as shown in the TLS callback is a prep for the main function.

## 2
By dynamically analyzing the main function of the program, first I understand that the length of the username must be bigger or equal 6.

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/ec2abb61-238a-4a43-94c8-f986db753391)

Second, the password is constructed based on the username and the “abcd…0123…” that we saw in the TLS.

### How is it done?
The program loops over the username and calculates the modulo of the division of the char’s ascii value with *0x24* (36 in decimal) which will be the index of where to grab a char from the *“abcd…0123…”* string.</br>
Therefore, the password will be constructed in a way, that in each iteration, the loop will add to it a new char from the *“abcd…0123…”* string based on the calculated index (*username[i]* % 36).</br>
This is the body of the loop that performs this action:

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/cb8de9f8-8ccf-49aa-9311-06197d37f175)

So, let’s give it a try.</br>
I will use the username “Benjamin Mamistvalov”</br>
This is the password that will be generated as explained above:

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/8fec6d9f-d79a-4a3d-8c3e-c2e247a15cdc)

Username = *Benjamin Mamistvalov*</br>
Password = **4xzxmnkmincdahpzoor1**

![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/7cef0d86-3ff6-44d2-aaaa-1f47923905b4)

## 3
In this part I will start by coding the main and after the main is done I will code the functions.</br>
By statically analyzing the program, I understand that the main works as follows:
### Main
1.	Assigns destIdx with the value 0 and then calls the function *inputUsername()* to allow the user to input his username.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/4ba32811-5a34-4293-9fbb-1217c5feb68c)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/3107aa11-28ff-48f2-b677-7e1091138ce2)

2.	If the length of the username is less than 6, the program exits.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/d8c54c63-9886-4afc-b448-e820e0654cb4)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/e0c24516-33b7-4915-9bb7-1ecae9a077de)

3.	Else, it loops over the username and generates the password as shown above in [Section #2](https://github.com/ben18mk/JCT_RE_Final_2023A/edit/main/README.md#2)</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/112c7484-0502-41b1-bed1-62fb57c3d55c)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/077c3eeb-5fa7-4db6-81e4-f2bf8d7219b3)

4.	After the loop, the program assigns the length of the username into a new variable named *bufferLength*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/6774b779-cca7-4799-b43d-acd80e616d32)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/23fac9eb-abe1-4cbc-b71a-68e1659dfe9d)


5.	Compiler-added check, therefore I will not add it to the code.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/c0c526d2-2510-43df-8a04-38beee0ce6f1)

6.	Add null char to the generated password.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/ca10075a-e483-4f7d-b993-9b8a161d37f3)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/3470c6ba-414f-4afb-bf2e-acee2706392a)

7.	Calls the function *inputPassword()* to allow the user to input his password.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/7a245296-7e31-4476-a49d-eed39a964c27)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/0be2417c-ff61-495e-bfc6-2f1c15aa9206)

8.	Calls the function *checkPassword()* in order to check if the user’s password matches the generated password by the program. This function will print a message accordingly.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/f1f1f731-a53d-483e-bc4d-4cbbe883e1a3)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a52a80d5-d77f-426d-b028-6bb6b2794434)

### inputUsername()
No parameters.</br>
1.	Prints the string *“Enter username\n”*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/7a50669d-804c-45a9-b1dd-8500655d21e0)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/36448c39-4d34-4cb7-883b-7cac5c36095d)

2.	Inputs the username to *Username*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a18cca49-fd75-45dd-b38e-2e6b59ba5138)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/6666a200-c459-477e-bc97-5a833456c261)

3.	Gets the length of the username without *‘\n’*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/e95a5af1-e0c8-4260-95a8-a9bf3952ddaa)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/f1dfbfa6-32b8-4073-af3e-041db5e97e44)

4.	Replaces *‘\n’* with null char.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/16cb3c89-5364-4546-8914-23c84bc81af3)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/26ef2db1-c506-4dd4-99d6-a3a7d4afb733)

### inputPassword()
No parameters.
1.	Prints the string *“Enter password\n”*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/ff3bd1af-29c5-4790-96f0-921d47282f07)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/bd8374e4-8257-468f-bb26-1f309e6e8be5)

2.	Inputs the password to *UserPassword*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/7030c4da-58e2-464d-9447-dc18d243e8ff)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/ab3566d5-b8e0-4dbc-b14c-992025d04533)

3.	Gets the length of the password without *‘\n’*.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/dd2175f4-b036-46e9-855d-429a00927baf)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/369a5d22-2f2f-4710-9f92-8f95008b78fc)

4.	Replaces *‘\n’* with null char.</br>
**asm:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/306927d7-a292-4fb0-b40f-eb6e51c4afda)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a8d106be-56ff-4183-b8c8-cdf6ce20fdb4)

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
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/a693ef95-d353-4123-89d6-05306d4dfffc)</br>
**C:**</br>
![image](https://github.com/ben18mk/JCT_RE_Final_2023A/assets/56043333/f8cc9a26-f806-4899-aa01-cbcaf7dc0d47)

#### Notes
1.	I defined *BUFFER_SIZE* to be 100
2.	*Destination*, *Username*, *GeneratedPassword* and *UserPassword* are global variables because as seen in the assembly, we could access them from everywhere in the code and because they are defined in the memory.
