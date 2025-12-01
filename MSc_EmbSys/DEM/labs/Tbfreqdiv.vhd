LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY Tbfreqdiv IS
END Tbfreqdiv;
 
ARCHITECTURE behavior OF Tbfreqdiv IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT freqdiv
    PORT(
         clk : IN  std_logic;
         reset : IN  std_logic;
         Enable : IN  std_logic;
         Output_signal : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal reset : std_logic := '0';
   signal Enable : std_logic := '0';

 	--Outputs
   signal Output_signal : std_logic;

   -- Clock period definitions
   constant clk_period : time := 8 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: freqdiv PORT MAP (
          clk => clk,
          reset => reset,
          Enable => Enable,
          Output_signal => Output_signal
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
		reset <= '1';
      wait for 100 ns;	
		reset <= '0';
		enable <= '1';
      wait;
   end process;

END;
