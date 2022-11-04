
#---------------------------------------------------------
# Read in Verilog file and map (synthesize) onto a generic
# library.
# MAKE SURE THAT YOU CORRECT ALL WARNINGS THAT APPEAR
# during the execution of the read command are fixed
# or understood to have no impact.
# ALSO CHECK your latch/flip-flop list for unintended
# latches
#---------------------------------------------------------

# read_verilog $RTL_DIR/${modname}.v
if { [catch {read_file -top ${modname} -autoread -verbose $RTL_DIR/${modname}.v }] } {
    puts "Error: Compile Error!"
    exit 1
}
file copy $RTL_DIR/${modname}.v $RPT_DIR/${modname}_${CLK_PER}.v
