library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Naleatorio is
    Port ( clk      : in std_logic;
           reset    : in std_logic;
           random   : out std_logic_vector(3 downto 0)
         );
end Naleatorio;

architecture Behavioral of Naleatorio is
    signal vinicial : std_logic_vector(3 downto 0) := "0000";
    signal di : std_logic_vector(3 downto 0) := "0001";
    signal i : integer := 0;

begin

     process(clk,reset)
             begin
                 if (reset = '1') then
                    vinicial <= (others => '0');
                 elsif (clk'event and clk = '1') then
			vinicial <= di(i) & vinicial(3 downto 1);
			if ( i < 3) then
                                i <= i+1;
			else
				i <= 0;
                        end if;
                end if;
    end process;

    random <= vinicial;

end Behavioral;