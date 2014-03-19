-- Inputs: SW7-0 are parallel port inputs to the Nios II system.
-- CLOCK_50 is the system clock.
-- KEY0 is the active-low system reset.
-- Outputs: LEDG7-0 are parallel port outputs from the Nios II system.
-- SDRAM ports correspond to the signals in Figure 2; their names are those used in the DE2 User Manual.

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

ENTITY zap IS
    PORT (
        SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
        KEY : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
        CLOCK_50 : IN STD_LOGIC;
        LEDG : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
        DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
        DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
        DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
        DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
        DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
        LCD_DATA : INOUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        LCD_ON, LCD_BLON, LCD_EN, LCD_RS, LCD_RW : OUT STD_LOGIC;
        VGA_R : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
        VGA_G : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
        VGA_B : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
        VGA_CLK : OUT STD_LOGIC;
        VGA_BLANK : OUT STD_LOGIC;
        VGA_HS : OUT STD_LOGIC;
        VGA_VS : OUT STD_LOGIC;
        VGA_SYNC : OUT STD_LOGIC;
        SRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
        SRAM_ADDR : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);
        SRAM_LB_N : OUT STD_LOGIC;
        SRAM_UB_N : OUT STD_LOGIC;
        SRAM_CE_N : OUT STD_LOGIC;
        SRAM_OE_N : OUT STD_LOGIC;
        SRAM_WE_N : OUT STD_LOGIC;
		  -- AV Config
		  I2C_SDAT : INOUT STD_LOGIC;
		  I2C_SCLK : OUT STD_LOGIC;
		  -- Clock (audio)
		  AUD_XCK : out std_logic;
		  CLOCK_27 : in std_logic;
		  -- Audio
			AUD_ADCDAT         : in    STD_LOGIC;
			AUD_ADCLRCK        : in    STD_LOGIC;
			AUD_BCLK           : in    STD_LOGIC;
			AUD_DACDAT         : out   STD_LOGIC;
			AUD_DACLRCK        : in    STD_LOGIC;
        -- SD Card
        SD_CMD : INOUT STD_LOGIC;
        SD_DAT : INOUT STD_LOGIC;
        SD_DAT3 : INOUT STD_LOGIC;
        SD_CLK : OUT STD_LOGIC;
		  PS2_CLK : INOUT STD_LOGIC;
		  PS2_DAT : INOUT STD_LOGIC;
		  -- RS 232
		  UART_RXD : IN STD_LOGIC;
		  UART_TXD : OUT STD_LOGIC
        );
END zap;

ARCHITECTURE Structure OF zap IS
    COMPONENT nios_system PORT (
        clk_clk : in std_logic;
        reset_reset_n : in std_logic;
        sdram_clk_clk : out std_logic;
        leds_export : out std_logic_vector(7 downto 0);
        switches_export : in std_logic_vector(7 downto 0);
        sdram_wire_addr : out std_logic_vector(11 downto 0);
        sdram_wire_ba : buffer std_logic_vector(1 downto 0);
        sdram_wire_cas_n : out std_logic;
        sdram_wire_cke : out std_logic;
        sdram_wire_cs_n : out std_logic;
        sdram_wire_dq : inout std_logic_vector(15 downto 0);
        sdram_wire_dqm : buffer std_logic_vector(1 downto 0);
        sdram_wire_ras_n : out std_logic;
        sdram_wire_we_n : out std_logic;
        lcd_data_data : inout std_logic_vector(7 downto 0);
        lcd_data_on : out std_logic;
        lcd_data_blon : out std_logic;
        lcd_data_en : out std_logic;
        lcd_data_rs : out std_logic;
        lcd_data_rw : out std_logic;
        vga_controller_clk : out std_logic;
        vga_controller_hs : out std_logic;
        vga_controller_vs : out std_logic;
        vga_controller_blank : out std_logic;
        vga_controller_sync : out std_logic;
        vga_controller_r : out std_logic_vector(9 downto 0);
        vga_controller_g : out std_logic_vector(9 downto 0);
        vga_controller_b : out std_logic_vector(9 downto 0);
        sram_dq : inout std_logic_vector(15 downto 0);
        sram_addr : out std_logic_vector(17 downto 0);
        sram_lb_n : out std_logic;
        sram_ub_n : out std_logic;
        sram_ce_n : out std_logic;
        sram_oe_n : out std_logic;
        sram_we_n: out std_logic;
		  -- AV Config
		  av_config_SDAT : inout std_logic;
		  av_config_SCLK : out std_logic;
		  -- Clock (audio)
		  audio_clk_clk : out std_logic;
		  audio_clk_in_clk : in std_logic;
		  -- Audio
		  audio_ADCDAT         : in    std_logic;
		  audio_ADCLRCK        : in    std_logic;
		  audio_BCLK           : in    std_logic;
		  audio_DACDAT         : out   std_logic;
		  audio_DACLRCK        : in    std_logic;
        -- SD CARD
        sd_card_b_SD_cmd : inout std_logic;
        sd_card_b_SD_dat : inout std_logic;
        sd_card_b_SD_dat3 : inout std_logic;
        sd_card_o_SD_clock : out std_logic;
		  -- RS 232
		  rs232_external_interface_RXD : in std_logic;
		  rs232_external_interface_TXD : out std_logic
        );
    END COMPONENT;
	 
    SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
    SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);

BEGIN
    DRAM_BA_0 <= BA(0);
    DRAM_BA_1 <= BA(1);
    DRAM_UDQM <= DQM(1);
    DRAM_LDQM <= DQM(0);
    -- Instantiate the Nios II system entity generated by the Qsys tool.
    NiosII: nios_system
        PORT MAP (
            clk_clk => CLOCK_50,
            reset_reset_n => KEY(0),
            sdram_clk_clk => DRAM_CLK,
            leds_export => LEDG,
            switches_export => SW,
            sdram_wire_addr => DRAM_ADDR,
            sdram_wire_ba => BA,
            sdram_wire_cas_n => DRAM_CAS_N,
            sdram_wire_cke => DRAM_CKE,
            sdram_wire_cs_n => DRAM_CS_N,
            sdram_wire_dq => DRAM_DQ,
            sdram_wire_dqm => DQM,
            sdram_wire_ras_n => DRAM_RAS_N,
            sdram_wire_we_n => DRAM_WE_N,
            lcd_data_DATA => LCD_DATA,
            lcd_data_ON => LCD_ON,
            lcd_data_EN => LCD_EN,
            lcd_data_RS => LCD_RS,
            lcd_data_RW => LCD_RW,
            lcd_data_BLON => LCD_BLON,
            vga_controller_CLK => VGA_CLK,
            vga_controller_HS => VGA_HS,
            vga_controller_VS => VGA_VS,
            vga_controller_BLANK => VGA_BLANK,
            vga_controller_SYNC => VGA_SYNC,
            vga_controller_R => VGA_R,
            vga_controller_G => VGA_G,
            vga_controller_B => VGA_B,
            sram_DQ => SRAM_DQ,
            sram_ADDR => SRAM_ADDR,
            sram_LB_N => SRAM_LB_N,
            sram_UB_N => SRAM_UB_N,
            sram_CE_N => SRAM_CE_N,
            sram_OE_N => SRAM_OE_N,
            sram_WE_N => SRAM_WE_N,
				-- AV Config
            av_config_SDAT => I2C_SDAT,
            av_config_SCLK => I2C_SCLK,
			   -- Clock (audio)
			   audio_clk_clk => AUD_XCK,
			   audio_clk_in_clk => CLOCK_27,
				-- Audio
            audio_ADCDAT         => AUD_ADCDAT,
            audio_ADCLRCK        => AUD_ADCLRCK,
            audio_BCLK           => AUD_BCLK,
            audio_DACDAT         => AUD_DACDAT,
            audio_DACLRCK        => AUD_DACLRCK,
            -- SD CARD
            sd_card_b_SD_cmd => SD_CMD,
            sd_card_b_SD_dat => SD_DAT,
            sd_card_b_SD_dat3 => SD_DAT3,
            sd_card_o_SD_clock => SD_CLK,
				-- RS 232
				rs232_external_interface_RXD => UART_RXD,
				rs232_external_interface_TXD => UART_TXD
				);
END Structure;
