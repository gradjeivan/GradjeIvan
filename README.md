# GradjeIvan
 
A set of applications and libraries for texas Instruments programmable calculators

## Table of contents
* [Introduction](#introduction)
* [Technologies](#technologies)

## Introduction
GradjeIvan is a set of applications for the Texas Instruments TI-89 and TI-Voyage calculators, created from the author's desire to facilitate uniform (boring) calculations related to problems in the field of statics, concrete and other fields of construction.

The purpose of applications is not to provide ready-made solutions to tasks, but to be a tool for solving them, which implies an understanding of the subject matter. By using these applications, solving these problems is accelerated and the possibility of errors in the calculation is eliminated.
Therefore, the target group of users are students (construction university). The author himself, Ivan, was a civil engineering student at the time of developing the applications. 
Hence the name of the application: **_GradjeIvan_** (Gradjevina - serbian for Construction).

## Technologies
All code is written in the C programming language, with the help of the [TIGCC C and ASM SDK](http://tigcc.ticalc.org/) (v.0.96 Beta 8), specially made for Texas Instruments calculators. Some functions are fully compatible with the [GCC](http://www.gnu.org/software/gcc/) compiler (v.4.1.2-pre9), and can be used for other platforms as well. Some become GCC compatible with very small intervention. One part of the code is still characteristic only for TIGCC, mainly in part related to TI graphics.
The [TiEmu virtual calculator](http://lpg.ticalc.org/prj_tiemu/win32.html) (v.3.02) was used for development and testing, with a debugger that can be connected to the TIGCC SDK. Of course, the final tests were performed on an actual calculator, the [TI-89 Titanium](https://education.ti.com/en/products/calculators/graphing-calculators/ti-89-titanium).

![TIGCC SDK with TiEmu]()