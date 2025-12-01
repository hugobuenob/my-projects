library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

entity bresenham is
generic(
    POINT_SIZE : integer := 11
);
port(
    -- Clock and reset
    clk_100M : in std_logic;
    resetn : in std_logic;
    -- Input coordiantes
    i_px0, i_py0, i_px1, i_py1 : in std_logic_vector(POINT_SIZE-1 downto 0);
    -- Input control signals
    i_endpoints_valid : in std_logic;
    o_endpoints_ready : out std_logic;
    -- Output coordinates
    o_point_x, o_point_y : out std_logic_vector(POINT_SIZE-1 downto 0);
    -- Output control signals
    i_point_ready : in std_logic;
    o_point_valid : out std_logic
);
end bresenham;

architecture Behavioral of bresenham is
  
  -- Debug signals
  attribute mark_debug : string;
  attribute mark_debug of i_px0 : signal is "true";
  attribute mark_debug of i_py0 : signal is "true";
  attribute mark_debug of i_px1 : signal is "true";
  attribute mark_debug of i_py1 : signal is "true";
  attribute mark_debug of i_endpoints_valid : signal is "true";
  attribute mark_debug of o_endpoints_ready : signal is "true";
  attribute mark_debug of o_point_x : signal is "true";
  attribute mark_debug of o_point_y : signal is "true";
  attribute mark_debug of i_point_ready : signal is "true";
  attribute mark_debug of o_point_valid : signal is "true";
  --
  
  -- FSM States
  type state_type is (S_IDLE, S_REG_IN, S_CALC);
  signal state : state_type;
  signal error_s : signed(POINT_SIZE-1 downto 0);
  begin
  -- Endpoint control
  o_endpoints_ready <= '1' when state = S_IDLE else '0';
  -- Output points control
  o_point_valid <= '1' when state = S_CALC else '0';
  
  -- Sequential process main FSM
  process(clk_100M, resetn)
  
  variable x_incr, y_incr, error : signed(POINT_SIZE-1 downto 0);
  variable driving_axis : std_logic;
  variable point_x, point_y, point_x1, point_y1 : signed(POINT_SIZE-1 downto 0);
  
  begin
  
    if(resetn = '0') then
      state <= S_IDLE;
      
    elsif(rising_edge(clk_100M)) then
       
      case state is      
      
        when S_IDLE =>
          if(i_endpoints_valid = '1') then
            state <= S_REG_IN;
            x_incr := signed(i_px1) - signed(i_px0);
            y_incr := signed(i_py1) - signed(i_py0);
            point_x := signed(i_px0);
            point_y := signed(i_py0);
            point_x1 := signed(i_px1);
            point_y1 := signed(i_py1);
          end if;
          
        when S_REG_IN =>
--          x_incr := signed(i_px1) - signed(i_px0);
--          y_incr := signed(i_py1) - signed(i_py0);
--          point_x := signed(i_px0);
--          point_y := signed(i_py0);
--          point_x1 := signed(i_px1);
--          point_y1 := signed(i_py1);
          
          -- First quadrant
            if(x_incr >= 0) and (y_incr >= 0) then
              -- Driving axis x  
              if(x_incr >= y_incr) then
                error := y_incr + y_incr - x_incr;
              -- Driving axis y
              else
                error := x_incr + x_incr - y_incr;
              end if;
                            
          -- Second quadrant
            elsif(x_incr <= 0) and (y_incr >= 0) then
               -- Driving axis x
               if(-x_incr >= y_incr) then
                 error := y_incr + y_incr + x_incr;
               -- Driving axis y
               else
                 error := -x_incr - x_incr - y_incr;
               end if;
           
           -- Third quadrant
            elsif(x_incr <= 0) and (y_incr <= 0) then
              -- Driving axis x
              if(-x_incr >= -y_incr) then
                error := -y_incr -y_incr  + x_incr;
              else 
                error := -x_incr -x_incr + y_incr;
              end if;
            -- Fourth quadrant
            else
              -- Driving axis x
              if(x_incr >= -y_incr) then
                error := - y_incr - y_incr - x_incr;
              else 
                error := x_incr + x_incr + y_incr; 
              end if;
            end if;
            
          -- Start calculating points on the next clock cycle
          state <= S_CALC; 
          
        when S_CALC =>
          if (i_point_ready = '1') then
          -- Check for line completition
            if (point_x = point_x1) and (point_y = point_y1) then
              state <= S_IDLE;
            else     
          -- Continue drawing
          -- First quadrant
            if(x_incr >= 0) and (y_incr >= 0) then
              -- Driving axis x  
              if(x_incr >= y_incr) then
                  point_x := point_x + 1;        
                  if(error > 0) then
                    point_y := point_y + 1;
                    error := error - x_incr - x_incr;
                  end if;
                  error := error + y_incr + y_incr;
              -- Driving axis y
              else
                point_y := point_y + 1;
                if(error > 0) then
                  point_x := point_x + 1;
                  error := error - y_incr - y_incr; 
                end if;
                error := error + x_incr + x_incr;
              end if;            
               
          -- Second quadrant
            elsif(x_incr <= 0) and (y_incr >= 0) then
               -- Driving axis x
               if(-x_incr >= y_incr) then
                 point_x := point_x - 1;
                 if(error > 0) then
                   point_y := point_y +1;
                   error := error + x_incr + x_incr;
                 end if;
                 error := error + y_incr + y_incr;
               -- Driving axis y
               else
                 point_y := point_y + 1;                 
                 if(error > 0) then
                   point_x := point_x - 1;
                   error := error - y_incr - y_incr; 
                 end if;
                 error := error - x_incr - x_incr;
               end if;
               
           -- Third quadrant
            elsif(x_incr <= 0) and (y_incr <= 0) then
              -- Driving axis x
              if(-x_incr >= -y_incr) then
                point_x := point_x - 1;                          
                if(error > 0) then
                  point_y := point_y - 1;
                  error := error + x_incr + x_incr;
                end if;
                error := error - y_incr - y_incr;        
              else 
                point_y := point_y - 1;
                if(error > 0) then
                  point_x := point_x - 1;
                  error := error + y_incr + y_incr; 
                end if;
                error := error - x_incr - x_incr;
              end if;
              
            -- Fourth quadrant
            else
              -- Driving axis x
              if(x_incr >= -y_incr) then
                point_x := point_x + 1;                 
                if(error > 0) then
                  point_y := point_y - 1;
                  error := error - x_incr - x_incr;
                end if;
                error := error - y_incr - y_incr;   
              else 
                point_y := point_y - 1;
                if(error > 0) then
                  point_x := point_x + 1;
                  error := error + y_incr + y_incr; 
                end if;
                error := error + x_incr + x_incr;
              end if;
            end if;
          end if;
          end if;
       end case;
       error_s <= error;
       o_point_x <= std_logic_vector(point_x);
       o_point_y <= std_logic_vector(point_y);
     end if;
  end process;

  
end Behavioral;
