library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


-- https://www.cs.columbia.edu/~sedwards/classes/2012/4840/lines.pdf
entity lines is
    port (
    clk : in std_logic;
    rst : in std_logic;
    x0, y0, x1, y1 : in signed(10 downto 0);
    x_p, y_p : out signed(10 downto 0);
    start : in std_logic;
    done, plot : out std_logic;
    -- MODIFIED
    stall : in std_logic);
end lines;

architecture datapath of lines is
    signal x1x0, y1y0, dx, dy : signed(10 downto 0);
    signal down, right, e2_lt_dx, e2_gt_dy : std_logic;
    signal in_loop, break : std_logic;
    signal err, err1, err2, e2, err_next : signed(11 downto 0);
    signal x, y, x_next, y_next, xa, xb, ya, yb : signed(10 downto 0);
    
    type states is (IDLE_STATE, RUNNING_STATE, DONE_STATE);
    signal state : states;
begin

    -- datapath for dx, dy, right and down
    x1x0 <= x1 - x0;
    y1y0 <= y1 - y0;
    right <= not x1x0(10);
    down <= not y1y0(10);
    dx <= x1x0 when right = '1' else -x1x0;
    dy <= -y1y0 when down = '1' else y1y0;
    
    -- datapath for err
    err_next <= ("0" & dx) + ("0" & dy) when in_loop = '0' else err2;
    err2 <= err1 + dx when e2_lt_dx = '1' else err1;
    err1 <= err + dy when e2_gt_dy = '1' else err;
    e2 <= err(10 downto 0) & "0";
    e2_gt_dy <= '1' when e2 > dy else '0';
    e2_lt_dx <= '1' when e2 < dx else '0';
    
    -- datapath for x and y
    x_next <= x0 when in_loop = '0' else xb;
    xb <= xa when e2_gt_dy = '1' else x;
    xa <= x + 1 when right = '1' else x - 1;
    y_next <= y0 when in_loop = '0' else yb;
    yb <= ya when e2_lt_dx = '1' else y;
    ya <= y + 1 when down = '1' else y - 1;
    break <= '1' when x = x1 and y = y1 else '0';
    process (clk)
    begin
        if rising_edge(clk) then
            if(stall = '0') then
                err <= err_next;
                x <= x_next;
                y <= y_next;
            end if;
        end if;
    end process;

    -- Control FSM

    fsm : process (clk)
    begin
        if rising_edge(clk) then
            if rst = '1' then
                state <= IDLE_STATE;
            elsif stall = '0' then
                case state is
                    when IDLE_STATE =>
                        if start = '1' then state <= RUNNING_STATE; end if;
                    when RUNNING_STATE =>
                        if break = '1' then state <= DONE_STATE; end if;
                    when DONE_STATE =>
                        if start = '1' then state <= RUNNING_STATE;
                        else state <= IDLE_STATE;
                        end if;
                end case;
            end if;
        end if;
    end process;
    
    in_loop <= '1' when state = RUNNING_STATE else '0';
    --done <= '1' when state = DONE_STATE else '0';
    done <= '1' when state = DONE_STATE or state = IDLE_STATE else '0';
    plot <= in_loop;
    
    x_p <= x;
    y_p <= y;
    
end datapath;