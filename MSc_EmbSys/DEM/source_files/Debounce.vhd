library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Debounce is
         Port (    clk:    in std_logic;
                    reset:  in std_logic;
                    b :     in std_logic;
                    btn_f :    out std_logic
                );
end Debounce;

architecture Behavioral of Debounce is

     --FSM
     type state_t is (s_wait, s_timer);
     signal state: state_t;
     
     --Timer
     signal e   : std_logic;
     constant C_MAX_1ms : integer := 125*10**2 / 1000;
     signal cnt_1ms : integer range C_MAX_1ms-1 downto 0;
     signal t   : std_logic;

    --- Synchronizer + edge detector A
    signal bd   : std_logic;    --FF  w/ metastrability
    signal bs  : std_logic;    --FF w/ sync button
    signal bsd : std_logic;    --FF w/ sync button
    signal be   : std_logic;    -- Rising edge in
    
         
    
begin

        ---- FSM 
    process(clk, reset)
        begin
            if (reset = '1') then
                state <= s_wait;
            elsif rising_edge(clk) then
                case state is
                    when s_wait => 
                        if (be = '1') then
                            state <= s_timer;
                        end if;    
                    when s_timer => 
                        if t = '1' then
                            state <= s_wait;
                        end if;
                end case;
            end if;    
    end process;

    e   <= '1' when state = s_timer else '0';   -- MOORE 
    btn_f  <= '1' when state = s_timer and t='1' and bs = '1' else '0';  --- MEALY 
    
       
    ----- Timer 
    process(clk, reset)
        begin
            if (reset = '1') then
                cnt_1ms <= 0;
            elsif (clk'event and clk='1') then
                if (e = '1') then
                   if (cnt_1ms < C_MAX_1ms-1) then
                        cnt_1ms <= cnt_1ms + 1;
                   else
                        cnt_1ms <= 0;  
                    end if;       
                end if;
            end if;
                      
     end process;
    
    t <= '1' when (cnt_1ms = C_MAX_1ms-1)else '0';

    
    ----- Synchronizer + edge detector BOTTON A--- 
        process(clk, reset)
        begin
            if (reset = '1') then
                bd <= '0';
                bs <= '0';
                bsd <= '0';
            elsif (clk'event and clk='1') then
                  bd <= b;
                  bs <= bd;
                  bsd <= bs;                      
            end if;
        end process;
        
        be <= '1' when bs ='1' and bsd = '0' else '0';  

end Behavioral;
