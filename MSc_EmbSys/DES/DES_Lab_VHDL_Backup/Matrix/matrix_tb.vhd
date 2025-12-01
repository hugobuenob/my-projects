library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity matrix_tb is
end matrix_tb;
   
architecture arch of matrix_tb is
    signal clk : std_logic := '0';
    signal resetn : std_logic := '1';
    signal reg_control : std_logic_vector(31 downto 0) := (0 => '1', others => '0');
    signal reg_status : std_logic_vector(31 downto 0);
    signal endpoints_ready : std_logic := '1';
    
    signal mem_data_out : std_logic_vector(31 downto 0);
    signal mem_address : std_logic_vector(31 downto 0);
    
    
    type t_memory_array is array (0 to 32) of std_logic_vector(31 downto 0);
begin
    
    clk <= not clk after 1 ns;

    process
    begin
     resetn <= '0';
     wait for 2 ns;
     resetn <= '1';
     
     wait for 20 ns;
     wait until rising_edge(clk);
     endpoints_ready <= '0';
     wait for 30 ns;
     wait until rising_edge(clk);
     endpoints_ready <= '1';
     wait until rising_edge(clk);
     endpoints_ready <= '1';
     
     wait for 100 ns;
     
     assert FALSE report "Simulation Finished" severity FAILURE;
    
    end process;
    
    -- Simulated RAM
    process(clk)
        variable mem_contents : t_memory_array;
    begin
        
        mem_contents(0) := x"00000000"; -- 0
        mem_contents(1) := x"00000001"; -- 1
        mem_contents(2) := x"00000002";
        mem_contents(3) := x"00000003"; -- 3
        mem_contents(4) := x"00000004"; -- 4
        mem_contents(5) := x"00000005";
        
        mem_contents(6) := x"00000006"; -- 6
        mem_contents(7) := x"00000007"; -- 7
        mem_contents(8) := x"00000008";
        mem_contents(9) := x"00000009"; -- 9
        mem_contents(10):= x"0000000A"; -- 10
        mem_contents(11):= x"0000000B";
        
        mem_contents(12):= x"0000000C"; -- 12
        mem_contents(13):= x"0000000D"; -- 13
        mem_contents(14):= x"0000000E"; 
        mem_contents(15):= x"0000000F"; -- 15
        mem_contents(16):= x"00000010"; -- 16
        mem_contents(17):= x"00000011"; 
        
        if rising_edge(clk) then
            mem_data_out <= mem_contents( to_integer(unsigned(mem_address))/4 );
        end if;
    
    end process;
    
    DUT: entity work.matrix
        port map(
        i_reg_num_lines => std_logic_vector(to_unsigned(2,32)), i_reg_control => reg_control, o_reg_status => reg_status,
        i_reg_matrix_11 => x"3f800000", i_reg_matrix_12 => x"00000000", i_reg_matrix_13 => x"00000000",
        i_reg_matrix_21 => x"00000000", i_reg_matrix_22 => x"3f800000", i_reg_matrix_23 => x"00000000",
        i_offset_x => x"00000000", i_offset_y => x"00000000",
        clk_100M => clk, resetn => resetn,
        o_mem_addr => mem_address, i_mem_data_out => mem_data_out, o_mem_we => open, o_mem_enable => open, o_mem_clk => open,
        o_px0 => open, o_py0 => open, o_px1 => open, o_py1 => open,
        o_endpoints_valid => open, i_endpoints_ready => endpoints_ready
        );
        
end;