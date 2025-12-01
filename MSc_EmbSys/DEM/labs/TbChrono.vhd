library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TbChrono is
end TbChrono;

architecture Behavioral of TbChrono is

    component Chrono is
    Port ( clk : in std_logic;
           reset : in std_logic;
           segundos : out std_logic_vector(3 downto 0);
           dec_segundos : out std_logic_vector(3 downto 0);
           minutos : out std_logic_vector(3 downto 0);
           dec_minutos : out std_logic_vector(3 downto 0)
         );
    end component;

   --Inputs
   signal clk       : std_logic := '0';
   signal reset     : std_logic := '0';
   signal Enable    : std_logic := '0';

 	--Outputs
   signal segundos      :  std_logic_vector(3 downto 0);
   signal dec_segundos  :  std_logic_vector(3 downto 0);
   signal minutos       :  std_logic_vector(3 downto 0);
   signal dec_minutos   :  std_logic_vector(3 downto 0);
   
   -- Clock period definitions
   constant clk_period : time := 8 ns;
 
BEGIN
  
	-- Instantiate the Unit Under Test (UUT)
   uut: Chrono PORT MAP (
          clk => clk,
          reset => reset,
          segundos => segundos,
          dec_segundos => dec_segundos,
          minutos => minutos,
          dec_minutos => dec_minutos);

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

end Behavioral;
