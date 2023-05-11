# GradjeIvan
 
A set of applications and libraries for Texas Instruments programmable calculators

## Table of contents
* [Introduction](#introduction)
* [Technologies](#technologies)
* [Applications and libraries](#applications-and-libraries)
    * [Main algorithm](#main-algorithm-description)
    * [Structure of data](#structure-of-data)
        * [Applications](#applications)
        * [Function archives](#function-archives)
        * [Dynamic libraries](#dynamic-libraries)
* [Operation manual](#operation-manual)
## Introduction
GradjeIvan is a set of applications for the Texas Instruments TI-89 and TI-Voyage calculators, created from the author's desire to facilitate uniform (boring) calculations related to problems in the field of statics, concrete and other fields of construction.

The purpose of applications is not to provide ready-made solutions to tasks, but to be a tool for solving them, which implies an understanding of the subject matter. By using these applications, solving these problems is accelerated and the possibility of errors in the calculation is eliminated.
Therefore, the target group of users are students (construction university). The author himself, Ivan, was a civil engineering student at the time of developing the applications. 
Hence the name of the application: **_GradjeIvan_** (Gradjevina - serbian for Construction).

## Technologies
All code is written in the C programming language, with the help of the [TIGCC C and ASM SDK](http://tigcc.ticalc.org/) (v.0.96 Beta 8), specially made for Texas Instruments calculators. Some functions are fully compatible with the [GCC](http://www.gnu.org/software/gcc/) compiler (v.4.1.2-pre9), and can be used for other platforms as well. Some become GCC compatible with very small intervention. One part of the code is still characteristic only for TIGCC, mainly in part related to TI graphics.
The [TiEmu virtual calculator](http://lpg.ticalc.org/prj_tiemu/win32.html) (v.3.02) was used for development and testing, with a debugger that can be connected to the TIGCC SDK. Of course, the final tests were performed on an actual calculator, the [TI-89 Titanium](https://education.ti.com/en/products/calculators/graphing-calculators/ti-89-titanium).

![TIGCC SDK with TiEmu](https://github.com/gradjeivan/GradjeIvan/blob/main/TIGCC%20SDK%20and%20TiEmu.png)

## Applications and libraries
All GradjeIvan applications are based on the same core libraries. Main difference between them is their subject matter (problem to be solved), but they all need to have possibilities of: 
[graphical input](), 
[tabular presentation of data](), 
[matrix calculations](), 
[open/save/export the data from the application to calculator variable(file) so it can be used in normal calculator use mode](),... 
...for which we use the same basic libraries loaded from dedicated .dlls developed to be used only while the functionality is needed. 
TI calculators are limited by its internal memory, and can't hold there all libraries at the same time, which is the main reason for having approach like this.
### Main algorithm description
Basic algorithm for each application, with links to algorithms describing specific functionalities is presented in [Documentation/Main function algorithm.pdf](https://github.com/gradjeivan/GradjeIvan/blob/main/Documentation/Main%20function%20algorithm.pdf). 
Below is the main algorithm, where green rectangle marks a spot from where each functionality is triggered by calling its main function. It actually represents a place in algorithm for each selected menu entry by the user.

![Documentation/Main function algorithm.svg](https://github.com/gradjeivan/GradjeIvan/blob/main/Documentation/Main%20function%20algorithm.svg)

We can list main functionalities of each application by going down through all menu entries. Some menu entries are made available to user only when some conditions are met (usually when user inputs necessary data).
Below is the quick run through menu items available for new file in *Statika* application:
![Statika_meni.gif](https://github.com/gradjeivan/GradjeIvan/blob/main/Documentation/Content/Statika/Help/GRI_PR_01_MENI.gif)

### Structure of data
The core programming code behind all GradjeIvan applications is located within the GradjeIvan API. This set of function archives (*.a) and dynamic libraries (*.dll) deals with the basic elements of each application, such as: graphical environment, menus, dialogs, drawing lines and points in a given scale, matrices, tabular display, etc. .

The working principle can be compared to some kind of primitive object-oriented programming: each archive contains types and functions that deal with one group of related application elements. "Objects" are defined by structural types of variables, and "methods" by functions that have variables of that type as objects of their action. All "objects" of the same type that exist within the application are held in a single doubly linked list of these variables, accessible to each function through a global pointer of that type.

Functions specific to individual applications are located in separate archives or dynamic libraries, in order to be used only within those applications that require these functions to work. Thus, for example, in the case of the GradjeIvan Statics application, such archives deal with the definition of types that describe bars and diagrams, their entry and modification, as well as the calculation that results from the entered data. Those archives are not needed for applications such as e.g. The level line, because it does not deal with that issue. However, basic functions, such as the functions listed above within the GradjeIvan API, are needed, because like most applications, it also needs menus, dialogs, tables, etc.
It is clear from the above that understanding the GradjeIvan API code is a condition for understanding the complete code of any of the GradjeIvan applications.


#### Applications
*   [Statika](https://github.com/gradjeivan/GradjeIvan/blob/main/Aplikacije/Statika/main.c)
*   Beton
*   Linija nivoa


#### Function archives


#### Dynamic libraries


## Operation manual
Currently available version is an incomplete html version in serbian, as a [compiled .chm](https://github.com/gradjeivan/GradjeIvan/raw/main/Documentation/GradjeIvan%20Documentation.chm), but also accessible through [GradjeIvan Documentation.htm](https://github.com/gradjeivan/GradjeIvan/blob/main/Documentation/Content/GradjeIvan%20Documentation.htm#enroll-beta) in Documentation folder. 