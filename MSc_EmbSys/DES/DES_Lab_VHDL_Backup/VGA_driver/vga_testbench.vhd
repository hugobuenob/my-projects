library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_testbench is
end vga_testbench;



architecture arch of vga_testbench is
    signal clk : std_logic := '0';
    signal sync_h, sync_v, green : std_logic;
    signal mem_addr : std_logic_vector(31 downto 0);
    signal mem_enable : std_logic;                       --0123456789ABCDEF0123456789ABCDEF
    signal mem_data_out : std_logic_vector(31 downto 0) := "10101111111111111111111111111111";--std_logic_vector(to_unsigned(16#7FFFFFF3#, 32));---(others => '1');
    signal mem_clk : std_logic := '0';
    
begin
    clk <= not clk after 1 ns;
    
    
--    DUT: entity work.vga_driver
--         port map (clk_100M => clk, sync_h => sync_h, sync_v => sync_v, green => green,
--         mem_addr => mem_addr, mem_enable => mem_enable, mem_data_in => mem_data_in, mem_clk => mem_clk);


DUT: entity work.vga_driver
         port map (clk_100M => clk, o_sync_h => sync_h, o_sync_v => sync_v, o_green => green,
         o_mem_addr => open, o_mem_enable => open, o_mem_data_in => open, i_mem_data_out => mem_data_out, o_mem_we => open, o_mem_clk => open,
         o_video_access => open, i_update_addr => (others => '1'), i_update_data_in => (others => '0'), o_update_data_out => open, i_update_we => (others => '1')
         );
         
         
         
end;