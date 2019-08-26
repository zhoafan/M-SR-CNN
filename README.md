# M-SR-CNN
Contents
Demo Contents
System Requirements
Installation Guide
Instructions for Use
Demo Contents
matlab: matlab package code.
C++: C++ package code.
Example_data: 
            examples.
test_data.
System Requirements

Hardware Requirements
The m-SR-DCNN package requires only a standard computer with enough RAM to support the operations defined by a user. The runtimes below are generated using a computer with the normal specs (4 GB RAM, 1 cores@1.7 GHz).
Software Requirements
The package has been tested on the following systems:
Windows: win7
Before setting up the package, users should install software Matlab R2016b and Microsoft visual C++ 6.0.
Installing Matlab version R2016b on Win7
Download version R2016b and install as following: 
Two modes of installation:
1) standalone:
- Install choosing the option "Use a File Installation Key" and supply the following FIK
	09806-07443-53955-64350-21751-41297
- To install Matlab Production Server,using this
	40236-45817-26714-51426-39281
- Use license_standalone.lic to activate, or make a "licenses" folder in %installdir% and copy license_standalone.lic to it,and run matlab without activation
- after the installation finishes copy the folders to %installdir% to overwriting the originally installed files
2) floating license (network license server):
- Install choosing the option "Use a File Installation Key" and supply the following FIK
	31095-30030-55416-47440-21946-54205
- To install Matlab Production Server,using this
	57726-51709-20682-42954-31195
- Use license_server.lic when asked
- after the installation finishes copy the folders to %installdir% to overwriting the originally installed files.
The installation time is about 2 minutes.

Installing Microsoft visual C++ 6.0 on Win7
Download Microsoft Visual C++ 6.0 Software, and click AUTORUN.EXE. Follow the prompts to install the software step by step without using any serial number.
The installation time is about 5 minutes.

Package dependencies
No Package dependencies are needed.
Instructions for Use
Matlab code
Matlab code for the methods employed in this package can be found in file <matlab> .
C++ code
C++ code for the methods employed in this package can be found in file <C++/GR> .
Taking an image with size of 80*80 as input, the test process and running time are as follows:
Step1: Run demo.m, and produce three image results which saved in <example data/test_data/bic>、<example data/test_data/bic_gray> and <example data/test_data/ sr_gradient>, respectively. (2 minutes)
Step 2 :Open demo.dsp by using Microsoft visual C++ 6.0, and run this demo;
Open the image in file <example data/test_data/bic_gray>; 
Click GR in the Menu Bar and chose image in <example data/test_data/ sr_gradient>;
Finally, we get the resolution enhanced MS image in grayscale, and save it in <example data/test_data/ sr_result>.(2 seconds)
Step 3: Run HR_result.m, and we get the HR MS images. (4 seconds)
