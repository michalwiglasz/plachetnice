library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;


entity servo is
   generic (
      BASE_ADDR  : integer
   );
   port (
      CLK          : in  std_logic;
      RESET        : in  std_logic;

      SPI_CS       : in  std_logic;
      SPI_DO       : in  std_logic;
      SPI_DO_VLD   : in  std_logic;
      SPI_DI       : out std_logic;
      SPI_DI_REQ   : in  std_logic;

      SERVO        : out std_logic;
      LED          : out std_logic
   );
end servo;



architecture basic of servo is

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

signal servo_en: std_logic := '1';
signal servo_max: std_logic_vector(18 downto 0);
signal servo_flip: std_logic_vector(18 downto 0);

signal led_max: std_logic_vector(24 downto 0);
signal led_flip: std_logic_vector(24 downto 0);


begin
  servo_max(2 downto 0) <= "000";
  servo_flip(2 downto 0) <= "000";

  servo_max(18 downto 3) <= "1100001101010000";

  led_max <= servo_max & "000000";
  led_flip <= servo_flip & "000000";

  spi: SPI_adc
     generic map (
       ADDR_WIDTH => 8,
       DATA_WIDTH => 16,
       ADDR_OUT_WIDTH => 1,
       BASE_ADDR  => BASE_ADDR
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


  reg: servo_ctrl_reg
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
        DOUT_FLIP => servo_flip(18 downto 3)
     );


  servo_pwm: PWM
     generic map (
        BITS => 19
     )
     port map (
        CLK => CLK,
        RESET => RESET,
        MAX_VAL => servo_max,
        FLIP_VAL => servo_flip,
        EN => servo_en,
        DOUT => SERVO
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
        EN => servo_en,
        DOUT => LED
     );

end basic;