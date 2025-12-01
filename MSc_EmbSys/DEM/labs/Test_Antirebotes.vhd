--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   17:51:11 11/16/2016
-- Design Name:   
-- Module Name:   C:/ed/prac3/Test_Antirebotes.vhd
-- Test Bench Circuito Antirebotes
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

 
ENTITY Test_Antirebotes IS
END Test_Antirebotes;
 
ARCHITECTURE behavior OF Test_Antirebotes IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
    COMPONENT Antirebotes
    PORT(
         clk : IN  std_logic;
			reset	: in std_logic;	
         boton : IN  std_logic;
         filtrado : OUT  std_logic
        );
    END COMPONENT;
    
   --Inputs
   signal clk : std_logic := '0';
   signal boton : std_logic := '0';

 	--Outputs
   signal filtrado : std_logic;
	signal reset : std_logic;

   -- Clock period definitions
   constant clk_period : time := 8 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Antirebotes PORT MAP (
          clk => clk,
			 reset => reset,
          boton => boton,
          filtrado => filtrado
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
		boton <= '0';
		wait for 100 ns;
		reset <= '0';
		wait for 1 ms;
	 
		-- Rebotes por ruido
		boton <= '1';
		wait for 100 us;
		boton <= '0';
		wait for 20 us;
		boton <= '1';
		wait for 10 us;
		boton <= '0';
		wait for 2 ms;
	 
		-- Pulsaci�n correcta
		boton <= '1';
		wait for 100 us;
		boton <= '0';
		wait for 20 us;
		boton <= '1';
		wait for 2 ms;
    
		-- Rebotes al dejar de pulsar el bot�n
		boton <= '0';
		wait for 10 us;
		boton <= '1';
		wait for 10 us;
		boton <= '0';
		wait for 10 us;
		boton <= '1';
		wait for 10 us;
		boton <= '0';
		wait for 2 ms;

      wait;
		
   end process;

END;
