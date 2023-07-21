# Autopac

This is a simple menu based C program for building and testing RPM packages. 

## Prerequisites
 - C compiler ( for example GCC ) 


##Getting started
1. Clone the repository:
   ```
   git clone https://github.com/iamianmullins/autopac.git
   ```

2. Compile the program:
   ```
   gcc autopac.c -o autopac
   ```


3. Run the program:
   ```
   ./autopac
   ```

4. Using command line options:
   Example
   ```
   ./autopac -n my-package-name -v 0.1 -r 1
   ```

   -n my-package-name : Specifies the package folder (This should match the package name)
   -v 0.1 : Specifies the package version we're building
   -r 1 : Specifies the package release value 
