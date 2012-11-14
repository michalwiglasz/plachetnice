
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;



architecture main of tlv_gp_ifc is

   -- spi decoder module
   component SPI_adc
      generic (
         ADDR_WIDTH : integer;
         DATA_WIDTH : integer;
         ADDR_OUT_WIDTH : integer;
         BASE_ADDR  : integer
      );
      port (
         CLK      : in  std_logic;

         CS       : in  std_logic;
         DO       : in  std_logic;
         DO_VLD   : in  std_logic;
         DI       : out std_logic;
         DI_REQ   : in  std_logic;

         ADDR     : out  std_logic_vector (ADDR_OUT_WIDTH-1 downto 0);
         DATA_OUT : out  std_logic_vector (DATA_WIDTH-1 downto 0);
         DATA_IN  : in   std_logic_vector (DATA_WIDTH-1 downto 0);

         WRITE_EN : out  std_logic;
         READ_EN  : out  std_logic
      );
   end component;


   -- servo control register module
   component servo_ctrl_reg is
      generic (
         BITS: integer
      );
      port (
         CLK, RESET: in std_logic;
         WE: in std_logic;

         DIN: in std_logic_vector(BITS-1 downto 0);
         DOUT: out std_logic_vector(BITS-1 downto 0);
         MUX: in std_logic;

         DOUT_MAX: out std_logic_vector(BITS-1 downto 0);
         DOUT_FLIP: out std_logic_vector(BITS-1 downto 0)
      );
   end component;


   -- pulse width modulation module
   component PWM is
      generic (
         BITS: integer
      );
      port (
         CLK, RESET: in std_logic;
         MAX_VAL: std_logic_vector(BITS-1 downto 0);
         FLIP_VAL: in std_logic_vector(BITS-1 downto 0);
         EN: in std_logic;

         DOUT: out std_logic
      );
   end component;


   signal spi_addr: std_logic_vector(0 downto 0);
   signal spi_din: std_logic_vector(15 downto 0);
   signal spi_dout: std_logic_vector(15 downto 0);
   signal spi_we : std_logic;

   signal servo_a_en: std_logic := '1';
   signal servo_a_max: std_logic_vector(18 downto 0);
   signal servo_a_flip: std_logic_vector(18 downto 0);

   signal led_max: std_logic_vector(24 downto 0);
   signal led_flip: std_logic_vector(24 downto 0);

begin

   --"1100001101010000000",  -- 400 000 = 20 ms
   -- "0000010111011100000",  --  600 us,
   servo_a_max(2 downto 0) <= "000";
   servo_a_flip(2 downto 0) <= "000";

   servo_a_max(18 downto 3) <= "1100001101010000";

   led_max <= servo_a_max & "000000";
   led_flip <= servo_a_flip & "000000";

   spi: SPI_adc
      generic map (
        ADDR_WIDTH => 8,
        DATA_WIDTH => 16,
        ADDR_OUT_WIDTH => 1,
        BASE_ADDR  => 16#E000#  -- base address 0xE000
     )
     port map (
        CLK      => CLK,
        CS       => SPI_CS,

        DO       => SPI_DO,
        DO_VLD   => SPI_DO_VLD,
        DI       => SPI_DI,
        DI_REQ   => SPI_DI_REQ,

        ADDR     => spi_addr,
        DATA_OUT => spi_dout,
        DATA_IN  => spi_din,
        WRITE_EN => spi_we,
        READ_EN  => open
     );

   servo_a_reg: servo_ctrl_reg
      generic map (
         BITS => 16
      )
      port map (
         CLK => CLK,
         RESET => RESET,

         WE => spi_we,
         DIN => spi_dout,
         DOUT => spi_din,
         MUX => spi_addr(0),

         DOUT_MAX => open,
         DOUT_FLIP => servo_a_flip(18 downto 3)
      );

   servo_a_pwm: PWM
      generic map (
         BITS => 19
      )
      port map (
         CLK => CLK,
         RESET => RESET,
         MAX_VAL => servo_a_max,
         FLIP_VAL => servo_a_flip,
         EN => servo_a_en,
         DOUT => X(16)
      );


   led_pwm: PWM
      generic map (
         BITS => 25
      )
      port map (
         CLK => CLK,
         RESET => RESET,
         MAX_VAL => led_max,
         FLIP_VAL => led_flip,
         EN => servo_a_en,
         DOUT => LEDF
      );

end main;
