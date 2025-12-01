library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_Simon is
--  Port ( );
end tb_Simon;

architecture Behavioral of tb_Simon is

         component Simon is
            Port (  clk       : in std_logic;
                    reset     : in std_logic;
                    btn_0     : in std_logic;
                    btn_1     : in std_logic;
                    btn_2     : in std_logic;
                    btn_3     : in std_logic;
                    segmentos : out std_logic_vector(6 downto 0);
                    selector  : out std_logic_vector (3 downto 0);
           	        leds      : out std_logic_vector(7 downto 0)
             );
        end component;
        


 constant clk_period : time := 8 ns;

 signal clk, reset, btn_0, btn_1, btn_2, btn_3 : std_logic;
-- signal btn_f0, btn_f1 , btn_f2, btn_f3: std_logic;
 signal segmentos 	: std_logic_vector(6 downto 0);
 signal selector 	: std_logic_vector(3 downto 0);
 signal leds 		: std_logic_vector(7 downto 0);


    
 begin
 


 Stim_CLK: process
 begin
 clk <= '1';
 wait for clk_period/2;
 clk <= '0';
 wait for clk_period/2;
 end process;

 Stim:process
     begin
     -- Reset e inicio de BTNs
     reset <= '1';
     btn_0 <= '0';
     btn_1 <= '0';
     btn_2 <= '0';
     btn_3 <= '0';
     wait for 5*clk_period;
     reset <= '0';
     
     
     wait for 15*clk_period;
     
     --- Btn de inicio de juego (any_btn_pressed)
     btn_1 <= '1';
     --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
     
     -- Espera de Display 1 ronda
     wait for 55*clk_period;
     
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
     
     -- Espera de Display 2 ronda
     wait for 165*clk_period;
     
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
     
     -- Espera de Display 3 ronda
     wait for 255*clk_period;
     
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
     
     -- Espera de Display 4 ronda
     wait for 385*clk_period;
     
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
     
     -- Espera de Display 5 ronda
     wait for 495*clk_period;

     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';   
     
          -- Espera de Display 6 ronda
     wait for 605*clk_period;

     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
       wait for 10*clk_period;
     btn_3 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_3 <= '0';
     
          -- Espera de Display 7 ronda
     wait for 715*clk_period;

     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0'; 
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
       wait for 10*clk_period;
     btn_3 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_3 <= '0';
       wait for 10*clk_period;
     btn_2 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_2 <= '0';
     
         -- Espera de Display 8 ronda
     wait for 825*clk_period;

     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_0 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_0 <= '0';
      wait for 10*clk_period;
     btn_1 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_1 <= '0';
       wait for 10*clk_period;
     btn_3 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_3 <= '0';
       wait for 10*clk_period;
     btn_2 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_2 <= '0';
       wait for 10*clk_period;
     btn_2 <= '1';
      --- Espera para deteccion de Debounce
      wait for 15*clk_period;
     btn_2 <= '0';
     
     --Espera de WIN
     wait for 120*clk_period;
     --- Btn de inicio de juego (any_btn_pressed)
     btn_3 <= '1';
     --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_3 <= '0';
     
     wait for 55*clk_period;

     btn_1 <= '1';
      --- Espera para deteccion de Debounce
     wait for 15*clk_period;
     btn_1 <= '0';
     
     wait;
     end process;

 CUT: Simon Port MAP (clk => clk,
 reset => reset,
 btn_0 => btn_0, 
 btn_1 => btn_1, 
 btn_2 => btn_2, 
 btn_3 => btn_3,
 segmentos => segmentos,
 selector => selector,
 leds => leds);

 end Behavioral;