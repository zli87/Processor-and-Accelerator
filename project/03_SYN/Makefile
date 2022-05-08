FOO := "set CLK_PER 2.43"
CLK = "2.43"
NUMBERS = "1" "2" "3" "4" "5" "6" "7" "8" "9"

doit:
#$(foreach var,$(NUMBERS), echo $(addprefix $(FOO),$(var));)
		rm -rf area_total_cell.rpt
		$(foreach var,$(NUMBERS),dc_shell -f syn.tcl -x $(addprefix $(FOO),$(var)) -64bit -output_log_file dc_shell.log; )
		mv area_total_cell.rpt area_total_cell6_$(CLK).txt
		cat area_total_cell6_$(CLK).txt
		cp ../02_RTL/MyDesign.v ./MyDesign_$(CLK).v

