# COMSYS723_Assignment_1_Team12_LCFR
Implementation of the application software of a low-cost frequency relay (LCFR), targeted for use in normal households.

- [miro board](https://miro.com/app/board/uXjVOEI94zc=/)
- [github](https://github.com/uoa-hlin784/COMSYS723_Assignment_1_Team12_LCFR)

## Deadlines

### Week 5, Wednesday (30 March 2022, 11:59 pm)
A) A **conceptual design of your solution** and **implementation plan** as a group taking into account specifics of the implementation platform (DE2 with or without VGA monitor, or simulated solution).
- Please submit your conceptual designs to Canvas via dedicated link.
- Only one submission per group is required.

> - All tasks/ISRs in your design and their functionality 
> - All communication/synchronization paths and what message is sent  
> - The conditions in which each task/ISR execute
> - A3 size.

### Week 7, Monday (11th April 2022, 11:00 am)
B) A **description and discussion of your project** and the **resulting solution in a joint documentation file**. That should include **final design diagram**, which may be based on your conceptual design delivered in Week 5.

### Final Documentation
Instructions for running your assignment on a DE2-115 board  

A description of your entire solution  
- All the tasks, what they do  
- Which tasks are periodic? Non-periodic? Why?  
- A final diagram of your design (highlighting any changes from your initial paper design) 

Justification of your design decisions  

- Why did you split functionality a certain way?  
- Why did you choose a specific mechanism for protecting shared variables?  
- Why did you choose a specific communication mechanism?  
- Why do your tasks have the priorities you’ve assigned?  
- Any issues with the application design  

Evidence that you're engaging with FreeRTOS  

- What features of FreeRTOS did you use
- How do your tasks interact?  
- What are potential problems with multitasking that you've overcome or prevented?  
- Limitations/issues in your design  

An additional page indicating the way in which you partitioned work on the assignment, and the time taken for each part

*!!! Assignment infomation is part of exam !!!*

C) Assignment **source code** submitted in electronic form via Canvas.

> This code will be tested by the assessors by executing your submission on a DE2-115 board, so **clear instruction** how to set up the system and execute code has to be provided in a readme.txt file.

Delivery method for the joint documentation and implementation will be via Canvas. If any error
occurs on the submission day and time, submit a zipped file with the file name that contains your group to
the lecturer (Prof Salcic).

D) Confidential peer review.
- Submit the confidential peer review to Canvas. The review must include a textual description of your reflection on the work of your partner with at **least 10 lines**.
- Non-submission of the peer review results in a penalty of 10% of the total mark for the assignment.

## Instructions

Instructions for running this project on a DE2-115 board.

**Folder Structure:**
- documentation:
  - Brief and system spec document
  - A copy of this report
- system
  - System SRAM object (.sof)
  - SOPC Builder file (.sopcinfo)
- software
  - calculation - depreciated folder
  - customAPI - contains all functions for interacting with board & peripheral components
  - freeRTOS - freeRTOS system files
  - RTOS - contains all custom define tasks and global variable
  - Main.c

Program a de2-115 board with the .sof file and build an empty eclipse project with the .sopcinfo. Move all folders under software into the software folder of the project. Set the 1u timer as the timestamp timer in the BSP editor. Compile and run with a VGA display and p/s2 keyboard connection. If the numerical displays of frequency and rate of changes are unreadable, the rate at which the "snapshots" are updated may be changed in `service_VGA.h`.

**P/s2 Keyboard Input commands**
All inputs are inputted and then registered via ENTER key. Ensure ENTER is pressed to clear previous input before inputting a new command. After entering one of the threshold setting commands (and have registered it) the next input will expect a decimal number and set the threshold value to that input. Any invalid input will not change the threshold value.

**Available Inputs (case-sensitive)**
- ENTER (key) - register command and clear carriage for new input.
- freq - began setting frequency threshold. Next set of inputs determines the value.
- rate - began setting the rate of change threshold. Next set of inputs determines the value.

**Example:**
freq + ENTER + 49.6 + ENTER - will set the frequency threshold value to 49.6Hz

