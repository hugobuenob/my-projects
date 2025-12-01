library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity matrix_axi_v1_0 is
	generic (
		-- Users to add parameters here
		-- START MODIFICATION
		POINT_SIZE : integer := 11;
		-- END MODIFICATION
		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 6
	);
	port (
		-- Users to add ports here
		
	    -- START MODIFICATION
		
		-- Memory interface
        o_mem_addr : out std_logic_vector(31 downto 0);
        i_mem_data_out : in std_logic_vector(31 downto 0);
        o_mem_we : out std_logic_vector(3 downto 0);
        o_mem_enable : out std_logic;
        o_mem_clk : out std_logic;
        -- Interface with Bresenham
        o_px0, o_py0, o_px1, o_py1: out std_logic_vector(POINT_SIZE-1 downto 0);
        o_endpoints_valid : out std_logic;
        i_endpoints_ready : in std_logic;
		
		-- END MODIFICATION

		-- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
end matrix_axi_v1_0;

architecture arch_imp of matrix_axi_v1_0 is
    -- START MODIFICATION
		--   Control
        signal reg_num_lines : std_logic_vector(31 downto 0);
        signal reg_control : std_logic_vector(31 downto 0);
        signal reg_status :  std_logic_vector(31 downto 0);
        --   Rotation matrix elements in float32 format
        signal reg_matrix_11, reg_matrix_12, reg_matrix_13,
               reg_matrix_21, reg_matrix_22, reg_matrix_23 : std_logic_vector(31 downto 0);
        --   Offset
        signal reg_offset_x, reg_offset_y : std_logic_vector(31 downto 0);
	-- END MODIFICATION
begin

-- MODIFICATION: Changed component for entity instantiation
-- Instantiation of Axi Bus Interface S00_AXI
Points_Transformer_v1_0_S00_AXI_inst : entity work.Points_Transformer_v1_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
	    -- START MODIFICATION
	    -- User ports
	    --   Control
        reg_num_lines => reg_num_lines,
        reg_control => reg_control,
        reg_status => reg_status,
        --   Rotation matrix elements in float32 format
        reg_matrix_11 => reg_matrix_11, reg_matrix_12 => reg_matrix_12, reg_matrix_13 => reg_matrix_13,
        reg_matrix_21 => reg_matrix_21, reg_matrix_22 => reg_matrix_22, reg_matrix_23 => reg_matrix_23,
        --   Offset
        reg_offset_x => reg_offset_x, reg_offset_y => reg_offset_y,
        -- END MODIFICATION
        
		S_AXI_ACLK	=> s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	=> s00_axi_wdata,
		S_AXI_WSTRB	=> s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	=> s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	=> s00_axi_rdata,
		S_AXI_RRESP	=> s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready
	);

	-- Add user logic here
	
	
	-- START MODIFICATION
	USER_LOGIC: entity work.matrix
	port map(
	    -- Registers accessible through AXI
        i_reg_num_lines => reg_num_lines, i_reg_control => reg_control, o_reg_status => reg_status,
        i_reg_matrix_11 => reg_matrix_11, i_reg_matrix_12 => reg_matrix_12, i_reg_matrix_13 => reg_matrix_13,
        i_reg_matrix_21 => reg_matrix_21, i_reg_matrix_22 => reg_matrix_22, i_reg_matrix_23 => reg_matrix_23,
        i_offset_x => reg_offset_x, i_offset_y => reg_offset_y,
        -- Clock and reset
        clk_100M => s00_axi_aclk, resetn => s00_axi_aresetn,
        -- Memory and endpoint interfaces, go straight through to outside
        o_mem_addr => o_mem_addr, i_mem_data_out => i_mem_data_out, o_mem_we => o_mem_we, o_mem_enable => o_mem_enable, o_mem_clk => o_mem_clk,
        o_px0 => o_px0, o_py0 => o_py0, o_px1 => o_px1, o_py1 => o_py1,
        o_endpoints_valid => o_endpoints_valid, i_endpoints_ready => i_endpoints_ready
        );

		
    -- END MODIFICATION

	-- User logic ends

end arch_imp;
