#---------------------------------------------------------
# Our first Optimization 'compile' is intended to
# produce a design that will meet hold-time
# under worst-case conditions:
# 		- slowest process corner
# 		- highest operating temperature and lowest Vcc
# 		- expected worst case clock skew
#---------------------------------------------------------
#---------------------------------------------------------
# Set the current design to the top level instance name
# to make sure that you are working on the right design
# at the time of constraint setting and compilation
#---------------------------------------------------------

 current_design $modname

#---------------------------------------------------------
# Set the synthetic library variable to enable use of
# desigware blocks
#---------------------------------------------------------
 set synthetic_library [list dw_foundation.sldb]

#---------------------------------------------------------
# Specify the worst case (slowest) libraries and
# slowest temperature/Vcc conditions
# This would involve setting up the slow library as the
# target and setting the link library to the conctenation
# of the target and the synthetic library
#---------------------------------------------------------
 set target_library NangateOpenCellLibrary_PDKv1_2_v2008_10_slow_nldm.db
 set link_library   [concat  $target_library $synthetic_library]

#---------------------------------------------------------
# Specify a 5000ps clock period with 50% duty cycle
# and a skew of 50ps
#---------------------------------------------------------
 #set CLK_PER  5
 set CLK_SKEW 0.05
 create_clock -name $clkname -period $CLK_PER -waveform "0 [expr $CLK_PER / 2]" $clkname
 set_clock_uncertainty $CLK_SKEW $clkname

#---------------------------------------------------------
# Now set up the 'CONSTRAINTS' on the design:
# 1.  How much of the clock period is lost in the
#     modules connected to it
# 2.  What type of cells are driving the inputs
# 3.  What type of cells and how many (fanout) must it
#     be able to drive
#---------------------------------------------------------

# Following parameters have been modified based on Nangate 45nm library (slow conditional):
# DFF_CKQ, IP_DELAY, DFF_SETUP, OP_DELAY, WIRE_LOAD_EST
# These values are based on simulation. Credited to: Christopher Mineo

#---------------------------------------------------------
# ASSUME being driven by a slowest D-flip-flop
# The DFF cell has a clock-Q delay of 638 ps
# EX: 50um M3 has R of 178.57 Ohms and C of 12.5585fF. 0.69RC = 1.55ps, and wire load
# of 50um M3 is 13fF. Therefore, roughly 20ps wire delay is assumed.
# NOTE: THESE ARE INITIAL ASSUMPTIONS ONLY
#---------------------------------------------------------
#
 set DFF_CKQ 0.638
 set IP_DELAY [expr 0.02 + $DFF_CKQ]
 set_input_delay $IP_DELAY -clock $clkname [remove_from_collection [all_inputs] $clkname]

#---------------------------------------------------------
# ASSUME this module is driving a D-flip-flip
# The DFF cell has a set-up time of 546 ps
# Same wire delay as mentioned above
# NOTE: THESE ARE INITIAL ASSUMPTIONS ONLY
#---------------------------------------------------------
 set DFF_SETUP 0.546
 set OP_DELAY [expr 0.02 + $DFF_SETUP]
 set_output_delay $OP_DELAY -clock $clkname [all_outputs]

#---------------------------------------------------------
# ASSUME being driven by a D-flip-flop
#---------------------------------------------------------

 set DR_CELL_NAME DFFR_X1
 set DR_CELL_PIN  Q

 set_driving_cell -lib_cell "$DR_CELL_NAME" -pin "$DR_CELL_PIN" [remove_from_collection [all_inputs] $clkname]

#---------------------------------------------------------
# ASSUME the worst case output load is
# 4 D-flip-flop (D-inputs) and
# 0.013 units of wiring capacitance
#---------------------------------------------------------
 set PORT_LOAD_CELL  NangateOpenCellLibrary_PDKv1_2_v2008_10_slow_nldm/DFFR_X1/D
 set WIRE_LOAD_EST   0.013
 set FANOUT          4
 set PORT_LOAD [expr $WIRE_LOAD_EST + $FANOUT * [load_of $PORT_LOAD_CELL]]
 set_load $PORT_LOAD [all_outputs]

#---------------------------------------------------------
# Now set the GOALS for the compile
# In most cases you want minimum area, so set the
# goal for maximum area to be 0
#---------------------------------------------------------
 set_max_area 0
#---------------------------------------------------------
# This command prevents feedthroughs from input to output and avoids assign statements
#---------------------------------------------------------
 set_fix_multiple_port_nets -all -buffer_constants [get_designs]

#------------------------------------------------------
# For logic reduction, I want to make one flat design
# This is commented out.  You can add it back in if you are having trouble with hierarchy
#------------------------------------------------------

 ungroup -flatten -all

#---------------------------------------------------------
# check the design before optimization
#---------------------------------------------------------

#---------------------------------------------------------
# check_design checks for consistency of design and issues
# warnings and errors. An error would imply the design is
# not compilable. See > man check_design for more information.
# HOWEVER, often problems pointed out before compile are ignorable
# Only fix things at this point if the fix is obvious
#---------------------------------------------------------
check_design

#---------------------------------------------------------
# link performs check for presence of the design components
# instantiated within the design. It makes sure that all the
# components (either library unit or other designs within the
# heirarchy) are present in the search path and connects all
# of the disparate components logically to the present design
#---------------------------------------------------------
link
