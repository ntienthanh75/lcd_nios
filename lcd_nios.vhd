-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

library ieee;
use ieee.std_logic_1164.all;
library altera;
use altera.altera_syn_attributes.all;

entity lcd_nios is
	port
	(
-- {ALTERA_IO_BEGIN} DO NOT REMOVE THIS LINE!

		BA : out std_logic_vector(1 downto 0);
		bidir_port_to_and_from_the_lcd32_data : inout std_logic_vector(15 downto 0);
		buzz : out std_logic_vector(3 downto 0);
		CAS : out std_logic;
		CKE : out std_logic;
		CLK : in std_logic;
		CS : out std_logic;
		DQ : inout std_logic_vector(15 downto 0);
		in_port_to_the_touch_irq : in std_logic;
		led : out std_logic_vector(3 downto 0);
		MISO_to_the_spi_touch : in std_logic;
		MOSI_from_the_spi_touch : out std_logic;
		out_port_from_the_bl_n : out std_logic;
		out_port_from_the_bl_p : out std_logic;
		out_port_from_the_lcd_cs : out std_logic;
		out_port_from_the_lcd_rd : out std_logic;
		out_port_from_the_lcd_rs : out std_logic;
		out_port_from_the_lcd_wr : out std_logic;
		out_port_from_the_reset : out std_logic;
		out_port_from_the_touch_cs : out std_logic;
		RA : out std_logic_vector(12 downto 0);
		RAS : out std_logic;
		RESET : in std_logic;
		S_CLK : out std_logic;
		S_DQM : out std_logic_vector(1 downto 0);
		SCLK_from_the_spi_touch : out std_logic;
		SS_n_from_the_spi_touch : out std_logic;
		WE : out std_logic
-- {ALTERA_IO_END} DO NOT REMOVE THIS LINE!

	);

-- {ALTERA_ATTRIBUTE_BEGIN} DO NOT REMOVE THIS LINE!
-- {ALTERA_ATTRIBUTE_END} DO NOT REMOVE THIS LINE!
end lcd_nios;

architecture ppl_type of lcd_nios is

-- {ALTERA_COMPONENTS_BEGIN} DO NOT REMOVE THIS LINE!
-- {ALTERA_COMPONENTS_END} DO NOT REMOVE THIS LINE!
begin
-- {ALTERA_INSTANTIATION_BEGIN} DO NOT REMOVE THIS LINE!
-- {ALTERA_INSTANTIATION_END} DO NOT REMOVE THIS LINE!

end;
