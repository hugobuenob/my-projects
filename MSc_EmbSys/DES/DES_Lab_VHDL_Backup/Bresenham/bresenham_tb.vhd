library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity bresenham_tb is
generic(
    POINT_SIZE : integer := 11
);
end bresenham_tb;

--entity lines is
--    port (
--    clk : in std_logic;
--    rst : in std_logic;
--    x0, y0, x1, y1 : in signed(1023 downto 0);
--    x_p, y_p : out signed(1023 downto 0);
--    start : in std_logic;
--    done, plot : out std_logic);
--end lines;

architecture arch of bresenham_tb is
    -- Clock and reset
    signal clk_100M : std_logic := '0';
    signal resetn : std_logic;
    -- Input coordiantes
    signal i_px0, i_py0, i_px1, i_py1 : std_logic_vector(POINT_SIZE-1 downto 0);
    -- Input control signals
    signal i_endpoints_valid : std_logic := '0';
    signal o_endpoints_ready : std_logic := '0';
    -- Output coordinates
    signal o_point_x, o_point_y : std_logic_vector(POINT_SIZE-1 downto 0);
    -- Output control signals
    signal i_point_ready : std_logic := '1';
    signal o_point_valid : std_logic;
begin

    clk_100M <= not clk_100M after 1ns;
    
    process
    begin
    for I in 0 to 20 loop
      wait until rising_edge(clk_100M);
      i_point_ready <= not i_point_ready;
    end loop;
    end process;
    
    
    process
    begin

        resetn <= '0';
        wait for 4 ns;
        resetn <= '1';
        
        wait for 3 ns;
        -- 0,0 to 30,20
        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(30, 11));
        i_py1 <= std_logic_vector(to_signed(20, 11));
--        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '1';
--        wait until rising_edge(clk_100M);
--        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to 20,30               
--        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(20, 11));
        i_py1 <= std_logic_vector(to_signed(30, 11));
--        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '1';
--        wait until rising_edge(clk_100M);
--        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to -20,30
--        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(-20, 11));
        i_py1 <= std_logic_vector(to_signed(30, 11));
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';

        -- 0,0 to -30,20        
        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(-30, 11));
        i_py1 <= std_logic_vector(to_signed(20, 11));
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to -20,-30
        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(-20, 11));
        i_py1 <= std_logic_vector(to_signed(-30, 11));
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to -30,-20
        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(-30, 11));
        i_py1 <= std_logic_vector(to_signed(-20, 11));
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to 20,-30
        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(20, 11));
        i_py1 <= std_logic_vector(to_signed(-30, 11));
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        
        -- 0,0 to 30,-20
--        wait until rising_edge(clk_100M);
        i_px0 <= std_logic_vector(to_signed(0, 11));
        i_py0 <= std_logic_vector(to_signed(0, 11));
        i_px1 <= std_logic_vector(to_signed(30, 11));
        i_py1 <= std_logic_vector(to_signed(-20, 11));
        i_endpoints_valid <= '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';
        wait on clk_100M until o_endpoints_ready = '1';
        wait until rising_edge(clk_100M);
        i_endpoints_valid <= '0';

        assert FALSE Report "Simulation Finished" severity FAILURE;
    end process;
 
    
    lines: entity work.bresenham
            port map(
            clk_100M => clk_100M, resetn => resetn,
            i_px0 => i_px0, i_py0 => i_py0, i_px1 => i_px1, i_py1 => i_py1,
            i_endpoints_valid => i_endpoints_valid, o_endpoints_ready => o_endpoints_ready,
            o_point_x => o_point_x, o_point_y => o_point_y,
            i_point_ready => i_point_ready, o_point_valid => o_point_valid
            );
            
end architecture;