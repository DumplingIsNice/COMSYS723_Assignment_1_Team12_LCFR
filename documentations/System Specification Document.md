## Brief
### Off-nominal Frequency Operation
- Overfrequency/underload: power supply in excess.
- Underfrequency/overload: 

**Goal:**
Accurately measures and control load-connection during lowest point of consumption (household).

**Solution:** Low-cost Frequency Relay

## Features
- **Operation:**
	- Measures current freq, rate of change of power network
	- Switch loads on/off
- **Human Interface (Memory mapped devices):**
	- Display above infomation (LED, VGA, LCD etc.)
	- Sets thresholds of above infomation
	- Switchs state of operation (running vs maintainence)

#### Platform
 - Intel (Altera) NIOS II-based SOPC implemented in a Cyclone FPGA on the DE2-115 development board

## Assumptions
### Digital Hardware
Power signal simulated as existing IP block stored in M9K memory.
- No specific hardware is designed.
- Interfacing for HW/SW co-design instead.

## Specs
Mains: 240V 50Hz AC

Concurrent tasks - multitasked with a RT-OS (FreeOS)
- Concurrently operate
- Communicate infomation
- Schedule execution order+priority
- Synchronise operation

## Functional Requirements
### Power Network (Mock) Interfacing
- Read at same freq as data sampling freq with ADC

1) Voltage read.
2) Frequency analyser determines period of signal (T)
3) uP converts freq to period

### Signal Monitor
**Inputs:**
- ADC count

**Measures:**
- Instaneous frequency - lower-bound threshold
- Rate of Change (ROC) - absolute-bound threshold

**Thresholds (configurable at run-time):**
- Highest instaneous frequence
- Highest **absolute ROC**

Any threshold violation will cause system to enter unstable state.

### Load Shedding Service
**Timing Constraints:**
- ***200ms*** response window time (from threshold violation to first load shred)
- ***500ms*** validation window time
	- All changes between stable and unstable state will reset this timer

**Validation:**
- If validation time elapses, and system is still unstable (threshold still violated):
	- Shred next-in-line highest priority load until all loads are shred
	- If all loads are shred, do nothing???
- If system returns to stable, reset validation timer:
	- Connects next-in-line highest priority load until all loads are connected

### Verification Timing
Function to measure timing between threshold violation to first load shred.

- From ISR call to beganning of load shed (end of decision making)

### VGA Controller Task
Displays stored frequency infomation through VGA display

**Display Infomation:**
- For instantaneous frequency AND ROC:
	- **5 most recent LOGGED measurements**
	- **Function:** Max and min time taken
	- **Function:** Average time taken
	- But not current (will need time to update and display for next VGA display cycle)
- Total system up-time (from boot?)

#### Frequency Relay Infomation:
### PS/2 Controller Task (for keyboard)
- Commands to configure measurement thresholds

### System States
#### Unstable
**Entry Condition:**
- Any threshold violation
	- Under-frequency
	- High rate of change (ROC)
- ***200ms*** timing constrain began

**Load Management Function:**
[[#Load Shedding Service]]

When all load is connected (apart from intial entry, before load shredding), return to  stable state.

#### Stable
- Boot/default normal operation.
- No special operation?

### Maintainence
**Entry Condition:**
- Single pushbutton entry/exit
- Disables state tranisition (load management & indication)
	- But not display and measurement?
	- Switchs operate normally

### Loads
- **At least 5 loads (No. 1-5)**
- Lower number (on-board), lower priority  
	- Lowest priority load is shread first

#### LEDS (RED)
- Status of each load (1-5)
- **ON:** Connected
- **Off:** Dis-connected

#### LEDS (Green)
**State Dependency:** Unstable
- Indication of software load shedding
- **ON:** Shred
- **Off:** Connected

#### Switches
**Two (state depenedent) Modes:**
- Manual load switching (dominate switch control)
- Manual switching **limited to on-to-off** during unstable 


## State Controller
Global infomation on the state of system.

---
## Detailed Requirements
### ISR (minmum 2)
2x ISR from each timer (response & validation window)

**Measurement ISR**
- ISR register counting semaphore? The handled by calculation_handler?

## Timer Services (from FreeRTOS)

### Shared Variables
Needs to be mutually excluded (mutex protection).

Memory for 2x5=10 frequency & ROC value
- Single-precision Float?
- Queued? FIFO Array?

**Global Variables**
- State flag (stable, unstable, maintainence)

### Tasks (minimum 3)
- Shred Load should be the highest priority 
- Distinguish sub-routines vs tasks
- Determine data packages and the messages passed from one task to another

- **Task:**
- **Type:** 
	- periodic (what period?)... event driven (what event?)
- **Behaviour:**
	- What does it do?
- **Function Calls:**
	- What function/subroutine it calls?
- **Message passed:**
	- ...to... 
- **Shared Resource Access:**
	- ...with...(task)

### FreeRTOS Features usage (Necessary + Justification)
