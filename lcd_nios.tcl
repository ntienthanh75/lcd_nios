# Copyright (C) 1991-2013 Altera Corporation
# Your use of Altera Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License 
# Subscription Agreement, Altera MegaCore Function License 
# Agreement, or other applicable license agreement, including, 
# without limitation, that your use is for the sole purpose of 
# programming logic devices manufactured by Altera and sold by 
# Altera or its authorized distributors.  Please refer to the 
# applicable agreement for further details.

# Quartus II: Generate Tcl File for Project
# File: lcd_nios.tcl
# Generated on: Wed Jan 17 23:47:39 2024

# Load Quartus II Tcl Project package
package require ::quartus::project

set need_to_close_project 0
set make_assignments 1

# Check that the right project is open
if {[is_project_open]} {
	if {[string compare $quartus(project) "lcd_nios"]} {
		puts "Project lcd_nios is not open"
		set make_assignments 0
	}
} else {
	# Only open if not already open
	if {[project_exists lcd_nios]} {
		project_open -revision lcd_nios lcd_nios
	} else {
		project_new -revision lcd_nios lcd_nios
	}
	set need_to_close_project 1
}

# Make assignments
if {$make_assignments} {
	set_global_assignment -name FAMILY "Cyclone II"
	set_global_assignment -name DEVICE EP2C5T144C8
	set_global_assignment -name ORIGINAL_QUARTUS_VERSION "13.0 SP1"
	set_global_assignment -name PROJECT_CREATION_TIME_DATE "23:13:18  JANUARY 17, 2024"
	set_global_assignment -name LAST_QUARTUS_VERSION "13.0 SP1"
	set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files
	set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
	set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85
	set_global_assignment -name ERROR_CHECK_FREQUENCY_DIVISOR 1
	set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top
	set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top
	set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top
	set_global_assignment -name QSYS_FILE SOPC.qsys
	set_global_assignment -name QIP_FILE altpll0.qip
	set_global_assignment -name STRATIX_DEVICE_IO_STANDARD "3.3-V LVTTL"
	set_global_assignment -name VHDL_FILE lcd_nios.vhd
	set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top

	# Commit assignments
	export_assignments

	# Close project
	if {$need_to_close_project} {
		project_close
	}
}
