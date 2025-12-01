library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

    entity Display is
        Port (clk         : in std_logic;
              reset       : in std_logic;
              v_show      : in std_logic_vector(3 downto 0);
              --- BCD conters
              segmentos  : out std_logic_vector (6 downto 0);
              selector   : out std_logic_vector (3 downto 0));
     end Display;

architecture Behavioral of Display is

	constant C_MAX_4kHz : integer := (125 * 10**3) / 4000;
	signal cnt_4kHz : integer range 0 to C_MAX_4kHZ-1;
	
    signal Enable_Cont1a4 : std_logic;                              ----Enable de cada contador
    signal contador_1a4      : unsigned(1 downto 0);                --- Contador de 1 a 4 
    signal omega    : integer range 0 to 1000 := 0;
    
    
    signal caso_1       : unsigned(3 downto 0);  
    signal caso_2       : unsigned(3 downto 0);
    signal caso_3       : unsigned(3 downto 0);
    signal caso_4       : unsigned(3 downto 0);
    
  signal v_simon : std_logic_vector(3 downto 0);
  
  ---- MUX 4to1
        signal data_BCD : unsigned(3 downto 0);                 --  DATA DEL VALOR INSTANTANEO (4BITS) A MOSTRAR , QUE SALE DEL MUX
  --- BCD to 7 segments    
        signal Segment_7  : std_logic_vector  (6 downto 0);     -- Signal CODIFICADA DE data_BCD QUE SE ENVIARA A LA SALIDA SEGMENTOS (6 DOWNTO 0)
  --- VALOR DE SELECTOR 0,1,2,3   
        signal C_mux : std_logic_vector(1 downto 0);            --- SIGNAL DE SELECCION DEL DISPLAY QUE SE VA A ENCENDER PERO EN VALOR DE 2 BITS    
  --- Decoder 2 to 4 
        signal interna: std_logic_vector(3 downto 0);           ---- Selector de display a mostrar
    

begin
    
--- Divisor de Freq 4 kHz  
        process(clk,reset)
             begin
                 if (reset = '1') then
                    cnt_4kHz <= 0;
                 elsif (clk'event and clk = '1') then
                     if (cnt_4kHz < C_MAX_4kHz-1) then
                        cnt_4kHz <= cnt_4kHz + 1;
                     else
                        cnt_4kHz <= 0;
                     end if;
                 end if;
             end process;
             
     Enable_Cont1a4 <= '1' when (cnt_4kHz = C_MAX_4kHz-1) else '0';
 
 --- Counter 0 a 3
      process(clk,reset)
             begin
                 if (reset = '1') then
                 contador_1a4 <= (others => '0');
                 elsif (clk'event and clk = '1') then
                    if (Enable_Cont1a4 = '1') then
                        if (contador_1a4 < 3) then
                            contador_1a4 <= contador_1a4 + 1;
                        else
                            contador_1a4 <= (others => '0');
                        end if;
                    end if;
                 end if;
             end process;
      
   C_mux <= std_logic_vector(contador_1a4);


                              
 ---- DECODER 2 TO 4
      with C_mux select
                interna <=    "0001" when "00",
                              "0010" when "01",
                              "0100" when "10",
                              "1000" when "11",
                              "0000" when others;
                selector <= interna;                              

 ---- BCD TO 7 SEGMENTS
     with data_BCD select
                Segment_7 <=  "0000001" when "0000",
                              "0000000" when "1000",
                              "0110000" when "1110",
                              "1111111" when others;
               segmentos <= Segment_7;                

---- MUX 4a1
     with C_mux select
                data_BCD <=   caso_1 when "00",
                              caso_2 when "01",
                              caso_3 when "10",
                              caso_4 when "11",
                              "1111" when others;        


---- Habilitacion de entity work.Debounce
    process(reset,v_show)
        begin
            if (reset = '1') then
                    caso_1 <= "1111";
                    caso_2 <= "1111";
                    caso_3 <= "1111";
                    caso_4 <= "1111";
                elsif (v_show = "0001") then
                        caso_1 <= "1000";
                        caso_2 <= "1111";
                        caso_3 <= "1111";
                        caso_4 <= "1111";
                elsif (v_show = "0010") then
                        caso_1 <= "1111";
                        caso_2 <= "1000";
                        caso_3 <= "1111";
                        caso_4 <= "1111";    
                elsif (v_show = "0100") then
                        caso_1 <= "1111";
                        caso_2 <= "1111";
                        caso_3 <= "1000";
                        caso_4 <= "1111";
                elsif (v_show = "1000") then
                        caso_1 <= "1111";
                        caso_2 <= "1111";
                        caso_3 <= "1111";
                        caso_4 <= "1000"; 
                elsif (v_show = "1111") then
                        caso_1 <= "1110";
                        caso_2 <= "1110";
                        caso_3 <= "1110";
                        caso_4 <= "1110";
                elsif (v_show = "1001") then
                        caso_1 <= "0000";
                        caso_2 <= "0000";
                        caso_3 <= "0000";
                        caso_4 <= "0000";                
                elsif(v_show = "0000") then
                        caso_1 <= "1111";
                        caso_2 <= "1111";
                        caso_3 <= "1111";
                        caso_4 <= "1111";
                end if;         
          end process;
        
end Behavioral;
