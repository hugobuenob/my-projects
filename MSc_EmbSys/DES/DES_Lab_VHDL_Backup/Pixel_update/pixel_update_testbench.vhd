library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pixel_update_testbench is
generic(
    POINT_SIZE : integer := 11
);
end pixel_update_testbench;

architecture arch of pixel_update_testbench is

    signal clk_100M : std_logic := '0';
    signal resetn : std_logic := '0';
    -- memory interface
    signal o_update_addr : std_logic_vector(31 downto 0);
    signal o_update_data_in : std_logic_vector(31 downto 0);
    signal i_update_data_out : std_logic_vector(31 downto 0) := x"00000000";
    signal o_update_we : std_logic_vector(3 downto 0);
    signal i_video_access : std_logic := '0';
    -- point data interface
    signal i_point_x : std_logic_vector(POINT_SIZE-1 downto 0) := (others => '0');
    signal i_point_y : std_logic_vector(POINT_SIZE-1 downto 0) := (others => '0');
    signal i_point_valid : std_logic := '0';
    signal o_point_ready : std_logic;

begin
    clk_100M <= not clk_100M after 1 ns;
    
    process
    begin
        resetn <= '1' after 3 ns;
        
        wait for 18 ns;
        wait until rising_edge(clk_100M); i_video_access <= '1';
        wait until rising_edge(clk_100M); i_video_access <= '0';
        
        wait for 12 ns;
        wait until rising_edge(clk_100M); i_video_access <= '1';
        wait until rising_edge(clk_100M); i_video_access <= '0';
        
        wait for 10 ns;
        wait until rising_edge(clk_100M); i_video_access <= '1';
        wait until rising_edge(clk_100M); i_video_access <= '0';

        wait for 12 ns;
        assert FALSE Report "Simulation Finished" severity FAILURE;
    end process;
    
    process
    begin
    i_point_x <= "000" & x"20";
    i_point_y <= "000" & x"00";
    i_point_valid <= '1';
    
    wait on clk_100M until o_point_ready = '1';
    wait until rising_edge(clk_100M);
    i_point_valid <= '0';
    
    wait until rising_edge(clk_100M);
    i_point_x <= "000" & x"41";
    i_point_y <= "000" & x"00";
    i_point_valid <= '1';
    
    wait on clk_100M until o_point_ready = '1';
    wait until rising_edge(clk_100M);
    i_point_valid <= '0';
    
    wait for 10 ns;
    
    wait until rising_edge(clk_100M);
    i_point_x <= "000" & x"20";
    i_point_y <= "000" & x"00";
    i_point_valid <= '1';
    
    wait on clk_100M until o_point_ready = '1';
    wait until rising_edge(clk_100M);
    i_point_valid <= '0';
    
    wait for 10 ns;
    
    wait until rising_edge(clk_100M);
    i_point_x <= "000" & x"41";
    i_point_y <= "000" & x"00";
    i_point_valid <= '1';
    
    wait on clk_100M until o_point_ready = '1';
    wait until rising_edge(clk_100M);
    i_point_valid <= '0';
    
    wait;
    
    end process;

DUT: entity work.pixel_update
        port map(clk_100M =>clk_100M, resetn =>resetn,
         o_update_addr =>o_update_addr, o_update_data_in =>o_update_data_in, i_update_data_out => i_update_data_out,
         o_update_we => o_update_we, i_video_access => i_video_access,
         i_point_x => i_point_x, i_point_y => i_point_y, i_point_valid => i_point_valid, o_point_ready => o_point_ready
        );

end;