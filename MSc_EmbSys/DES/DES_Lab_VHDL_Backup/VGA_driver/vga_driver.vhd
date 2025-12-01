library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_driver is
port(
    clk_100M: in std_logic;
    o_sync_h, o_sync_v : out std_logic := '1';
    o_green : out std_logic := '0';
    -- BRAM interface
    o_mem_addr : out std_logic_vector(31 downto 0) := (others => '0');
    o_mem_enable : out std_logic;
    o_mem_data_in : out std_logic_vector(31 downto 0);
    i_mem_data_out : in std_logic_vector(31 downto 0);
    o_mem_we : out std_logic_vector(3 downto 0);
    o_mem_clk : out std_logic;
    -- Interface with Update IP
    o_video_access : out std_logic := '1';
    i_update_addr : in std_logic_vector(31 downto 0);
    i_update_data_in : in std_logic_vector(31 downto 0);
    o_update_data_out : out std_logic_vector(31 downto 0);
    i_update_we : in std_logic_vector(3 downto 0)
);
end vga_driver;


architecture arch of vga_driver is

    -- Debug signals
    attribute mark_debug : string;
    attribute mark_debug of o_sync_h: signal is "true";
    attribute mark_debug of o_sync_v: signal is "true";
    attribute mark_debug of o_green: signal is "true";
    attribute mark_debug of o_mem_addr: signal is "true";
    attribute mark_debug of o_mem_enable: signal is "true";
    attribute mark_debug of o_mem_data_in: signal is "true";
    attribute mark_debug of i_mem_data_out: signal is "true";
    attribute mark_debug of o_mem_we: signal is "true";
    attribute mark_debug of o_video_access: signal is "true";
    attribute mark_debug of i_update_addr: signal is "true";
    attribute mark_debug of i_update_data_in: signal is "true";
    attribute mark_debug of o_update_data_out: signal is "true";
    attribute mark_debug of i_update_we: signal is "true";
    -- 
    signal pix_enable : std_logic := '0';
    signal count_h, count_v : integer range 0 to 2048 := 0;
    signal blank : std_logic := '1';
    
    signal video_access_sig : std_logic := '0';
    signal video_access_sel : std_logic := '0';
    
    signal video_mem_addr : std_logic_vector(31 downto 0) := (others => '0');
    
    constant H_BLANK_START  : integer := 0+32;    -- 0+32;
    constant H_BLANK_END    : integer := 800+32;  -- 800+32;
    constant H_SYNC_START   : integer := 856+32;  -- 840+32;
    constant H_SYNC_END     : integer := 976+32;  -- 968+32;
    constant H_RESTART      : integer := 1040+32; -- 1056+32;
    constant V_BLANK_START  : integer := 600;     -- 600;
    constant V_SYNC_START   : integer := 637;     -- 601;
    constant V_SYNC_END     : integer := 643;     -- 605;
    constant V_RESTART      : integer := 666;     -- 628;
begin
    -- Pixel enable signal for operation at 50 MHz with 100 MHz input clk
    process(clk_100M)
    begin
        if(rising_edge(clk_100M)) then
            pix_enable <= not pix_enable;
        end if;
    end process;
    
    -- Sync signal generation
    process(clk_100M)
    begin
        if(rising_edge(clk_100M)) then
            if(pix_enable = '1') then
                if(count_h = H_RESTART-1) then -- new horizontal line
                    count_h <= 0;
                    
                    if(count_v = V_RESTART-1) then count_v <= 0;
                    else count_v <= count_v + 1;
                    end if;
                    
                    if(count_v = V_SYNC_START-1) then o_sync_v <= '0';
                    elsif(count_v = V_SYNC_END-1) then o_sync_v <= '1';
                    end if;
                    
                else
                    count_h <= count_h + 1;
                end if;
                        
                -- blank active one pixel before, so registered output is on time
                if(count_v > V_BLANK_START-2) then blank <= '1';
                elsif (count_h = H_BLANK_START-2) then blank <= '0';
                elsif (count_h = H_BLANK_END-2) then blank <= '1';
                end if;
                
                if(count_h = H_SYNC_START-1) then o_sync_h <= '0';
                elsif(count_h = H_SYNC_END-1) then o_sync_h <= '1'; end if;
            end if;
        end if;
    end process;
    
    -- Read from memory and shift out memory word
    process(clk_100M)
        variable word_shift : std_logic_vector (31 downto 0) := (others => '0');
    begin
        if(rising_edge(clk_100M)) then
            if(pix_enable = '1') then
                video_mem_addr <= std_logic_vector(to_unsigned(4*(count_h / 32) + 4*25*count_v, 32));
                
                if((count_h MOD 32) = 31) then
                    word_shift := i_mem_data_out;
                else
                    word_shift := word_shift(30 downto 0) & '0';
                end if;
                
                o_green <= word_shift(31) and not blank;
                
                video_access_sig <= '0'; -- Clear video acess after a video access begins
            else
                if((count_h MOD 32) = 30 and count_h < H_BLANK_END) then
                    video_access_sig <= '1'; -- Signal video access one cycle before reading
                end if;
            end if;
            
            video_access_sel <= video_access_sig; -- Select signal for video access mux delayed one cycle
        end if;
    end process;
    
    -- Continuous assignments
    o_mem_enable <= '1';
    o_mem_clk <= clk_100M;
    
    o_video_access <= video_access_sig;
    
    o_mem_data_in <= i_update_data_in;
    o_update_data_out <= i_mem_data_out;
     
    -- Video access muxes
    o_mem_addr <= video_mem_addr when video_access_sel = '1' else
                  i_update_addr;       
    o_mem_we <= "0000" when video_access_sel = '1' else
                i_update_we;
                
end arch;