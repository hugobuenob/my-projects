--------------------------------------------
-- Digital Signal Processing              --
-- Practical work: Digital filter in FPGA --
--------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity filter_top is
    generic (
        ---------------------
        -- USER PARAMETERS --
        ---------------------
        
        -- NOTE: Users have to modify this section to include their custom filter parameters (obtained in MATLAB fdatool)
        
        C_FILTER_TYPE : string := "FIR";      -- Filter type: FIR or IIR (only affects SERIAL implementation)
        C_FILTER_MODE : string := "SERIAL";   -- Filter implementation. Can be fully PARALLEL, fully SERIAL, PARtial SERial or a plain BYPASS (default)
        C_FILTER_ORD  : integer := 128;       -- Filter order (only used in SERIAL implementation)
        C_FILTER_FOLD : integer := 128;        -- Filter folding factor (only used in PARtial SERial implementation)
        C_FILTER_BITS : integer := 10;        -- Filter output data resolution (usually different than DAC resolution if full filter resolution is enabled in MATLAB)
        
        -----------------------
        -- SYSTEM PARAMETERS --
        -----------------------
        
        -- WARNING: Users MUST NOT change anything here unless otherwise specified
        
        C_SYS_FREQ    : integer := 50000000;  -- System frequency in Hz
        C_SAMPLE_FREQ : integer := 100000;    -- Sampling frequency in Hz (1MHz maximum in AD7991, limited to approx. 80kHz in Pmod AD2) [OVERCLOCKED]
        C_I2C_FREQ_NM : integer := 400000;    -- I2C interface clock frequency (Normal/Fast mode) in Hz (100-400kHz) [FAST MODE]
        C_I2C_FREQ_HS : integer := 5000000;   -- I2C interface clock frequency (High Speed mode) in Hz (3.4MHz maximum, 1.7MHz minimum) [OVERCLOCKED]
        C_I2C_FREQ_ST : integer := 500000;    -- I2C interface clock stretching timing frequency in Hz (2us -> 500kHz)
        C_SPI_FREQ    : integer := 25000000;  -- SPI interface clock frequency in Hz (30MHz maximum in Pmod DA1)
        C_RST_POL     : std_logic := '1'      -- Reset polarity
    );
    port (
        -- Clock and reset
        clk    : in  std_logic;
        reset  : in  std_logic;
        -- Serial input interface (I2C)
        scl    : inout std_logic; -- Clock
        sda    : inout std_logic; -- Data
        -- Serial output interface (SPI)
        sclk   : out std_logic; -- Clock
        din    : out std_logic; -- Data
        sync_n : out std_logic  -- Chip select (active-low)        
    );
    -- DEBUG    
    attribute mark_debug : string;
    attribute mark_debug of scl    : signal is "TRUE"; 
    attribute mark_debug of sda    : signal is "TRUE"; 
    attribute mark_debug of sclk   : signal is "TRUE"; 
    attribute mark_debug of din    : signal is "TRUE"; 
    attribute mark_debug of sync_n : signal is "TRUE"; 
end filter_top;

architecture top of filter_top is

    ----------------------
    -- Sampling signals --
    ----------------------
    
    signal enable : std_logic;

    -----------------
    -- ADC signals --
    -----------------
    
    signal adc_data  : std_logic_vector(11 downto 0);
    signal adc_valid : std_logic;
    signal adc_ready : std_logic;

    -----------------
    -- DAC signals --
    -----------------
    
    signal dac_data  : std_logic_vector(7 downto 0);
    signal dac_valid : std_logic;
    signal dac_ready : std_logic;
    
    -----------
    -- DEBUG --
    -----------
    
    attribute mark_debug of enable    : signal is "TRUE"; 
    attribute mark_debug of adc_data  : signal is "TRUE"; 
    attribute mark_debug of adc_valid : signal is "TRUE"; 
    attribute mark_debug of adc_ready : signal is "TRUE"; 
    attribute mark_debug of dac_data  : signal is "TRUE"; 
    attribute mark_debug of dac_valid : signal is "TRUE"; 
    attribute mark_debug of dac_ready : signal is "TRUE"; 

begin

    -------------------
    -- Sampling unit --
    -------------------
    
    -- Unit in charge of generating enable signals to sample input
    sampler_i: entity work.sampler
    generic map (
        C_SYS_FREQ    => C_SYS_FREQ,
        C_SAMPLE_FREQ => C_SAMPLE_FREQ,
        C_RST_POL     => C_RST_POL
    )
    port map (
        clk    => clk,
        reset  => reset,
        enable => enable
    );
    
    ---------------
    -- ADC logic --
    ---------------

    -- PMOD AD2 controller
    ad2_i: entity work.ad2_ctrl
    generic map (
        C_SYS_FREQ    => C_SYS_FREQ,
        C_I2C_FREQ_NM => C_I2C_FREQ_NM,
        C_I2C_FREQ_HS => C_I2C_FREQ_HS,
        C_I2C_FREQ_ST => C_I2C_FREQ_ST,   
        C_RST_POL     => C_RST_POL
    )
    port map (
        clk     => clk,
        reset   => reset,
        capture => enable,
        scl     => scl,
        sda     => sda,
        data    => adc_data,
--        data    => open, -- Uncomment this line and comment the previous one to emulate measurements (testbench only!)
        valid   => adc_valid,
        ready   => adc_ready
    );
    
    ----------
    -- TEST --
    ----------
    
    -- NOTE: this section is intended to be used as part of the simulation framework.
    --       Since no I2C slave is present in the testbench, users can leave the data
    --       port of the ADC controller open and use the following logic to emulate 
    --       external measurements acquired by the ADC.
    
--    -- RAMP GENERATION
--    meas_gen: process(clk, reset)
--    begin
--        if reset = C_RST_POL then
--            adc_data <= (others => '0');
--        elsif clk'event and clk = '1' then
--            if adc_valid = '1' then
--                adc_data <= std_logic_vector(unsigned(adc_data) + 1);
--            end if;            
--        end if;
--    end process;
    
--    -- STEP GENERATION
--    adc_data <= (11 => '0', others => '1');
            
    ------------------
    -- Filter logic --
    ------------------
    
    -- NOTE: when designing the filter, some implementation details have to be fixed so that this template works:
    --       1. When using fixed-point arithmetic, inputs have 12 bit resolution (decimal part arbitrary) [ADC resolution]
    --       2. When using fixed-point arithmetic, outputs have 8 bit resolution (4 bits for decimal part) [DAC resolution].
    --          If MATLAB is set to generate filters with FULL resolution, the output signal of the filter has to be truncated
    --          so that the DAC can have valid input data
    --       3. The reset polarity in MATLAB has to be set to match C_RST_POL (otherwhise the filter won't work)
    --       4. The addition of pipeline registers is mandatory whenever it can be selected (otherwhise, latency control will fail)
        
    -- Filter bypass --
    bypass_gen: if C_FILTER_MODE = "BYPASS" generate
    begin
    
        -- (ADC output goes straight into DAC input)
        dac_data <= adc_data(11 downto 4);
        dac_valid <= adc_valid;
        
    end generate;
    
    -- Fully parallel implementation --
--    parallel_gen: if C_FILTER_MODE = "PARALLEL" generate
--        -- Signal definitions
--        signal clk_enable : std_logic;
--        signal filter_in  : std_logic_vector(11 downto 0);
--        signal filter_out : std_logic_vector(C_FILTER_BITS-1 downto 0);
--    begin
--    
--        -- Filter instantiation
--        filter_i: entity work.filter_parallel
--        port map (
--            clk        => clk,
--            clk_enable => clk_enable,
--            reset      => reset,            
--            filter_in  => filter_in,
--            filter_out => filter_out
--        );
--        
--        -- Filter input connection
--        filter_in <= (not adc_data(11)) & adc_data(10 downto 0); -- ADC uses unsigned notation; filter uses signed
--        
--        -- Filter output connection
--        dac_data <= (not filter_out(C_FILTER_BITS-1)) & filter_out(C_FILTER_BITS-2 downto C_FILTER_BITS-8); -- DAC uses unsigned notation; filter uses signed
--        
--        -- Filter enable generation
--        clk_enable <= adc_valid;   
--             
--        -- DAC enable generation   
--        dac_en: process(clk, reset)
--        begin
--            if reset = C_RST_POL then
--                dac_valid <= '0';
--            elsif clk'event and clk = '1' then
--                -- Valid data appears at the output of the filter one clock cycle after its enable signal is asserted
--                dac_valid <= clk_enable;
--            end if;
--        end process;             
--    
--    end generate;
    
    -- Fully serial implementation --
    serial_gen: if C_FILTER_MODE = "SERIAL" generate
        -- Signal definitions
        signal clk_enable     : std_logic;
        signal clk_enable_dly : std_logic;
        signal filter_in      : std_logic_vector(11 downto 0);
        signal filter_out     : std_logic_vector(C_FILTER_BITS-1 downto 0);
    begin
    
 --        Filter instantiation
        filter_i: entity work.filter_serial
        port map (
            clk        => clk,
            clk_enable => clk_enable,
            reset      => reset,            
            filter_in  => filter_in,
            filter_out => filter_out
        );
        
        -- Filter output connection
        dac_data <= (not filter_out(C_FILTER_BITS-1)) & filter_out(C_FILTER_BITS-2 downto C_FILTER_BITS-8); -- DAC uses unsigned notation; filter uses signed
        
        -- FIR filters
        fir_gen: if C_FILTER_TYPE = "FIR" generate
            signal count : integer range 0 to C_FILTER_ORD;
        begin
        
            -- Filter enable generation
            filter_en: process(clk, reset)
            begin
                if reset = C_RST_POL then                
                    clk_enable <= '0';
                    filter_in <= (others => '0');
                    count <= 0;
                elsif clk'event and clk = '1' then
                    if adc_valid = '1' then
                        clk_enable <= '1';
                        filter_in <= (not adc_data(11)) & adc_data(10 downto 0); -- ADC uses unsigned notation; filter uses signed
                    end if;
                    if clk_enable = '1' then
                        if count = C_FILTER_ORD then
                            clk_enable <= '0';
                            count <= 0;         
                        else
                            count <= count + 1;
                        end if;
                    end if;
                end if;
            end process;
        
        end generate; 
        
        -- IIR filters (TODO: check if this is working or not)
        iir_gen: if C_FILTER_TYPE = "IIR" generate
            signal count : integer range 0 to 3*C_FILTER_ORD;
        begin
        
            -- Filter enable generation
            filter_en: process(clk, reset)
            begin
                if reset = C_RST_POL then                
                    clk_enable <= '0';
                    filter_in <= (others => '0');
                    count <= 0;
                elsif clk'event and clk = '1' then
                    if adc_valid = '1' then
                        clk_enable <= '1';
                        filter_in <= (not adc_data(11)) & adc_data(10 downto 0); -- ADC uses unsigned notation; filter uses signed
                    end if;
                    if clk_enable = '1' then
                        if count = 3*C_FILTER_ORD then
                            clk_enable <= '0';
                            count <= 0;         
                        else
                            count <= count + 1;
                        end if;
                    end if;
                end if;
            end process;
        
        end generate;      
                     
        -- DAC enable generation
        dac_en: process(clk, reset)
        begin
            if reset = C_RST_POL then
                clk_enable_dly <= '0';
                dac_valid <= '0';
            elsif clk'event and clk = '1' then
                -- Valid data appears at the output of the filter one clock cycle after its enable signal is asserted
                clk_enable_dly <= clk_enable;
                -- Rising edge detection (registered)
                dac_valid <= clk_enable and (not clk_enable_dly); 
            end if;
        end process; 
    
    end generate;
--    
    -- Partial serial implementation --
--    parser_gen: if C_FILTER_MODE = "PARSER" generate
--        -- Signal definitions
--        signal clk_enable     : std_logic;
--        signal clk_enable_dly : std_logic;
--        signal filter_in      : std_logic_vector(11 downto 0);
--        signal filter_out     : std_logic_vector(C_FILTER_BITS-1 downto 0);
--        signal count          : integer range 0 to C_FILTER_FOLD+1;
--    begin
--    
--        -- Filter instantiation
--        filter_i: entity work.filter_parser
--        port map (
--            clk        => clk,
--            clk_enable => clk_enable,
--            reset      => reset,            
--            filter_in  => filter_in,
--            filter_out => filter_out
--        );
--        
--        -- Filter output connection
--        dac_data <= (not filter_out(C_FILTER_BITS-1)) & filter_out(C_FILTER_BITS-2 downto C_FILTER_BITS-8); -- DAC uses unsigned notation; filter uses signed
--        
--        -- Filter enable generation
--        enable_proc: process(clk, reset)
--        begin
--            if reset = C_RST_POL then                
--                clk_enable <= '0';
--                filter_in <= (others => '0');
--                count <= 0;
--            elsif clk'event and clk = '1' then
--                if adc_valid = '1' then
--                    clk_enable <= '1';
--                    filter_in <= (not adc_data(11)) & adc_data(10 downto 0); -- ADC uses unsigned notation; filter uses signed
--                end if;
--                if clk_enable = '1' then
--                    if count = C_FILTER_FOLD+1 then
--                        clk_enable <= '0';
--                        count <= 0;         
--                    else
--                        count <= count + 1;
--                    end if;
--                end if;
--            end if;
--        end process;
--        
--        -- DAC enable generation
--        dac_en: process(clk, reset)
--        begin
--            if reset = C_RST_POL then
--                clk_enable_dly <= '0';
--                dac_valid <= '0';
--            elsif clk'event and clk = '1' then
--                -- Valid data appears at the output of the filter one clock cycle after its enable signal is asserted
--                clk_enable_dly <= clk_enable;
--                -- Rising edge detection (registered)
--                dac_valid <= clk_enable and (not clk_enable_dly); 
--            end if;
--        end process;      
--            
--    end generate;
            
    ---------------
    -- DAC logic --
    ---------------

    -- PMOD DA1 controller
    da1_i: entity work.da1_ctrl
    generic map (
        C_SYS_FREQ => C_SYS_FREQ,
        C_SPI_FREQ => C_SPI_FREQ,
        C_RST_POL  => C_RST_POL
    )
    port map (
        clk    => clk,
        reset  => reset,
        data   => dac_data,
        valid  => dac_valid,
        ready  => dac_ready,
        sclk   => sclk,
        din    => din,
        sync_n => sync_n  
    );

end top;
