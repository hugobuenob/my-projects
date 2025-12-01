library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Simon is
    Port (  clk       : in std_logic;
            reset     : in std_logic;
            btn_0     : in std_logic;
            btn_1     : in std_logic;
            btn_2     : in std_logic;
            btn_3     : in std_logic;
            segmentos : out std_logic_vector(6 downto 0);
	        selector  : out std_logic_vector (3 downto 0);
            leds      : out std_logic_vector(7 downto 0));
end Simon;

architecture Behavioral of Simon is
    type State is (WAIT_START, GENERATE_SEQUENCE, ROUND_LED, DISPLAY_SEQUENCE, DISPLAY_OFF, WAIT_INPUT, CHECK_INPUT, CORRECT, INCORRECT, GAME_OVER, WIN);
    signal current_state : State := WAIT_START;
    signal cnt           : integer :=0;             -- Contador Integer para cuenta el numero de randoms generados (=8)
    signal i             : integer :=0;             -- Variable para comparar con cnt el numero de ronda y saber con que 4 bits comparar
    signal j             : integer :=0;             -- Variable para comparar con cnt el numero de ronda y mostrar en los displays de cada ronda por un cierto tiempo 
    signal bit_count     : integer :=0;             -- Contador para secuencia y encendido de LEDs
    signal nled          : std_logic_vector(7 downto 0) := "00000000";        -- Senal que toma los bits de la secuencia y lo carga al outpu leds
    
    signal btn_f0, btn_f1 , btn_f2, btn_f3: std_logic;  -- Senal de cada boton despues del Debounce Program   
    signal any_btn_pressed : std_logic;                         -- Se�al que indica si uno de los botones fue presionado
    
    signal input_vector  : std_logic_vector(3 downto 0);         -- Senal que almacena el dato pulsado por el user
    type input_values  is array (31 downto 0) of std_logic_vector(3 downto 0);       -- ARRAY que guarda los 4 bits de cada entrada digitada en cada ronda
    signal random        : std_logic_vector(3 downto 0);        -- Numero random generado en Naleatorio Program
    --signal random_vector : std_logic_vector(3 downto 0);        -- Senal que toma el valor del random generado en cada ronda
    signal random_values : std_logic_vector(31 downto 0);       -- Senal que guarda los 4 bits de cada random generado en cada ronda
    signal v_show        : std_logic_vector(3 downto 0);        -- Random a enviar para encender a Display
    
    constant c_max_10s  : integer := 125*10**0 ;            -- Valor maximo del contador de 10 segundos (125*10**7)(/0.1) ; ej 125*10^0 = 125ns
    signal cont_10s	    : integer range 0 to c_max_10s-1;           -- Contador de 10 seg (Game Over o Win)    
    constant c_max_2s   : integer := 250*4/10 ;            -- Valor maximo del contador de 2 segundos (250*10**6); ej 250*4/10 = 1ns
	signal cont_2s      : integer range 0 to c_max_2s-1;             -- Contador de 2 segundos de espera de INPUT   
	constant c_max_1s   : integer := 125*4/10 ;            -- Valor maximo del contador de 1 segundo (125*10**6) ; ej 250*4/10 = 0.5 ns
	signal cont_1s      : integer range 0 to c_max_1s-1;             -- Contador de 1 segundo mostrar Display 
	signal cont_1s_off  : integer range 0 to c_max_1s-1;             -- Contador de 1 segundo apagar Display entre secuencias 
	
    component Naleatorio is
         Port (     clk      : in std_logic;
                    reset    : in std_logic;
                    random   : out std_logic_vector(3 downto 0) );
    end component;    
 
    component Debounce is
         Port (     clk      : in std_logic;
                    reset    : in std_logic;
                    b        : in std_logic;
                    btn_f    : out std_logic );
    end component;
    
    component Display is
         Port (     clk         : in std_logic;
                    reset       : in std_logic;
                    v_show      : in std_logic_vector(3 downto 0);
                    --- BCD conters
                    segmentos  : out std_logic_vector (6 downto 0);
                    selector   : out std_logic_vector (3 downto 0));
    end component;

begin
----- PortMAP Naleatorio
    Aleatorio: entity work.Naleatorio
	port map ( clk => clk, reset => reset, random => random);

----- PortMAP Debouncer
    Boton1: entity work.Debounce
	port map ( clk => clk, reset => reset, b => btn_0, btn_f => btn_f0);
    Boton2: entity work.Debounce
	port map ( clk => clk, reset => reset, b => btn_1, btn_f => btn_f1);
	Boton3: entity work.Debounce
	port map ( clk => clk, reset => reset, b => btn_2, btn_f => btn_f2);
	Boton4: entity work.Debounce
	port map ( clk => clk, reset => reset, b => btn_3, btn_f => btn_f3);

----- PortMAP Display
    Show7seg: entity work.Display
	port map ( clk => clk, reset => reset, v_show =>v_show, segmentos => segmentos, selector => selector );


    process(clk, reset)
    begin
        if (reset = '1') then
            bit_count <= 0;
            cont_1s <= 0;
            cont_10s <= 0;
            cont_2s <= 0;
            cnt <= 0;
            nled <= "00000000";
            current_state <= WAIT_START; 
            leds(cnt) <= '0' ;
            input_vector <= "0000";
            v_show <= "0000";
            random_values <= "00000000000000000000000000000000";
        elsif (clk'event and clk = '1') then
            case current_state is
                when WAIT_START =>          -- Inicializaci�n del juego
                    v_show <= "0000";
                    leds <= nled;
                    any_btn_pressed <= btn_f0 or btn_f1 or btn_f2 or btn_f3;
				   if (any_btn_pressed = '1') then  
                        bit_count <= 0; 
                        i <= 0;
                        current_state <= GENERATE_SEQUENCE;                        
                    end if;

                when GENERATE_SEQUENCE => 
                       if (cnt < 8) then
                            random_values((cnt*4)+3 downto cnt*4) <= random; 
                            current_state <= ROUND_LED;     
                       elsif (cnt = 8) then
                           cnt <= 0;
                           current_state <= WIN;
                       end if;                     
                       
                when ROUND_LED =>  
                        cont_2s <= 0; 
                        leds(cnt) <= '1' ;
                    current_state <= DISPLAY_SEQUENCE;

                                
                when DISPLAY_SEQUENCE =>
                        v_show <= random_values((j*4)+3 downto j*4);
                       if (j < cnt) then
                         if (cont_1s < c_max_1s-1) then
                            cont_1s <= cont_1s + 1;
                          else
                              cont_1s <= 0;
                              j <= j + 1;
                              v_show <= "0000";
                              current_state <= DISPLAY_OFF; 
                          end if;
                      elsif j = cnt then
                          if (cont_1s < c_max_1s-1) then
                            cont_1s <= cont_1s + 1;
                          else
                              cont_1s <= 0; 
                              v_show <= "0000";
                              j <= 0;
                              current_state <= WAIT_INPUT; 
                       end if;
                      end if;
                      
                when DISPLAY_OFF => 
                              if (cont_1s_off < c_max_1s-1)then
                                cont_1s_off <= cont_1s_off +1;
                              else
                                  cont_1s_off <= 0;
                                  current_state <= DISPLAY_SEQUENCE; 
                              end if;  
                        
                 when WAIT_INPUT =>
                    	       	input_vector <= (btn_f3, btn_f2, btn_f1, btn_f0);
                    	       	any_btn_pressed <= btn_f0 or btn_f1 or btn_f2 or btn_f3;
                    	       if (any_btn_pressed = '1') then
                    	              input_vector <= input_vector;
                    	              cont_2s <= 0;
                                      current_state <= CHECK_INPUT;
                               elsif (cont_2s < c_max_2s-1) then
                                      cont_2s <= cont_2s + 1;
                               else
                                      cont_2s <= 0;
                                      current_state <= GAME_OVER;
                               end if;

                when CHECK_INPUT =>
                   	if input_vector = random_values((i*4)+3 downto i*4) then
                            if (i < cnt) then
                                i <= i + 1;
                                current_state <= WAIT_INPUT;
                            elsif i = cnt then
                                i <= 0;
                                current_state <= CORRECT;
                            end if;
                        else
                            current_state <= INCORRECT;   
                     end if;
                 
                 
                 when CORRECT =>

                            if (bit_count < 8) then
                                cnt <= cnt + 1;
                                bit_count <= bit_count + 1;
                                i <= 0;
                                current_state <= GENERATE_SEQUENCE;
                            else
                                bit_count <= 0;
                                cnt <= 0;
                                current_state <= WIN;
                            end if;    
                            
                  
                 when INCORRECT => 
      
                        current_state <= GAME_OVER;

                when GAME_OVER =>
                  -- Esperar 10 segundos
                        cnt <= 0;
                      if (cont_10s < c_max_10s-1) then
                            cont_10s <= cont_10s +1;
                            v_show <= "1001";      
                       else
                           cont_10s <= 0;
                           current_state <= WAIT_START;
                       end if;

                 when WIN =>
                    -- Esperar 10 segundos
                    cnt <= 0;
                    if (cont_10s < c_max_10s-1) then		
                            cont_10s <= cont_10s + 1;
                            v_show <= "1111";    
                       else
                           cont_10s <= 0;			
			         current_state <= WAIT_START;
                       end if;
            end case;
        end if;
    end process;

end Behavioral;