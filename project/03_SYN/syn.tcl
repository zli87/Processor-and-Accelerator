source script1Project.tcl
source script2Project.tcl
exec cat {*}[glob $RPT_DIR/*.rpt] | grep "slack"
exec cat {*}[glob $RPT_DIR/*.rpt] | grep "slack" >> $RPT_DIR/clk_${CLK_PER}_slacks.rpt
exit
