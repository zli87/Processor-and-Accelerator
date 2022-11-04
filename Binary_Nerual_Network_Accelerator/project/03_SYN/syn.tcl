#source script1Project.tcl
#source script2Project.tcl
source setup.tcl
source read.tcl
source Constraints.tcl
source CompileAnalyze.tcl
exec cat {*}[glob $RPT_DIR/*.rpt] | grep "slack"
exec echo ${CLK_PER} >> $RPT_DIR/clk_${CLK_PER}_slacks_area.rpt
exec cat {*}[glob $RPT_DIR/*.rpt] | grep "slack" >> $RPT_DIR/clk_${CLK_PER}_slacks_area.rpt
exec cat {*}[glob $RPT_DIR/*.rpt] | grep "Total cell area" >> $RPT_DIR/clk_${CLK_PER}_slacks_area.rpt

exec cat $RPT_DIR/clk_${CLK_PER}_slacks_area.rpt >> ./area_total_cell.rpt
exit
