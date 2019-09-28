# -*- coding: utf-8 -*-
"""
Created on Tue Sep  3 17:50:19 2019

@author: Carter
This program is meant to aid Rossette Erica Reid in her duties for the MSEE program.
"""

import pandas as pd
import os
import time

"""clears the screen on linux or windows"""
def clear():
    os.system('cls' if os.name == 'nt' else 'clear')
    
"""read function simply opens an excel file specific to the MSEE program and returns a tuple (int, dataframe)
if the int is 1, then improt was successful, if the int is 0 then it failed"""
def read(file_name):
    try:
        c_pd = pd.read_excel(file_name, index_col=0)
        return (1,c_pd);
    except:
        print("error reading file: ", file_name)
        print("make sure the file is named and formatted correctly. Read the instructions if unclear")
        return (0,0);
    
""" Write writes additional pages to the excel output file. It creates several dataframes, a country count dataframe, an area dataframe, a country by area dataframe,
and an area by country dataframe. It calculates and writes these in a try catch statement so that the user can troubleshoot should something fail without needing to restart
the program"""    
def write(c_pd):
    try:
        country_df = c_pd['Country'].value_counts()
        area_df = c_pd['area'].value_counts()
        country_area_df = c_pd.groupby(['Country', 'area']).area.count()
        area_country_df = c_pd.groupby(['area','Country']).Country.count()
        with pd.ExcelWriter('output.xlsx') as writer:  # doctest: +SKIP
            c_pd.to_excel(writer, sheet_name='Master_sheet')
            country_df.to_excel(writer, sheet_name='Counts_by_Country')
            area_df.to_excel(writer, sheet_name='Counts_by_Specialization')
            country_area_df.to_excel(writer, sheet_name='Country_by_Area')
            area_country_df.to_excel(writer, sheet_name='Area_by_Country')
        return 1;
    
    except:
        print("Error writing to file, make sure all versions of output.xlsx are closed")
        return 0;
    
    
"""Menu is where the user can change file names, see the menu etc. Its basically a while true loop that only breaks when reading/writing is successful or user gives up
and quits."""
def menu():
    file_name = 'test_stats.xlsx'
    while(True):
        print('type "help" for help')
        print('type "i" or "I" for instructions on how to prepare the file for data processing')
        print('type "r" or "R" to read the file')
        print('type "fin" or "FIN" to change file input name')
        print('type "q" or "Q" to quit')
        user_in = input("Please enter you're input>")
        clear()
        
        if(user_in=="help"):
            #TODO
            #print("you have selected help, but I have no help for you now")
            print("If you are having trouble reading a file, be sure to double check the file is in the same folder as this program.")
            print("Double check the spelling of the file name and that it contains the .xlsx tag")
            print('Please also double check that column O has been renamed to "area" and that the first row has been removed. Cell 1 A should NOT be "Declaration Form" ')
            print("If you are having trouble writing to the file, double check that you do not already have a copy of output.xlsx open, as this will cause a problem")
            print("Simply close output.xlsx and retry")
            
            
        elif(user_in=="FIN" or user_in=="fin"):
            print("I knew a shark named Fin once. Anyway, what is the new file name? - Be sure to include the .xlsx")
            file_name = input("New File Name>")
            
        elif(user_in=="i" or user_in=="I"):
            print("Please make sure the input file is named correctly - currently needs to be named 'test_stats.xlsx' ")
            print("Please make sure the first row is removed, so that row 1 column 1 is Serial, row 1 column 2 is SID etc.")
            #print("Please change column N to be 'Country'")
            print("Please change column O to 'area'")
            print("It is very important to change these as if they are not correct and the right case, the program will not function properly")
            
        elif(user_in=="r" or user_in=="R"):
            checkpoint_1,c_pd = read(file_name)
            
            if(checkpoint_1 != 0):
                print("data import successfull!")
                checkpoint_2 = write(c_pd)
                
                if(checkpoint_2==1):
                    print("Read/write successful")
                    print_stupid()
                    print("Closing now")
                    print("Goodbye!")
                    time.sleep(2)
                    break;
                    
        elif(user_in=="q" or user_in=="Q"):
            print("Goodbye!")
            break;
            
        else:
            print("unrecognized command")
                
def print_stupid():
    print("────────────██████████──████────")
    print("────────████▒▒░░░░░░░░██▒▒░░██──")
    print("──────██▒▒░░░░██░░██░░░░██░░░░██")
    print("────██▒▒░░░░░░██░░██░░░░░░▒▒░░██")
    print("────██░░░░░░░░██░░██░░░░░░▒▒▒▒██")
    print("──██░░░░░░▒▒▒▒░░░░░░▒▒▒▒░░░░▒▒██")
    print("██▒▒░░░░░░░░░░░░██░░░░░░░░░░░░██")
    print("██░░░░▒▒░░░░░░░░██░░░░░░░░░░▒▒██")
    print("██░░░░▒▒░░░░░░░░░░░░░░░░░░░░██──")
    print("──██████░░░░░░░░░░░░░░░░░░▒▒██──")
    print("██▒▒▒▒▒▒██░░░░░░░░░░░░░░░░▒▒██──")
    print("██▒▒▒▒▒▒▒▒██░░░░░░░░░░░░▒▒██────")
    print("██▒▒▒▒▒▒▒▒██░░░░░░░░░░▒▒████────")
    print("──██▒▒▒▒▒▒▒▒██▒▒▒▒▒▒████▒▒▒▒██──")
    print("────██▒▒▒▒██████████▒▒▒▒▒▒▒▒▒▒██")
    print("──────██████──────████████████──﻿")

def main():
    print("This program is inteded solely for the MSEE online graduate program.")
    print("It is inteded to help facilitate reporting the demographic data of interested degree seeking candidates.")
    menu()


if __name__== '__main__': #run main
    main()
