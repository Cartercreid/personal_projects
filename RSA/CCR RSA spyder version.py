# -*- coding: utf-8 -*-
"""
Created on Fri Oct 19 17:12:38 2018

@author: carte
"""
"""
Convert_Text takes in a string and returns a list of integers using ord()
The list of integers corresponds to the characters of the string
"""

def Convert_Text(_string):
    integer_list = []
    for i in range(0, len(_string)):
        integer_list.append(ord(_string[i]))    #ord() gets the number for a letter and then this adds it to the list.
    return integer_list

"""Convert_Num takes a list of numbers, and translates that to a string using the inverse of ord(), which is chr()
The integer list is the decoded version, so items should be ~3 digits
"""
def Convert_Num(_list):
    text_string = ''
    for i in range(0, len(_list)):
        text_string = text_string + chr(_list[i])
    return text_string
"""
Base_2_expansion takes an integer, and converts it into binary or base 2
"""
def Base_2_expansion(n): #n is a positive integer
    a = []   #a is an initially empty list to store the binary number
    while n != 0:           #when q (the remainder) is zero, there are no further parts to add to the binary list
        a.append(n % 2)    #add the remainder of q divided by 2 to the end of the list
        n = n // 2         #q = integer (floor) division of 2
    a.reverse()            #the binary list is in reverse order, as higher values are added at the end, an insert method earlier would remove this step
    return a
"""
FME - fast modular exponentiation
takes a number (b) which will be raised to a power expressed as a binary list modulo m
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
IT IS EXTREMELY IMPORTANT THAT THE BINARY_NUMBER_AS_LIST BE REVERSED PRIOR TO CALLING THIS FUNCITON
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
"""
def FME(b, binary_number_as_list, m):
    x = 1                           #Iinitalize x as 1
    k = len(binary_number_as_list)  #k is number of binary digits
    power = b % m                   #
    for i in range (0, (k)):
        if binary_number_as_list[i] == 1:  
            x = (x * power) % m
        power = (power * power) % m 
    return x
"""
div_alg takes two integer arguments and divids the first by the second
div_alg returns the quotition of a / d and the remainder
This uses a brute force method of subtracting d from a over a loop and keeping track of how many times d is subtracted from a
"""
def div_alg(a,d): #a is an intger (number to be divided, and d is a positive integer (quotient)
    q = 0         #q is the quotient
    if a < 0:     #in case a negative is being divided by another number
        r = -1 * a    
    else:
        r = a
    while r >= d:   #while r is bigger than d, subtract d from r
        r=r-d
        q = q+1     # increase the counter by 1
    if a < 0 and r >0:          #if a was less than zero, make adjustments to account for negative division
        r = d-r
        q = -1 * (q + 1)
        
    return (q, r) # q = a divided by d. r is a mod d in this case
"""
Euclidean_alg is a simple euclidean algorithym for finding the gcd between two numbers
This function is used for checking wheter two numbers are relatively prime for key generation
"""
def Euclidean_alg(a,b):
    x = a
    y = b
    while y != 0:
        r = x % y        #this is the form of the euclidean algorithm
        x = y            #shift the identities by one
        y = r
    return x             #x is the gcd of the two arguments
"""
extended_euclidean_alg takes two arguments, m,n and returns the bezout coefficients for the first argument
"""
def extended_euclidean_alg(m,n):  
    (s1,t1) = (1,0)    #These are setting up variables to keep track of the bezout coefficients while running the euclidean algorithm
    (s2,t2) = (0,1)
    while n > 0:
        k = m % n
        q = m // n       
        m = n            #up to this point it is the standard euclidean algorithm, of writing larger number = a*smaller number + remainder
        n = k            #and then repeating the process for the next line with the new larger number = a*smaller number + remainder
        (s1a,t1a) = (s2,t2)                #
        (s2a,t2a) = (s1-q*s2, t1 - q*t2)   # These steps here are keeping track of the bezout coefficients as the euclidean algorithm runs
        (s1,t1) = (s1a, t1a)               #
        (s2,t2) = (s2a,t2a)                #
    #return (m ,s1 ,t1)        #This would return the gcd and both bezout coefficients, but they were not needed for this application when calling this function
    return(t1)


"""
This function finds the public key e from p and q
It returns n. n is the product of p and q, however p and q are arguments
For this function because e must be congruent to p-1 * q-1
"""
def Find_Public_Key_e(p, q):
    n = p * q
    pq_minus_one_product = (p-1) * (q-1)
            #now need to find e, e is any number that is relatively prime to (p-1)(q-1)
    e = 2 #using a brute force method of finding a key e such that e is relatively prime to p-1 q-1, starting at 2 to avoid 1
    gcd_check = 0     #initializing the gcd_check to be not one
    while (gcd_check != 1):                    #run the loop until e is relatively prime to p-1 * q-1
        e = e + 1
        gcd_check = Euclidean_alg(e, pq_minus_one_product)  #Euclidean_alg returns the gcd, if gcd_chek == 1, then the loop will terminate
        if e == p or e == q:                                #e should not equal p or q
            gcd_check = 2                                   #if e equals p or q and still is relatively prime to p-1 * q-1, set the gcd to anything other than 1 and keep going
    return (e, n)           #return the publick key component e, and n


"""
Find_Private_Key_d takes two arguments, e, and n_prime : the product of p-1 * q -1
"""
def Find_Private_Key_d(e, n_prime):
    d = extended_euclidean_alg(n_prime, e) #the Bezout coefficient assoceated with e will give us a modular inverse such that d*e mod p-1 *q-1 = 1
    while d < 0:    #if the bezout coefficient is negative, we can find another d by adding (p-1)*(q-1) to the original key, d until d is positive
        d = d + (n_prime) 
    return d       #return the "private" key

"""
Encode() takes three arguments, n, e, message. get it? Any message?
Anyway, this function takes message (string) and converts it into a number list.
It uses convert_Text() to do so and it also converts the number e into a binary representation of e to be uses with fast modular exponentiation
"""
def Encode(n, e, message):    
    cipher_text = []         # initialize ciper_text as a list
    message_as_number_list = Convert_Text(message)   #convert message into a number list
    base_2_e = Base_2_expansion(e)                  #convert e into a binary representation
    base_2_e.reverse()          #this is extremely important as the binary number list needs to be read from smallest to largest
    for i in range(0, len(message_as_number_list)):    #for loop to go through the entire list
        cipher_text.append(((FME(message_as_number_list[i], base_2_e, n))))   #cipher_text is the encoded integer list, it is coputing each item with: Messge^e mod n using fast modular exponentiation
    return cipher_text    #return an encoded integer list


"""
Decode() function takes 3 arguments, n, d and cipher text. n is part of the public key, p * q and d is the private key
RSA theory (and practice) says that an encrypted message (from M^e mod n) raised to the d mod n will equal the original message. 
This function calls several other functions in it, base_2_expansion for converting d to a binary number, and covert_Num, converting
an integer list into a character string and fast modular exponentiation
"""
def Decode(n, d, cipher_text):
    message = ''    #initialize a string variable
    decoded_number_list = []   #initialize a decoded number list
    base_2_d = [] #initialize a base 2 d list for fast modular exponentiation later
    base_2_d = Base_2_expansion(d) 
    base_2_d.reverse()    #This is extremely important as the binary number needs to be read in opposite order during fast modular exponentiation
    for i in range(0, len(cipher_text)):
        decoded_number_list.append((FME(cipher_text[i], base_2_d, n)))
        #decoded_number_list.append((cipher_text[i] ** d) % n)    This line does the exact thing the previous line does, just theoretically in a longer run time
    message = Convert_Num(decoded_number_list) #once the decoded number list is generated, plug it into the convert numbers to strings function and return the final message
    return message

"""
Convert_string_list_integers() converts a list of strings to a list of integers
it takes one argument, the list of strings and returns a list of integers
"""
def convert_string_list_integers(string_list):
    for i in range(0,(len(string_list))):
        string_list[i] = int(string_list[i])
    return string_list


"""
factor_n takes one argument, n, and attempts to factor it by brute force by attempting to divide
n by every number >=2. If it cannot be factored, it returns -1, -1 (for convenience of downstream if statements)
"""
def factor_n(n):
    counter = 3
    if n % 2 == 0:
        return (2, n //2)
    while counter < n** (0.5):  #only have to go to the square root of n to find the lowest prime factor.
        (factor,remainder_test) = div_alg(n,counter)    #test to see if n is divisible by the current counter
        if remainder_test == 0:                        #if there is no remainder from the previous division, then it is factored
            return (factor, counter)
        counter = counter + 2         #increase the counter and continue
    return (-1,-1)             #return -1, -1 to indicate a failure, the -1 is for convenience in downstream if statements


"""
The pollard_rho() function takes 1 argument, though its seed can be altered if needed.
This is a really neat algorithm for factoring large composite numbers.
This is my adaptation of the code presented on http://www.cs.colorado.edu/~srirams/courses/csci2824-spr14/pollardsRho.html

"""
def pollard_rho(n, seed=2,):
    x, y, d = seed, seed, 1 #set x and y to 2 (cannot be 1 because 1 will provide useless factorization results)
    while d == 1:
        x = ( x**2 +1 ) % n #x increases at a certain rate
        y = ((y**2 + 1)**2 +1) % n   #y increases at twice the rate of x they are both mod n though, so if they ever exceed n, it repeats sort of
        d = Euclidean_alg((x - y) % n, n)    #d is the GCD of x-y mod n and n. Because n is the product of 2 prime numbers, its gcd will either be 1, or one of the two prime factors 
    if d == n:    #if the GCD of n and some number is n, then something went terribly wrong
        return -1
    else:
        return d   #return the found factor, the other factor will be solved for easilly

"""
code_break() This function takes two arguments, e and n. the components of someones public key
It factors n, such that we can find p and q, and from there we can find private key d, or at least a private key d that works to decrypt the encrypted message.

"""
def code_break (e,n):
    if n > 10000000:     #if n is large, then use the more error prone, but more time efficient pollard_rho algorithm. 
        factor_one = pollard_rho(n)   #pollard_rho gives us one of the factors of n
        factor_two = n // factor_one   #I use integer division, because I'm lazy and otherwise factor_two is a float
    else:
        (factor_one,factor_two) = factor_n(n) #This is my own brute force method for factoring large composite numbers
        
    if factor_one == -1:             #-1 indicates a failure to factor
        print("Cannot factor")
        return (-1)
    else:
        print(n, "is the product of", factor_one , factor_two) 
        d = Find_Private_Key_d(e, ((factor_one - 1)*(factor_two - 1)))    #generate a private key form p and q
        return d   #return the not so private key

"""
Main function. This function mostly outlines the order of which steps to do based on user input.
4 choices, generate a key, encode a message, decode a message, and break a public key

"""
def main():
    print("Welcome to CCR's RSA, would you like to:")                         
    
    while True:
        print("1. Make Specific Keys - if you have a favorite pair of primes")                  #Pretty much the home menue,
        print("2. Encode - using a public key")                                                 #I put it in a loop for additional functionality
        print("3. Decode - you will need a coded message, and public and private keys")        
        print("4. Beak - you will need a public key")
        input_choice = input("please enter 1 2 3 4 or any other key to quit")                   
        
        
        
        if input_choice == '1':
            p = int(input("please enter your first prime number (large values reccomended)"))
            q = int(input("please enter your second prime number (large values recomended)"))
            (e,n) = Find_Public_Key_e(p,q)    #find public key using p and q
            d = Find_Private_Key_d(e,((p-1)*(q-1)))      #find the private key using e and whatever (p-1)*(q-1) is
            print("Your public key is:" , e , "," , n)
            print("And your private key is:", d )
            
            
            
        elif input_choice == '2':
            e = int(input("please enter the e of the public key"))
            n = int(input("Please enter n from your public key"))
            message = input("Okay, what message would you like me to encode?")
            cipher_text = Encode(n,e, message)  #call the Encode function - message(as an integer list)^e mod n
            print("Beep Boop, your secret message now reads:", cipher_text )  #cipher_text is an encoded integer list
            
            
        elif input_choice == '3':
            e = int(input("please enter e from your public key"))
            n = int(input("please enter n from your public key"))
            d = int(input("please enter d from your private key"))
            coded_message = input("please enter your coded message")   #despite coded_message being numbers separated by commas, its class is a string
            coded_message_list = []                                    #initialize a new list to contain the coded_message
            coded_message_list = coded_message.split(',') #= all_data[i].split(',')    #split the message by commas
            coded_message_int_list = convert_string_list_integers(coded_message_list)     #convert the list to integers
            decoded_message = Decode(n, d, coded_message_int_list)                        #call Decode function, takes n and d and the integer list of coded numbers and yields a string
            print("Your decoded message is:", decoded_message)                           #print the decoded message
            
            
        elif input_choice == '4':
            public_key_to_break_e = int(input("please enter e from the public key"))
            public_key_to_break_n = int(input("please enter n from the public key"))
            public_key_to_break_d = code_break(public_key_to_break_e , public_key_to_break_n)  #run the public key through the code_break function, generating their private key (or an equivalent)
            if public_key_to_break_d == -1:     #if result is -1, then the attempt has failed. Recall that during the Find_Private_Key_d() function we make sure d is positive
                print("unable to factor")
            else:
                print("private key d is" , public_key_to_break_d)    #to be used for bragging rights
                
        else:      #if any other keys are entered other than 1-4 during menue input, exit program 
            break
    
    return    
if __name__== '__main__': #run main
    main()
