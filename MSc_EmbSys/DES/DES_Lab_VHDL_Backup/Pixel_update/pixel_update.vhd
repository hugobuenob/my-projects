library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pixel_update is
generic(
    POINT_SIZE : integer := 11
);
port(
    -- clk and reset
    clk_100M : in std_logic;
    resetn : in std_logic;
    -- memory interface
    o_update_addr : out std_logic_vector(31 downto 0);
    o_update_data_in : out std_logic_vector(31 downto 0);
    i_update_data_out : in std_logic_vector(31 downto 0);
    o_update_we : out std_logic_vector(3 downto 0);
    i_video_access : in std_logic;
    -- point data interface
    i_point_x : in std_logic_vector(POINT_SIZE-1 downto 0);
    i_point_y : in std_logic_vector(POINT_SIZE-1 downto 0);
    i_point_valid : in std_logic;
    o_point_ready : out std_logic
);
end pixel_update;

architecture arch of pixel_update is
    signal point_x_reg, point_y_reg : unsigned(POINT_SIZE-1 downto 0);
    
    signal word_mask : unsigned(31 downto 0);
    signal video_access_reg : std_logic := '0';
    
    
    type Control_state_t is (S0, S1, S2);
    signal control_state : Control_state_t := S2;
    signal last_control_state : Control_state_t := S2;
     
begin
    
    -- Determine address and mask
    process(point_x_reg, point_y_reg)
        variable x_word : integer range 0 to 31;
        variable x_word_idx : integer range 0 to 31;
        variable tmp_update_addr : unsigned(31 downto 0);
    begin
        x_word := to_integer(point_x_reg)/32;
        x_word_idx := to_integer(point_x_reg) mod 32;
        word_mask <= shift_right(x"80000000", x_word_idx);
        tmp_update_addr := resize(4*(x_word + 25*point_y_reg),tmp_update_addr'length);
        o_update_addr <= std_logic_vector(tmp_update_addr(31 downto 0));
    end process;
    
    -- Apply mask to RAM data
    o_update_data_in <= i_update_data_out or std_logic_vector(word_mask);
    
    -- Control state machine
    -- A memory access consists of S1, S2.
    -- If the sequence is interrupted by video access, S1 is extended.
    -- State machine loads new points in S2.
    process(clk_100M)
    begin
    if rising_edge(clk_100M) then
        -- Register video_access
        video_access_reg <= i_video_access;
        
        if (resetn = '0') then
            control_state <= S2;
            last_control_state <= S2;
            point_x_reg <= (others => '0');
            point_y_reg <= (others => '0');
        else  
        case(control_state) is
            when S1 =>
                if(i_video_access = '0' and video_access_reg = '0') then control_state <= S2; end if;
            when S2 =>
                if(i_point_valid = '1') then
                    control_state <= S1;
                    point_x_reg <= unsigned(i_point_x);
                    point_y_reg <= unsigned(i_point_y);
                end if;
            when others =>
                control_state <= S2;
        end case;
        last_control_state <= control_state;
        end if;      
    end if;
    end process;
    
    -- S2 is ready to load new dats
    o_point_ready <= '1' when control_state = S2 else '0';
    -- Write to memory only when we come from S1, that is, when the address has been loaded
    -- If we come from S2, we are just waiting
    o_update_we <= "1111" when (control_state = S2 and last_control_state = S1) else "0000";
    
end arch;




---- Control state machine
--    process(clk_100M)
--    begin
--    if rising_edge(clk_100M) then
--        if (resetn = '0') then control_state <= S0;
--        else
--        case(control_state) is
--            when S1 =>
--                if(video_access_reg = '0') then control_state <= S2; end if;
--            when S2 =>
--                if(video_access_reg = '0') then control_state <= S0;
--                else control_state <= S1; end if;
--            when S0 =>
--                if(i_point_valid = '1') then
--                    control_state <= S1;
--                    point_x_reg <= unsigned(i_point_x);
--                    point_y_reg <= unsigned(i_point_y);
--                end if;
--            when others =>
--                control_state <= S0;
--        end case;
--        end if;      
--    end if;
--    end process;
    
--    o_point_ready <= '1' when control_state = S0 else '0';
--    o_update_we <= "1111" when (control_state = S2 and video_access_reg = '0') else "0000";