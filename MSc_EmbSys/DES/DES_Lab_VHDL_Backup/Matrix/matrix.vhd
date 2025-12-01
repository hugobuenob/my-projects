library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity matrix is
generic(
    POINT_SIZE : integer := 11
);
port(
    -- To AXI interface
    --   Control
    i_reg_num_lines : in std_logic_vector(31 downto 0);
    i_reg_control : in std_logic_vector(31 downto 0);
    o_reg_status : out std_logic_vector(31 downto 0);
    --   Rotation matrix elements in float32 format
    i_reg_matrix_11, i_reg_matrix_12, i_reg_matrix_13,
    i_reg_matrix_21, i_reg_matrix_22, i_reg_matrix_23 : in std_logic_vector(31 downto 0);
    --   Offset
    i_offset_x, i_offset_y : in std_logic_vector(31 downto 0);
    
    -- Clock and reset
    clk_100M : in std_logic;
    resetn : in std_logic;
    -- Memory interface
    o_mem_addr : out std_logic_vector(31 downto 0);
    i_mem_data_out : in std_logic_vector(31 downto 0);
    o_mem_we : out std_logic_vector(3 downto 0);
    o_mem_enable : out std_logic;
    o_mem_clk : out std_logic;
    -- Interface with Bresenham
    o_px0, o_py0, o_px1, o_py1: out std_logic_vector(POINT_SIZE-1 downto 0);
    o_endpoints_valid : out std_logic;
    i_endpoints_ready : in std_logic
);
end matrix;

architecture arch of matrix is

    -- State machine
    type Control_state_t is (S0, S1, S2, S3, S4, S5, S6, S7, S8, S9);
    signal control_state : Control_state_t := S0;
    signal read_lines : unsigned(31 downto 0) := (others => '0');
    signal mem_address : unsigned(31 downto 0) := (others => '0');
    
    -- Input & output variables for transform
    signal px_float, py_float, pz_float : std_logic_vector(31 downto 0);
    signal transformed_x, transformed_y : std_logic_vector(31 downto 0);
    signal reg_x0, reg_y0 : std_logic_vector(31 downto 0);
    
    -- Control
    signal endpoints_valid : std_logic := '0';
    
    signal done : std_logic;
    
    signal start : std_logic;
begin
    -- Control registers
    start <= i_reg_control(0);
    o_reg_status <= (0 => done, others => '0');
    
    -- Combinational function, computes transformed_x, transformed_y from input x, y, z, rotation matrix and offsets.
--    transform: entity work.point_transform
--        port map(
--        i_reg_matrix_11 => i_reg_matrix_11, i_reg_matrix_12 => i_reg_matrix_12, i_reg_matrix_13 => i_reg_matrix_13,
--        i_reg_matrix_21 => i_reg_matrix_21, i_reg_matrix_22 => i_reg_matrix_22, i_reg_matrix_23 => i_reg_matrix_23,
--        i_offset_x => i_offset_x, i_offset_y => i_offset_y,
--        i_px_float => px_float, i_py_float => py_float, i_pz_float => pz_float,
--        o_px => transformed_x, o_py => transformed_y
--        ); 
        
        transformed_x <= px_float;
        transformed_y <= py_float;
    
    -- Control state machine, reads points from memory (x0, y0, z0), (x1, y1, z1), outputs transformed points (x0, y0), (x1, y1)
    process(clk_100M)
    begin
        if(rising_edge(clk_100M)) then
            if(resetn = '0') then
                control_state <= S0;
                endpoints_valid <= '0';
            else
            case control_state is
                when S0 =>
                    read_lines <= to_unsigned(0, read_lines'length);
                    mem_address <= to_unsigned(0, mem_address'length);
                    if(start = '1') then control_state <= S1; end if;
                when S1 => -- Wait for memory to register address
                    mem_address <= mem_address + 4;
                    control_state <= S2;
                when S2 => -- Read x0, y0, z0
                    px_float <= i_mem_data_out;
                    mem_address <= mem_address + 4;
                    control_state <= S3;
                when S3 =>
                    py_float <= i_mem_data_out;
                    mem_address <= mem_address + 4;
                    control_state <= S4;
                when S4 =>
                    pz_float <= i_mem_data_out;
                    mem_address <= mem_address + 4;
                    control_state <= S5;
                when S5 => -- Read x1, y1, z1, register transformed x0, y0
                    reg_x0 <= transformed_x;
                    reg_y0 <= transformed_y;
                    px_float <= i_mem_data_out;
                    mem_address <= mem_address + 4;
                    control_state <= S6;
                when S6 =>
                    py_float <= i_mem_data_out;
                    mem_address <= mem_address + 4;
                    control_state <= S7;
                when S7 => -- If transformed points not read yet, wait
                    pz_float <= i_mem_data_out;
                    if(endpoints_valid = '1') then
                        control_state <= S8;
                    else
                        control_state <= S9;
                    end if;
                when S8 => -- Wait for endpoints to be read
                    if(endpoints_valid = '0') then control_state <= S9; end if;
                when S9 =>
                    mem_address <= mem_address + 4;
                    -- Output point
                    o_px0 <= reg_x0(POINT_SIZE-1 downto 0);
                    o_py0 <= reg_y0(POINT_SIZE-1 downto 0);
                    o_px1 <= transformed_x(POINT_SIZE-1 downto 0);
                    o_py1 <= transformed_y(POINT_SIZE-1 downto 0);
                    
                    -- Valid
                    endpoints_valid <= '1';
                    
                    -- Count lines
                    read_lines <= read_lines + 1;
                    
                    -- Next state, either we continue with next line or we are done.
                    if(read_lines >= unsigned(i_reg_num_lines)) then
                        control_state <= S0;
                    else
                        control_state <= S2;
                    end if;
                when others =>
                    control_state <= S0;
            
            end case;
            
            -- Valid and ready signal
            if(endpoints_valid = '1' and i_endpoints_ready = '1') then
                endpoints_valid <= '0';
            end if;
            end if;    
        end if;
    end process;
    
    -- State dependant signals
    done <= '1' when control_state = S0 else '0';
    
    -- Output assignments
    o_endpoints_valid <= endpoints_valid;
    o_mem_addr <= std_logic_vector(mem_address);
    o_mem_we <= "0000";
    o_mem_enable <= '1';
    o_mem_clk <= clk_100M;
 
end arch;

