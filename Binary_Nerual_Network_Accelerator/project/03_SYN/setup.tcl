#------------------------------------------------------------
#
# Basic Synthesis Script (TCL format)
#
# Revision History
#   1/15/03  : Author Shane T. Gehring - from class example
#   2/09/07  : Author Zhengtao Yu      - from class example
#   12/14/07 : Author Ravi Jenkal      - updated to 180 nm & tcl
#   10/7/20  : P Franzon - Project specific script
#   10/22/21 : Andrew Li - Project specific script
#------------------------------------------------------------
# ignore warning
# suppress_message  { OPT-106,LINT-1,LINT-28 }

# setup name of the clock in your design.
set clkname clk
#set CLK_PER 2.432
#set CLK_PER 2.44

# set variable "modname" to the name of topmost module in design
set modname MyDesign

# set variable "RTL_DIR" to the HDL directory w.r.t synthesis directory
set timestamp "[clock format [clock seconds] -format %b%d_%H%M]"
set RTL_DIR     ../02_RTL
set OUT_DIR     ./Netlist
set RPT_DIR     ./Report_${CLK_PER}_${timestamp}
file mkdir $RPT_DIR
file mkdir $OUT_DIR

# set variable "type" to a name that distinguishes this synthesis run
set type tut1

#set the number of digits to be used for delay results
set report_default_significant_digits 4
